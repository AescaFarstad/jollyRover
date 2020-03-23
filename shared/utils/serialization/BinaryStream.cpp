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

void BinaryStream::resetCursors()
{
	m_readCursor.block = &m_blocks[0];
	m_readCursor.position = 0;
	m_writeCursor.block = &m_blocks[0];
	m_writeCursor.position = 0;
}

std::vector<char> BinaryStream::readAll()
{
	std::vector<char> result;
	auto length = getLength();
	result.resize(length);
	
	size_t cursor = 0;
	for(auto& block : m_blocks)
	{
		std::memcpy(&result[cursor], block.array, block.occupied);
		cursor += block.occupied;
	}
	return result;
}

size_t BinaryStream::getLength()
{
	size_t result = 0;
	for(auto& block : m_blocks)
		result += block.occupied;
	return result;
}
	

std::string BinaryStream::crc()
{
	CRC32  digestCrc32;
	auto data = readAll();
	digestCrc32.add(&data[0], data.size());
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
	//printf("Block()\n");
}

BinaryStreamInternal::Block::~Block()
{
	//printf("~Block\n");
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