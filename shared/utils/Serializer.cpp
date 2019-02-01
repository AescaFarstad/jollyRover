#include <Serializer.h>

void Serializer::write(const int32_t& value, std::ostream& stream)
{
	char out[4];
	write(value, out);
	stream.write(out, 4);
}

void Serializer::write(const int32_t& value, char buffer[])
{
	buffer[0] = value & 0xff;
	buffer[1] = (value >> 8) & 0xff;
	buffer[2] = (value >> 16) & 0xff;
	buffer[3] = (value >> 24) & 0xff;
}

void Serializer::read(int32_t& out, std::istream& stream)
{
	char value[4];
	stream.read(value, 4);
	read(out, value);
}

void Serializer::write(const uint32_t& value, std::ostream& stream)
{
	char out[4];
	write(value, out);
	stream.write(out, 4);
}

void Serializer::write(const uint32_t& value, char buffer[])
{
	buffer[0] = value & 0xff;
	buffer[1] = (value >> 8) & 0xff;
	buffer[2] = (value >> 16) & 0xff;
	buffer[3] = (value >> 24) & 0xff;
}

void Serializer::read(uint32_t& out, std::istream& stream)
{
	char value[4];
	stream.read(value, 4);
	read(out, value);
}

void Serializer::write(const int16_t& value, std::ostream& stream)
{
	char out[2];
	write(value, out);
	stream.write(out, 2);
}

void Serializer::write(const int16_t& value, char buffer[])
{
	buffer[0] = value & 0xff;
	buffer[1] = (value >> 8) & 0xff;
}


void Serializer::read(int16_t& out, std::istream& stream)
{
	char value[2];
	stream.read(value, 2);
	read(out, value);
}

void Serializer::write(const uint16_t& value, std::ostream& stream)
{
	char out[2];
	write(value, out);
	stream.write(out, 2);
}

void Serializer::write(const uint16_t& value, char buffer[])
{
	buffer[0] = value & 0xff;
	buffer[1] = (value >> 8) & 0xff;
}


void Serializer::read(uint16_t& out, std::istream& stream)
{
	char value[2];
	stream.read(value, 2);
	read(out, value);
}


void Serializer::write(const int8_t& value, std::ostream& stream)
{
	char out[1];
	write(value, out);
	stream.write(out, 1);
}

void Serializer::write(const int8_t& value, char buffer[])
{
	buffer[0] = value & 0xff;
}

void Serializer::read(int8_t& out, std::istream& stream)
{
	char value[1];
	stream.read(value, 1);
	read(out, value);
}

void Serializer::write(const bool& value, std::ostream& stream)
{
	char out[1];
	write(value, out);
	stream.write(out, 1);
}

void Serializer::write(const bool& value, char buffer[])
{
	buffer[0] = value ? 1 : 0;
}

void Serializer::read(bool& out, std::istream& stream)
{
	char value[1];
	stream.read(value, 1);
	read(out, value);
}

void Serializer::write(const float& value, std::ostream& stream)
{
	//TODO endianess
	char* converted = (char*)& value;
	stream.write(converted, sizeof(value));
}

void Serializer::write(const float& value, char buffer[])
{
	//TODO endianess
	char* converted = (char*)& value;
	memcpy(buffer, converted, 4);
}

void Serializer::read(float& out, std::istream& stream)
{
	//TODO endianess
	char value[sizeof(out)];
	stream.read(value, sizeof(out));
	read(out, value);
}

void Serializer::write(const std::string& value, std::ostream& stream)
{
	write((int16_t)value.size(), stream);
	stream.write(value.c_str(), value.size());
}
void Serializer::write(const std::string& value, SerializationStream& stream)
{
	write((int16_t)value.size(), stream);
	stream.write(value.c_str(), value.size());
}
void Serializer::write(const std::string& value, char buffer[])
{
	write((int16_t)value.size(), buffer);
	memcpy(buffer+sizeof(int16_t), value.c_str(), value.size());
}

void Serializer::read(std::string& out, std::istream& stream)
{
	int16_t length;
	read(length, stream);
	char* value = new char[length];
	stream.read(value, length);
	out.assign(value, length);
	delete[] value;
}
void Serializer::read(std::string& out, SerializationStream& stream)
{
	int16_t length;
	read(length, stream);
	const char* value = stream.read(length);
	out.assign(value, length);
}
void Serializer::read(std::string& out, const char* value)
{
	int16_t length;
	read(length, value);
	out.assign(value + sizeof(int16_t), length);
}

