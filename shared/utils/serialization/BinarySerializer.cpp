#include <BinarySerializer.h>
#include <SerializeSimpleTypes.h>
#include <sstream>
#include <iomanip>
#include <cstring>

BinarySerializer::BinarySerializer()
{
	m_stream.init(1024*8, BinaryStream::exponentialGrow);
}

#define WRITE(type) 																\
void BinarySerializer::write(const type& value, const std::string& fieldName)		\
{																					\
	Serializer::write(value, m_stream.allocate(sizeof(value)));						\
}

#define READ(type) 														  			\
void BinarySerializer::read(type& out, const std::string& fieldName)				\
{																					\
	Serializer::read(out, m_stream.read(sizeof(out)));								\
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

void BinarySerializer::write(const std::string& value, const std::string& fieldName)
{
	Serializer::write(value, m_stream.allocate(Serializer::sizeOfString(value)));
}

void BinarySerializer::read(std::string& out, const std::string& fieldName)
{
	int16_t length;
	Serializer::read(length, m_stream.read(sizeof(length)));
	out.assign(m_stream.read(length), length);
}

void BinarySerializer::write(const std::vector<char>& vec, const std::string& fieldName)
{
	int32_t size = (int32_t)vec.size();
	write(size);
	auto target = m_stream.allocate(size);
	std::memcpy(target, &vec[0], size);
}

void BinarySerializer::read(std::vector<char>& vec, const std::string& fieldName)
{
	int32_t size;
	read(size);
	vec.clear();
	vec.resize(size);
	std::memcpy(&vec[0], m_stream.read(size), size);
}

std::string BinarySerializer::crc()
{
	return m_stream.crc();
}

std::string BinarySerializer::base16()
{
	std::stringstream  stringstream;
	stringstream << std::hex;
	auto data = m_stream.readAll();
	for (size_t i = 0; i < data.size(); i++)
		stringstream << std::setfill('0') << std::setw(2) << (int)(unsigned char)data[i] << " ";
	return stringstream.str();
}

std::vector<char> BinarySerializer::dumpAll()
{
	return m_stream.readAll();
}

void BinarySerializer::assign(std::vector<char>& data)
{
	m_stream.reset(data.size());
	auto array = m_stream.allocate(data.size());
	std::memcpy(array, &data[0], data.size());
}

void BinarySerializer::assign(char* data, int32_t size)
{
	m_stream.reset(size);
	auto array = m_stream.allocate(size);
	std::memcpy(array, data, size);
}

void BinarySerializer::resetCursors()
{
	m_stream.resetCursors();
}