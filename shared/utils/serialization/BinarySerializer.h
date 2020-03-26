#pragma once
#include <BinaryStream.h>
#include <Serialization.h>

class BinarySerializer;
namespace Serialization
{
	template <typename T>
	void write(const BinarySerializer& object, T& serializer)
	{
		
	}
	
	template <typename T>
	void read(BinarySerializer& object, T& serializer)
	{
		
	}
}


class BinarySerializer
{
public:
	BinarySerializer();
	
	std::string crc();
	std::string base16();
	std::vector<char> dumpAll();
	void assign(std::vector<char>& data);
	void assign(char* data, int32_t size);
	void resetCursors();
	
	void write(const int64_t& value, const std::string& fieldName = anonymous);
	void write(const uint64_t& vavalue, const std::string& fieldName = anonymous);
	void write(const int32_t& value, const std::string& fieldName = anonymous);
	void write(const uint32_t& value, const std::string& fieldName = anonymous);
	void write(const int16_t& value, const std::string& fieldName = anonymous);
	void write(const uint16_t& value, const std::string& fieldName = anonymous);
	void write(const int8_t& value, const std::string& fieldName = anonymous);
	void write(const bool& value, const std::string& fieldName = anonymous);
	void write(const float& value, const std::string& fieldName = anonymous);
	void write(const std::string& value, const std::string& fieldName = anonymous);
	void write(const std::vector<char>& vec, const std::string& fieldName = anonymous);
	
	template <typename T>
	void write(const std::vector<T>& vec, const std::string& fieldName = anonymous)
	{
		int16_t size = (int16_t)vec.size();
		write(size);
		for (int16_t i = 0; i < size; i++)
		{
			write(vec[i]);
		}
	}
	
	template <typename T>
	void write(const T& object, const std::string& fieldName = anonymous)
	{
		Serialization::write(object, *this);
	}
	
	template <typename T>
	void writeArray(const T& object, std::size_t size, const std::string& fieldName = anonymous)
	{
		for (size_t i = 0; i < size; i++)
			write(object[i]);
	}
	
	void read(int64_t& out, const std::string& fieldName = anonymous);
	void read(uint64_t& out, const std::string& fieldName = anonymous);
	void read(int32_t& out, const std::string& fieldName = anonymous);
	void read(uint32_t& out, const std::string& fieldName = anonymous);
	void read(int16_t& out, const std::string& fieldName = anonymous);
	void read(uint16_t& out, const std::string& fieldName = anonymous);
	void read(int8_t& out, const std::string& fieldName = anonymous);
	void read(bool& out, const std::string& fieldName = anonymous);
	void read(float& out, const std::string& fieldName = anonymous);
	void read(std::string& out, const std::string& fieldName = anonymous);
	void read(std::vector<char>& vec, const std::string& fieldName = anonymous);
	
	
	template <typename T>
	void read(std::vector<T>& vec, const std::string& fieldName = anonymous)
	{
		int16_t size;
		read(size);
		vec.clear();
		for (int16_t i = 0; i < size; i++)
			read(vec.emplace_back());
	}
	
	template <typename T>
	void read(T& object, const std::string& fieldName = anonymous)
	{
		Serialization::read(object, *this);
	}	
	
	template <typename T>
	void readArray(T& object, size_t size, const std::string& fieldName = anonymous)
	{
		for (size_t i = 0; i < size; i++)
			read(object[i]);
	}
	
	template <typename T>
	static T copyThroughSerialization(const T& object)
	{
		T result;
		
		BinarySerializer s;
		s.write(object);		
		s.read(*result);
		
		return result;
	}
	
	template <typename T>
	static void copyThroughSerialization(const T& from, T&to)
	{
		BinarySerializer s;
		s.write(from);		
		s.read(to);
	}
	
	template <typename T>
	static std::string crc(const T& object)
	{
		BinarySerializer s;
		s.write(object);
		return s.crc();
	}
	
	template <typename T>
	static size_t size(const T& object)
	{
		BinarySerializer s;
		s.write(object);
		return s.m_stream.getLength();
	}
	
private:	
	BinaryStream m_stream;
};