std::string Serializer::toHex(const char* source, size_t size)
{
	std::stringstream  stream;
	stream << std::hex;
	for (size_t i = 0; i < size; i++)
		stream << std::setfill('0') << std::setw(2) << (int)(unsigned char)source[i] << " ";
	return stream.str();
}

std::string Serializer::toDec(const char* source, size_t size)
{
	std::stringstream  stream;
	stream << std::dec;
	for (size_t i = 0; i < size; i++)
		stream << std::setfill(' ') << std::setw(3) << (int)(unsigned char)source[i] << " ";
	return stream.str();
}

void Serializer::fromHex(std::string source, std::stringstream& stream)
{
	//TODO
	/*
	stream.seekp(0);
	std::string result = base64_decode(source);
	const char* cstr = result.c_str();
	unsigned int size = result.size();
	char* cstr2 = new char[size];
	for (unsigned int i = 0; i < size; i++)
	{
		cstr2[i] = cstr[i];
		if (cstr2[i] == '\0')
			cstr2[i] = '0';
	}
	stream.write(cstr2, size);
	stream.seekg(0);

	char* cstr9 = new char[size];
	stream.read(cstr9, size);
	unsigned int size9 = result.size();

	//std::string oo = stream.read(cstr9, size);

	char* cstr10 = new char[size9];
	for (unsigned int i = 0; i < size9; i++)
	{
		cstr10[i] = cstr9[i];
		if (cstr10[i] == '\0')
			cstr10[i] = '0';
	}
	std::cout << "9-10:" << cstr10 << ".\n";

	char trgtr[25];

	for (unsigned int i = 0; i < 25; i++)
	{
		if (size > i)
			trgtr[i] = cstr2[i];
		else
			trgtr[i] = '_';
		if (trgtr[i] == '\0')
			trgtr[i] = '0';
	}
	trgtr[24] = '\0';
	std::cout << "25:" << trgtr <<".\n";
	*/
}

/*
std::string base64_encode(unsigned char const*, unsigned int len);
std::string base64_decode(std::string const& s);*/

//---------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------

void Serializer::write(const int32_t& value, SerializationStream& stream)
{
	char out[4];
	write(value, out);
	stream.write(out, 4);
}

void Serializer::read(int32_t& out, SerializationStream& stream)
{
	const char* value = stream.read(4);
	read(out, value);
}

void Serializer::write(const uint32_t& value, SerializationStream& stream)
{
	char out[4];
	write(value, out);
	stream.write(out, 4);
}

void Serializer::read(uint32_t& out, SerializationStream& stream)
{
	const char* value = stream.read(4);
	read(out, value);
}

void Serializer::write(const int16_t& value, SerializationStream& stream)
{
	char out[2];
	write(value, out);
	stream.write(out, 2);
}


void Serializer::read(int16_t& out, SerializationStream& stream)
{
	const char* value = stream.read(2);
	read(out, value);
}

void Serializer::write(const uint16_t& value, SerializationStream& stream)
{
	char out[2];
	write(value, out);
	stream.write(out, 2);
}


void Serializer::read(uint16_t& out, SerializationStream& stream)
{
	const char* value = stream.read(2);
	read(out, value);
}


void Serializer::write(const int8_t& value, SerializationStream& stream)
{
	char out[1];
	write(value, out);
	stream.write(out, 1);
}

void Serializer::read(int8_t& out, SerializationStream& stream)
{
	const char* value = stream.read(1);
	read(out, value);
}

void Serializer::write(const bool& value, SerializationStream& stream)
{
	char out[1];
	write(value, out);
	stream.write(out, 1);
}

void Serializer::read(bool& out, SerializationStream& stream)
{
	const char* value = stream.read(1);
	read(out, value);
}

void Serializer::write(const float& value, SerializationStream& stream)
{
	//TODO endianess
	char* converted = (char*)& value;
	stream.write(converted, sizeof(value));
}

void Serializer::read(float& out, SerializationStream& stream)
{
	//TODO endianess
	const char* value = stream.read(sizeof(out));
	read(out, value);
}

//---------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------

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