#include <SerializeSimpleTypes.h>

#ifdef __EMSCRIPTEN__
void Serializer::write(const size_t& value, char buffer[])
{
	write((int64_t)value, buffer);
}
#endif

void Serializer::write(const int64_t& value, char buffer[])
{
	buffer[0] = value & 0xff;
	buffer[1] = (value >> 8) & 0xff;
	buffer[2] = (value >> 16) & 0xff;
	buffer[3] = (value >> 24) & 0xff;
	buffer[4] = (value >> 32) & 0xff;
	buffer[5] = (value >> 40) & 0xff;
	buffer[6] = (value >> 48) & 0xff;
	buffer[7] = (value >> 56) & 0xff;
}

void Serializer::write(const uint64_t& value, char buffer[])
{
	buffer[0] = value & 0xff;
	buffer[1] = (value >> 8) & 0xff;
	buffer[2] = (value >> 16) & 0xff;
	buffer[3] = (value >> 24) & 0xff;
	buffer[4] = (value >> 32) & 0xff;
	buffer[5] = (value >> 40) & 0xff;
	buffer[6] = (value >> 48) & 0xff;
	buffer[7] = (value >> 56) & 0xff;
}

void Serializer::write(const int32_t& value, char buffer[])
{
	buffer[0] = value & 0xff;
	buffer[1] = (value >> 8) & 0xff;
	buffer[2] = (value >> 16) & 0xff;
	buffer[3] = (value >> 24) & 0xff;
}

void Serializer::write(const uint32_t& value, char buffer[])
{
	buffer[0] = value & 0xff;
	buffer[1] = (value >> 8) & 0xff;
	buffer[2] = (value >> 16) & 0xff;
	buffer[3] = (value >> 24) & 0xff;
}

void Serializer::write(const int16_t& value, char buffer[])
{
	buffer[0] = value & 0xff;
	buffer[1] = (value >> 8) & 0xff;
}

void Serializer::write(const uint16_t& value, char buffer[])
{
	buffer[0] = value & 0xff;
	buffer[1] = (value >> 8) & 0xff;
}

void Serializer::write(const int8_t& value, char buffer[])
{
	buffer[0] = value & 0xff;
}

void Serializer::write(const bool& value, char buffer[])
{
	buffer[0] = value ? 1 : 0;
}

void Serializer::write(const float& value, char buffer[])
{
	//TODO endianess
	char* converted = (char*)& value;
	memcpy(buffer, converted, 4);
}

void Serializer::write(const std::string& value, char buffer[])
{
	write((int16_t)value.size(), buffer);
	memcpy(buffer+sizeof(int16_t), value.c_str(), value.size());
}

void Serializer::read(std::string& out, const char* value)
{
	int16_t length;
	read(length, value);
	out.assign(value + sizeof(int16_t), length);
}

#ifdef __EMSCRIPTEN__
void Serializer::read(size_t& out, const char* value)
{
	int64_t v;
	read(v, value);
	out = v;
}
#endif

void Serializer::read(int64_t& out, const char* value)
{
	out = (int64_t)(unsigned char)value[0];
	out += (int64_t)(unsigned char)value[1] << 8;
	out += (int64_t)(unsigned char)value[2] << 16;
	out += (int64_t)(unsigned char)value[3] << 24;
	out += (int64_t)(unsigned char)value[4] << 32;
	out += (int64_t)(unsigned char)value[5] << 40;
	out += (int64_t)(unsigned char)value[6] << 48;
	out += (int64_t)(unsigned char)value[7] << 56;
}

void Serializer::read(uint64_t& out, const char* value)
{
	out = (uint64_t)(unsigned char)value[0];
	out += (uint64_t)(unsigned char)value[1] << 8;
	out += (uint64_t)(unsigned char)value[2] << 16;
	out += (uint64_t)(unsigned char)value[3] << 24;
	out += (uint64_t)(unsigned char)value[4] << 32;
	out += (uint64_t)(unsigned char)value[5] << 40;
	out += (uint64_t)(unsigned char)value[6] << 48;
	out += (uint64_t)(unsigned char)value[7] << 56;
}

void Serializer::read(int32_t& out, const char* value)
{
	out = (unsigned char)value[0];
	out += (unsigned char)value[1] << 8;
	out += (unsigned char)value[2] << 16;
	out += (unsigned char)value[3] << 24;
}

void Serializer::read(uint32_t& out, const char* value)
{
	out = (unsigned char)value[0];
	out += (unsigned char)value[1] << 8;
	out += (unsigned char)value[2] << 16;
	out += (unsigned char)value[3] << 24;
}

void Serializer::read(int16_t& out, const char* value)
{
	out = (unsigned char)value[0];
	out += (unsigned char)value[1] << 8;
}

void Serializer::read(uint16_t& out, const char* value)
{
	out = (unsigned char)value[0];
	out += (unsigned char)value[1] << 8;
}

void Serializer::read(int8_t& out, const char* value)
{
	out = value[0];
}

void Serializer::read(bool& out, const char* value)
{
	out = value[0] != 0;
}

void Serializer::read(float& out, const char* value)
{
	//TODO endianess

	char *outFloat = (char*)& out;

	outFloat[0] = value[0];
	outFloat[1] = value[1];
	outFloat[2] = value[2];
	outFloat[3] = value[3];
}