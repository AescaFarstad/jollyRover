#include <SerializationStream.h>
#include <assert.h>
#include <cmath>

SerializationStream::SerializationStream(StreamGrower* grower)
{
	this->m_grower = grower;
	m_grower->grow(this, 0);
}

SerializationStream::SerializationStream()
{
}

SerializationStream::~SerializationStream()
{
	//printf("~SerializationStream()\n");
	delete m_grower;
	//printf("delete grower\n");
	for (size_t i = 0; i < m_blocks.size(); i++)
	{
		delete m_blocks[i];
	}
}

const char* SerializationStream::read(size_t length)
{
	char* result = &m_cursor.block->block[m_cursor.index];
	seekRelative(length);
	return result;
}

char * SerializationStream::readAll()
{
	char* result = new char[m_totalLength];
	size_t m_cursor = 0;
	for (size_t i = 0; i < m_blocks.size(); i++)
	{
		memcpy(result + m_cursor, m_blocks[i]->block, m_blocks[i]->occupied);
		m_cursor += m_blocks[i]->occupied;
	}
	return result;
}

char * SerializationStream::c_str()
{
	char* result = new char[m_totalLength + 1];
	size_t m_cursor = 0;
	for (size_t i = 0; i < m_blocks.size(); i++)
	{
		memcpy(result + m_cursor, m_blocks[i]->block, m_blocks[i]->occupied);
		m_cursor += m_blocks[i]->occupied;
	}
	result[m_totalLength] = '\0';
	return result;
}

char* SerializationStream::readAllAsHex()
{
	const char* code = "0123456789abcdef";
	char* result = new char[m_totalLength * 3];
	size_t m_cursor = 0;
	for (size_t i = 0; i < m_blocks.size(); i++)
	{
		for (size_t j = 0; j < m_blocks[i]->occupied; j++)
		{
			result[m_cursor * 3] = code[(unsigned char)m_blocks[i]->block[j] / 16];
			result[m_cursor * 3 + 1] = code[(unsigned char)m_blocks[i]->block[j] % 16];
			result[m_cursor * 3 + 2] = ' ';
			m_cursor++;
		}
	}
	return result;
}

char * SerializationStream::readAllAsHex_c()
{
	return nullTerminate(readAllAsHex(), m_totalLength * 3);
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
	return &m_cursor.block->block[m_cursor.index];
}

void SerializationStream::seekAbsolute(size_t position)
{
	size_t passed = 0;
	for (size_t i = 0; i < m_blocks.size(); i++)
	{
		if (passed + m_blocks[i]->occupied > position)
		{
			m_cursor.block = m_blocks[i];
			m_cursor.index = position - passed;
			return;
		}
		else
		{
			passed += m_blocks[i]->occupied;
		}

	}
	if (passed == position)
	{
		m_cursor.block = m_blocks.back();
		m_cursor.index = m_cursor.block->occupied;
		return;
	}
	printf(" ");
	THROW_FATAL_ERROR("seekAbsolute is out if bounds (" + std::to_string(position) + ")");
}

void SerializationStream::seekRelative(size_t position)
{
	if (position + m_cursor.index > 0 && position + m_cursor.index <= m_cursor.block->occupied)
	{
		m_cursor.index = position + m_cursor.index;
		if (m_cursor.index == m_cursor.block->occupied && m_cursor.block != m_blocks.back())
		{
			m_cursor.index = 0;
			m_cursor.block = getNextBlock(m_cursor.block);
		}
	}
	else
	{
		seekAbsolute(getAbsolutePosition() + position);
	}
}

void SerializationStream::seekEnd()
{
	m_cursor.block = m_blocks.back();
	m_cursor.index = m_cursor.block->occupied;
}

size_t SerializationStream::getLength()
{
	return m_totalLength;
}

void SerializationStream::writeSimple(const char* data, size_t length)
{
	for (size_t i = 0; i < length; i++)
	{
		m_cursor.block->block[m_cursor.index + i] = data[i];
	}
	m_cursor.index += length;
}

//solve for total length
void SerializationStream::write(const char* data, size_t length)
{
	if (m_cursor.block == m_blocks.back())
	{
		//we're at the end of the last block. grow if nesscessary and write
		if (m_cursor.index + length >= m_cursor.block->capacity)//todo if fits exactly - writesimple and then grow
		{
			m_totalLength -= m_cursor.block->occupied - m_cursor.index;
			m_cursor.block->occupied = m_cursor.index;
			m_grower->grow(this, length);
		}
		writeSimple(data, length);
		m_cursor.block->occupied = m_cursor.index;
		m_totalLength += length;
	}
	else
	{
		//overwrite within the boundaries of one block
		if (m_cursor.index + length < m_cursor.block->occupied)
		{
			writeSimple(data, length);
		}
		else
		{
			bool dataFits = m_cursor.index + length < m_cursor.block->capacity;
			size_t cutFromBeginningAmount = length - (m_cursor.block->occupied - m_cursor.index);

			//write to the end of one block and possibly delete some data from the next block
			if (dataFits)
			{
				for (size_t i = 0; i < length; i++)
				{
					m_cursor.block->block[m_cursor.index + i] = data[i];
				}
				m_cursor.block->occupied = m_cursor.index + length;

				m_cursor.block = getNextBlock(m_cursor.block);
				m_cursor.index = 0;

				if (cutFromBeginningAmount > m_cursor.block->occupied)
				{
					m_totalLength += cutFromBeginningAmount - m_cursor.block->occupied;
					cutFromBeginningAmount = m_cursor.block->occupied;
				}

				if (cutFromBeginningAmount > 0)
				{
					memmove(m_cursor.block->block, m_cursor.block->block + cutFromBeginningAmount, m_cursor.block->occupied - cutFromBeginningAmount);
					m_cursor.block->occupied -= cutFromBeginningAmount;
				}
			}
			//delete data from the current block and rebuild the next block entirely
			else
			{
				m_cursor.block->occupied = m_cursor.index;
				CharBlock* nextBlock = getNextBlock(m_cursor.block);
				CharBlock* newBlock = new CharBlock(length + nextBlock->occupied);

				if (cutFromBeginningAmount > m_cursor.block->occupied)
				{
					m_totalLength += cutFromBeginningAmount - m_cursor.block->occupied;
					cutFromBeginningAmount = m_cursor.block->occupied;
				}

				memcpy(newBlock->block, data, length);
				memcpy(newBlock->block + length, nextBlock->block + cutFromBeginningAmount, nextBlock->occupied - cutFromBeginningAmount);
				m_cursor.block = newBlock;
				m_cursor.index = length;

				for (size_t i = 0; i < m_blocks.size(); i++)
				{
					if (m_blocks[i] == nextBlock)
					{
						m_blocks[i] = newBlock;
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
	m_blocks.push_back(newBlock);
	m_cursor.block = newBlock;
	m_cursor.index = 0;
}

size_t SerializationStream::getAbsolutePosition()
{
	size_t passed = 0;
	for (size_t i = 0; i < m_blocks.size(); i++)
	{
		if (m_blocks[i] != m_cursor.block)
		{
			passed += m_blocks[i]->occupied;
		}
		else
		{
			passed += m_cursor.index;
			break;
		}
	}
	return passed;
}

CharBlock* SerializationStream::getNextBlock(CharBlock* currentBlock)
{
	for (size_t i = 0; i < m_blocks.size(); i++)
	{
		if (m_blocks[i] == currentBlock)
			return m_blocks[i + 1];
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
	return stream->m_blocks.size();
}

CharBlock * StreamGrower::getLastBlock(SerializationStream* stream)
{
	return stream->m_blocks.back();
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
