#include <BinaryReader.h>
#include <Global.h>
#include <crc32.h>

BinaryReader::BinaryReader()
{
	m_data = nullptr;
	m_readCursor = 0;
}

BinaryReader::BinaryReader(const char* data, size_t size) : BinaryReader()
{
	m_data = data;
	m_size = size;
}

BinaryReader::BinaryReader(const std::vector<char>& data) : BinaryReader()
{
	m_data = data.data();	
	m_size = data.size();
}
size_t BinaryReader::loc()
{
	return m_readCursor;
}

const char* BinaryReader::read(size_t size)
{
	auto result = peek(size);
	m_readCursor += size;
	
	return result;
}

const char* BinaryReader::peek(size_t size)
{
	if (m_data == nullptr || m_readCursor + size > m_size)
		THROW_FATAL_ERROR("wrong stream read.");
	
	return m_data + m_readCursor;
}

std::vector<char> BinaryReader::readAll() const
{
	std::vector<char> result;
	result.resize(m_size);
	std::memcpy(result.data(), m_data, m_size);
	return result;
}

void BinaryReader::resetCursors()
{
	m_readCursor = 0;
}

size_t BinaryReader::getLength() const
{
	return m_size;
}

size_t BinaryReader::getDataLeft() const
{
	return m_size - m_readCursor;
}

std::string BinaryReader::crc()
{
	CRC32  digestCrc32;
	digestCrc32.add(m_data, m_size);
	return digestCrc32.getHash();
}