#include <SerializationStream.h>
#include <assert.h>
#include <cmath>
bool SerializationStream::AGA = false;
SerializationStream::SerializationStream(StreamGrower* grower)
{
	this->grower = grower;
	grower->grow(this, 0);
}

SerializationStream::SerializationStream()
{
}

SerializationStream::~SerializationStream()
{
	//printf("~SerializationStream()\n");
	delete grower;
	//printf("delete grower\n");
	for (size_t i = 0; i < blocks.size(); i++)
	{
		delete blocks[i];
	}
}

const char* SerializationStream::read(size_t length)
{
	char* result = &cursor.block->block[cursor.index];
	seekRelative(length);
	return result;
}

char * SerializationStream::readAll()
{
	char* result = new char[totalLength];
	size_t cursor = 0;
	for (size_t i = 0; i < blocks.size(); i++)
	{
		memcpy(result + cursor, blocks[i]->block, blocks[i]->occupied);
		cursor += blocks[i]->occupied;
	}
	return result;
}

char * SerializationStream::c_str()
{
	char* result = new char[totalLength + 1];
	size_t cursor = 0;
	for (size_t i = 0; i < blocks.size(); i++)
	{
		memcpy(result + cursor, blocks[i]->block, blocks[i]->occupied);
		cursor += blocks[i]->occupied;
	}
	result[totalLength] = '\0';
	return result;
}

char* SerializationStream::readAllAsHex()
{
	const char* code = "0123456789abcdef";
	char* result = new char[totalLength * 3];
	size_t cursor = 0;
	for (size_t i = 0; i < blocks.size(); i++)
	{
		for (size_t j = 0; j < blocks[i]->occupied; j++)
		{
			result[cursor * 3] = code[(unsigned char)blocks[i]->block[j] / 16];
			result[cursor * 3 + 1] = code[(unsigned char)blocks[i]->block[j] % 16];
			result[cursor * 3 + 2] = ' ';
			cursor++;
		}
	}
	return result;
}

char * SerializationStream::readAllAsHex_c()
{
	return nullTerminate(readAllAsHex(), totalLength * 3);
}

char* SerializationStream::readAllAsBase64()
{
	return nullptr;
}

char* SerializationStream::nullTerminate(char* source, size_t length)
{
	char* result = new char[length + 1];
	memcpy(result, source, length);
	result[length] = '\0';
	delete[] source;
	return result;
}

char* SerializationStream::peek()
{
	return &cursor.block->block[cursor.index];
}

void SerializationStream::seekAbsolute(size_t position)
{
	size_t passed = 0;
	for (size_t i = 0; i < blocks.size(); i++)
	{
		if (passed + blocks[i]->occupied > position)
		{
			cursor.block = blocks[i];
			cursor.index = position - passed;
			return;
		}
		else
		{
			passed += blocks[i]->occupied;
		}

	}
	if (passed == position)
	{
		cursor.block = blocks.back();
		cursor.index = cursor.block->occupied;
		return;
	}
	printf(" ");
	THROW_FATAL_ERROR("seekAbsolute is out if bounds (" + std::to_string(position) + ")");
}

void SerializationStream::seekRelative(size_t position)
{
	if (position + cursor.index > 0 && position + cursor.index <= cursor.block->occupied)
	{
		cursor.index = position + cursor.index;
		if (cursor.index == cursor.block->occupied && cursor.block != blocks.back())
		{
			cursor.index = 0;
			cursor.block = getNextBlock(cursor.block);
		}
	}
	else
	{
		seekAbsolute(getAbsolutePosition() + position);
	}
}

void SerializationStream::seekEnd()
{
	cursor.block = blocks.back();
	cursor.index = cursor.block->occupied;
}

size_t SerializationStream::getLength()
{
	return totalLength;
}

void SerializationStream::writeSimple(const char* data, size_t length)
{
	for (size_t i = 0; i < length; i++)
	{
		cursor.block->block[cursor.index + i] = data[i];
	}
	cursor.index += length;
}

