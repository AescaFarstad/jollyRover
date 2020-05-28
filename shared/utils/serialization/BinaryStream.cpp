#include <BinaryStream.h>
#include <Global.h>
#include <crc32.h>
#include <cstring>

BinaryStream::BinaryStream()
{
	m_growStrategy = noGrow;
	m_writeCursor.block = nullptr;
	m_readCursor.block = nullptr;
}

BinaryStream::BinaryStream(size_t startingSize, GrowStrategy growStrategy)
{
	init(startingSize, growStrategy);
}

void BinaryStream::init(size_t startingSize, GrowStrategy growStrategy)
{
	m_blocks.clear();
	m_blocks.emplace_back(startingSize);
	
	m_growStrategy = growStrategy;
	
	resetCursors();
}

void BinaryStream::reset(size_t startingSize)
{
	init(startingSize, m_growStrategy);
}


char* BinaryStream::allocate(size_t size)
{
	if (size > (m_writeCursor.block->size - m_writeCursor.block->occupied))
	{
		auto newSize = m_growStrategy(m_writeCursor.block->size, size);
		auto& newBlock = m_blocks.emplace_back(newSize);
		
		m_writeCursor.block = &newBlock;
		m_writeCursor.position = 0;
		m_readCursor.block = &m_blocks[0];
		m_readCursor.position = 0;
	}
	
	m_writeCursor.position += size;
	m_writeCursor.block->occupied += size;
	
	return &m_writeCursor.block->array[m_writeCursor.position - size];
}

const char* BinaryStream::read(size_t size)
{
	if (m_readCursor.block == nullptr || m_readCursor.block->occupied < m_readCursor.position + size)
		THROW_FATAL_ERROR("wrong stream read.");
	
	auto tmpCursor = m_readCursor;
	m_readCursor.position += size;
	if (m_readCursor.position == m_readCursor.block->occupied)
	{
		if (m_readCursor.block == &m_blocks.back())
			m_readCursor.block = nullptr;
		else
			m_readCursor.block++;
		
		m_readCursor.position = 0;		
	}
	return &tmpCursor.block->array[tmpCursor.position];	
}

const char* BinaryStream::peek(size_t size)
{
	if (m_readCursor.block == nullptr || m_readCursor.block->occupied < m_readCursor.position + size)
		THROW_FATAL_ERROR("wrong stream read.");
		
	return &m_readCursor.block->array[m_readCursor.position];	
}

void BinaryStream::resetCursors()
{
	m_readCursor.block = &m_blocks[0];
	m_readCursor.position = 0;
	m_writeCursor.block = &m_blocks[0];
	m_writeCursor.position = 0;
}

std::vector<char> BinaryStream::readAll() const
{
	std::vector<char> result;
	auto length = getLength();
	result.resize(length);
	
	size_t cursor = 0;
	for(auto& block : m_blocks)
	{
		std::memcpy(result.data() + cursor, block.array, block.occupied);
		cursor += block.occupied;
	}
	return result;
}

size_t BinaryStream::getLength() const
{
	size_t result = 0;
	for(auto& block : m_blocks)
		result += block.occupied;
	return result;
}

size_t BinaryStream::getDataLeft() const
{
	size_t result = 0;
	bool startCounting = false;
	for(auto& block : m_blocks)
	{
		if (&block == m_readCursor.block)
		{
			result += block.occupied - m_readCursor.position;
			startCounting = true;
		}
		else if (startCounting)
		{
			result += block.occupied;
		}			
	}
	return result;
}


size_t BinaryStream::loc()
{
	size_t result = 0;
	for(auto& block : m_blocks)
	{
		if (&block != m_writeCursor.block)
			result += block.occupied;
		else{
			result+= m_writeCursor.position;
			break;}
	}
	
	return result;
}

std::string BinaryStream::crc()
{
	CRC32  digestCrc32;
	auto data = readAll();
	digestCrc32.add(data.data(), data.size());
	return digestCrc32.getHash();
}

std::size_t BinaryStream::exponentialGrow(size_t previousSize, size_t  allocation)
{
	return std::max((size_t)(previousSize * growExponent), allocation);
}

std::size_t BinaryStream::noGrow(size_t previousSize, size_t  allocation)
{
	THROW_FATAL_ERROR("Not supposed to grow.");
}

BinaryStreamInternal::Block::Block(size_t size)
{
	occupied = 0;
	array = new char[size];
	this->size = size;
}

BinaryStreamInternal::Block::~Block()
{
	if (array != nullptr)
		delete[] array;
}

BinaryStreamInternal::Block::Block(Block&& that)
{
	array = that.array;
	size = that.size;
	occupied = that.occupied;
	
	that.array = nullptr;
}