#include <BinarySerializer.h>
#include <SerializeSimpleTypes.h>
#include <sstream>
#include <iomanip>
#include <cstring>

BinarySerializer::BinarySerializer()
{
	m_readingStream = std::make_unique<BinaryStream>(1024*8, BinaryStream::exponentialGrow);
	m_writingStream = static_cast<BinaryStream*>(m_readingStream.get());
}

BinarySerializer::BinarySerializer(const std::vector<char>& data)
{
	m_readingStream = std::make_unique<BinaryReader>(data);
	m_writingStream = nullptr;
}

BinarySerializer::BinarySerializer(const char* data, size_t size)
{
	m_readingStream = std::make_unique<BinaryReader>(data, size);
	m_writingStream = nullptr;
}


#define WRITE(type) 																\
void BinarySerializer::write(const type& value, const std::string& fieldName)		\
{																					\
	Serializer::write(value, m_writingStream->allocate(sizeof(value)));		\
	if (DEBUG_TRACE) std::cout << m_writingStream->loc() << " " << "write " << fieldName << "\t" << value << "\n";		\
}

#define READ(type) 														  			\
void BinarySerializer::read(type& out, const std::string& fieldName)				\
{																					\
	Serializer::read(out, m_readingStream->read(sizeof(out)));	\
	if (DEBUG_TRACE) std::cout << m_readingStream->loc() << " " << "read  " << fieldName << "\t" << out << "\n";					\
}

#define PEEK(type) 														  			\
void BinarySerializer::peek(type& out, const std::string& fieldName)				\
{																					\
	Serializer::read(out, m_readingStream->peek(sizeof(out)));						\
}

WRITE(int64_t)
WRITE(uint64_t)
WRITE(int32_t)
WRITE(uint32_t)
WRITE(int16_t)
WRITE(uint16_t)
WRITE(int8_t)
WRITE(bool)
WRITE(float)

READ(int64_t)
READ(uint64_t)
READ(int32_t)
READ(uint32_t)
READ(int16_t)
READ(uint16_t)
READ(int8_t)
READ(bool)
READ(float)

PEEK(int64_t)
PEEK(uint64_t)
PEEK(int32_t)
PEEK(uint32_t)
PEEK(int16_t)
PEEK(uint16_t)
PEEK(int8_t)
PEEK(bool)
PEEK(float)

void BinarySerializer::write(const std::string& value, const std::string& fieldName)
{
	Serializer::write(value, m_writingStream->allocate(Serializer::sizeOfString(value)));
	if (DEBUG_TRACE) std::cout << "write " << fieldName << "\t" << value << "\n";
}

void BinarySerializer::read(std::string& out, const std::string& fieldName)
{
	int16_t length;
	Serializer::read(length, m_readingStream->read(sizeof(length)));
	out.assign(m_readingStream->read(length), length);
	if (DEBUG_TRACE) std::cout << "read  " << fieldName << "\t" << out << "\n";
}

void BinarySerializer::write(const std::vector<char>& vec, const std::string& fieldName)
{
	if (DEBUG_TRACE) std::cout << "write " << fieldName << "\t" << "char assary of size "<<vec.size() << "\n";
	int32_t size = (int32_t)vec.size();
	write(size);
	auto target = m_writingStream->allocate(size);
	std::memcpy(target, vec.data(), size);
	
}

void BinarySerializer::read(std::vector<char>& vec, const std::string& fieldName)
{
	int32_t size;
	read(size);
	if (DEBUG_TRACE) std::cout << "read  " << fieldName << "\t" << "^char assary of size "<<size << "\n";
	vec.clear();
	if (size > 0)
	{
		vec.resize(size);	
		std::memcpy(vec.data(), m_readingStream->read(size), size);		
	}
}

std::string BinarySerializer::crc()
{
	return m_readingStream->crc();
}

std::string BinarySerializer::base16() const
{
	std::stringstream  stringstream;
	stringstream << std::hex;
	auto data = m_readingStream->readAll();
	for (size_t i = 0; i < data.size(); i++)
		stringstream << std::setfill('0') << std::setw(2) << (int)(unsigned char)data[i] << " ";
	return stringstream.str();
}

std::vector<char> BinarySerializer::dumpAll() const
{
	return m_readingStream->readAll();
}

size_t BinarySerializer::getLength() const
{
	return m_readingStream->getLength();
}

size_t BinarySerializer::getDataLeft() const
{
	return m_readingStream->getDataLeft();
}
/*
void BinarySerializer::assign(const std::vector<char>& data)
{
	m_stream.reset(data.size());
	auto array = m_stream.allocate(data.size());
	std::memcpy(array, &data[0], data.size());
}

void BinarySerializer::assign(const char* data, int32_t size)
{
	m_stream.reset(size);
	auto array = m_stream.allocate(size);
	std::memcpy(array, data, size);
}
*/
void BinarySerializer::resetCursors()
{
	m_readingStream->resetCursors();
}