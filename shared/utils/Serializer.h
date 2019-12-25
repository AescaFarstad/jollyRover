#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <base64_2.h>
#include <SerializationStream.h>
#include <iomanip>
#include <SerializeSimpleTypes.h>

namespace Serializer {

	void write(const int32_t& value, std::ostream& stream);
	void write(const uint32_t& value, std::ostream& stream);
	void write(const int16_t& value, std::ostream& stream);
	void write(const uint16_t& value, std::ostream& stream);
	void write(const int8_t& value, std::ostream& stream);
	void write(const bool& value, std::ostream& stream);
	void write(const float& value, std::ostream& stream);
	void write(const std::string& value, std::ostream& stream);

	void write(const int32_t& value, SerializationStream& stream);
	void write(const uint32_t& value, SerializationStream& stream);
	void write(const int16_t& value, SerializationStream& stream);
	void write(const uint16_t& value, SerializationStream& stream);
	void write(const int8_t& value, SerializationStream& stream);
	void write(const bool& value, SerializationStream& stream);
	void write(const float& value, SerializationStream& stream);
	void write(const std::string& value, SerializationStream& stream);


	template <typename T>
	void writeVector(const T& value, std::ostream& stream)
	{
		int16_t size = (int16_t)value.size();
		write(size, stream);
		for (int16_t i = 0; i < size; i++)
		{
			write(value[i], stream);
		}
	}
	template <typename T>
	void writeVector(const T& value, SerializationStream& stream)
	{
		int16_t size = (int16_t)value.size();
		write(size, stream);
		for (int16_t i = 0; i < size; i++)
		{
			write(value[i], stream);
		}
	}

	void read(int32_t& out, std::istream& stream);
	void read(uint32_t& out, std::istream& stream);
	void read(int16_t& out, std::istream& stream);
	void read(uint16_t& out, std::istream& stream);
	void read(int8_t& out, std::istream& stream);
	void read(bool& out, std::istream& stream);
	void read(float& out, std::istream& stream);
	void read(std::string& out, std::istream& stream);

	void read(int32_t& out, SerializationStream& stream);
	void read(uint32_t& out, SerializationStream& stream);
	void read(int16_t& out, SerializationStream& stream);
	void read(uint16_t& out, SerializationStream& stream);
	void read(int8_t& out, SerializationStream& stream);
	void read(bool& out, SerializationStream& stream);
	void read(float& out, SerializationStream& stream);
	void read(std::string& out, SerializationStream& stream);

	template <typename T>
	void readVector(T& out, std::istream& stream)
	{
		int16_t size;
		read(size, stream);
		out.clear();
		for (int16_t i = 0; i < size; i++)
		{
			out.emplace_back();
			read(out[i], stream);
		}
	}
	template <typename T>
		void readVector(T& out, SerializationStream& stream)
	{
		int16_t size;
		read(size, stream);
		out.clear();
		for (int16_t i = 0; i < size; i++)
		{
			out.emplace_back();
			read(out[i], stream);
		}
	}
	
	//Debug Utils
	std::string toHex(const char* source, size_t size);
	std::string toDec(const char* source, size_t size);
	void fromHex(std::string source, std::stringstream& stream);
	template <typename T>
	void swapBytes(T* target)
	{
		T tmp = *target;
		unsigned char* ptmp = (unsigned char*)&tmp;
		unsigned char* ttmp = (unsigned char*)target;
		for(size_t i = 0; i < sizeof(T); i++)
			ttmp[i] = ptmp[sizeof(T) - 1 - i];
	}
}