//solve for total length
void SerializationStream::write(const char* data, size_t length)
{
	if (SerializationStream::AGA)
	{
		const char* code = "0123456789ABCDEF";
		char* result = new char[length * 3];
		size_t cr = 0;
		for (size_t i = 0; i < length; i++)
		{
				result[cr * 3] = code[(unsigned char)data[i] / 16];
				result[cr * 3 + 1] = code[(unsigned char)data[i] % 16];
				result[cr * 3 + 2] = ' ';
				cr++;
		}
		std::string h;
		h.assign(result, length * 3);
		SerializationStream::AGA = false;
		S::log.add("W:" + h);
		SerializationStream::AGA = true;
	}/**/
	if (cursor.block == blocks.back())
	{
		//we're at the end of the last block. grow if nesscessary and write
		if (cursor.index + length >= cursor.block->capacity)//todo if fits exactly - writesimple and then grow
		{
			totalLength -= cursor.block->occupied - cursor.index;
			cursor.block->occupied = cursor.index;
			grower->grow(this, length);
		}
		writeSimple(data, length);
		cursor.block->occupied = cursor.index;
		totalLength += length;
	}
	else
	{
		//overwrite within the boundaries of one block
		if (cursor.index + length < cursor.block->occupied)
		{
			writeSimple(data, length);
		}
		else
		{
			bool dataFits = cursor.index + length < cursor.block->capacity;
			size_t cutFromBeginningAmount = length - (cursor.block->occupied - cursor.index);

			//write to the end of one block and possibly delete some data from the next block
			if (dataFits)
			{
				for (size_t i = 0; i < length; i++)
				{
					cursor.block->block[cursor.index + i] = data[i];
				}
				cursor.block->occupied = cursor.index + length;

				cursor.block = getNextBlock(cursor.block);
				cursor.index = 0;

				if (cutFromBeginningAmount > cursor.block->occupied)
				{
					totalLength += cutFromBeginningAmount - cursor.block->occupied;
					cutFromBeginningAmount = cursor.block->occupied;
				}

				if (cutFromBeginningAmount > 0)
				{
					memmove(cursor.block->block, cursor.block->block + cutFromBeginningAmount, cursor.block->occupied - cutFromBeginningAmount);
					cursor.block->occupied -= cutFromBeginningAmount;
				}
			}
			//delete data from the current block and rebuild the next block entirely
			else
			{
				cursor.block->occupied = cursor.index;
				CharBlock* nextBlock = getNextBlock(cursor.block);
				CharBlock* newBlock = new CharBlock(length + nextBlock->occupied);

				if (cutFromBeginningAmount > cursor.block->occupied)
				{
					totalLength += cutFromBeginningAmount - cursor.block->occupied;
					cutFromBeginningAmount = cursor.block->occupied;
				}

				memcpy(newBlock->block, data, length);
				memcpy(newBlock->block + length, nextBlock->block + cutFromBeginningAmount, nextBlock->occupied - cutFromBeginningAmount);
				cursor.block = newBlock;
				cursor.index = length;

				for (size_t i = 0; i < blocks.size(); i++)
				{
					if (blocks[i] == nextBlock)
					{
						blocks[i] = newBlock;
						delete nextBlock;
						break;
					}
				}
			}
		}
	}

}

std::unique_ptr<SerializationStream> SerializationStream::createExp(size_t base, size_t exponent)
{
	return std::unique_ptr<SerializationStream>(new SerializationStream(new StreamGrowerExp(base, exponent)));
}

void SerializationStream::grow(size_t blockSize)
{
	CharBlock* newBlock = new CharBlock(blockSize);
	blocks.push_back(newBlock);
	cursor.block = newBlock;
	cursor.index = 0;
}

size_t SerializationStream::getAbsolutePosition()
{
	size_t passed = 0;
	for (size_t i = 0; i < blocks.size(); i++)
	{
		if (blocks[i] != cursor.block)
		{
			passed += blocks[i]->occupied;
		}
		else
		{
			passed += cursor.index;
			break;
		}
	}
	return passed;
}

CharBlock* SerializationStream::getNextBlock(CharBlock* currentBlock)
{
	for (size_t i = 0; i < blocks.size(); i++)
	{
		if (blocks[i] == currentBlock)
			return blocks[i + 1];
	}
	return nullptr;
}

StreamGrower::StreamGrower()
{
}

StreamGrower::~StreamGrower()
{
}

void StreamGrower::grow(SerializationStream * stream, size_t minimum)
{
	stream->grow(minimum);
}

size_t StreamGrower::getBlockCount(SerializationStream* stream)
{
	return stream->blocks.size();
}

CharBlock * StreamGrower::getLastBlock(SerializationStream* stream)
{
	return stream->blocks.back();
}

StreamGrowerExp::StreamGrowerExp(size_t base, size_t exponent)
{
	this->base = base;
	this->exponent = exponent;
}

StreamGrowerExp::~StreamGrowerExp()
{
}

void StreamGrowerExp::grow(SerializationStream * stream, size_t minimum)
{
	size_t oldBlockCount = getBlockCount(stream);
	size_t newBlockSize = (size_t)(base * std::pow(exponent, oldBlockCount));
	
	while (newBlockSize < minimum)
	{
		newBlockSize *= exponent;
	}

	StreamGrower::grow(stream, newBlockSize);
}

CharBlock::CharBlock(size_t size)
{
	capacity = size;
	occupied = 0;
	block = new char[size];
}

CharBlock::~CharBlock()
{
	delete [] block;
}
