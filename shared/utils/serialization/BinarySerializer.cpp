#include <BinarySerializer.h>
#include <SerializeSimpleTypes.h>
#include <sstream>
#include <iomanip>

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