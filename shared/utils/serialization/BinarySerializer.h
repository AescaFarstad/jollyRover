#pragma once
#include <BinaryStream.h>
#include <Serialization.h>
#include <Global.h>
#include <iostream>

class BinarySerializer;
namespace Serialization
{
	template <typename T>
	void write(const BinarySerializer& object, T& serializer)
	{
		THROW_FATAL_ERROR("Not impelmented write(const BinarySerializer");
	}
	
	template <typename T>
	void read(BinarySerializer& object, T& serializer)	
	{
		THROW_FATAL_ERROR("Not impelmented read(BinarySerializer");
	}
}


class BinarySerializer
{
public:
	BinarySerializer();
	BinarySerializer(const std::vector<char>& data);
	BinarySerializer(const char* data, size_t size);
	
	std::string crc();
	std::string base16() const;
	std::vector<char> dumpAll() const;
	void resetCursors();
	size_t getLength() const;
	size_t getDataLeft() const;
	bool DEBUG_TRACE = false;
	
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
		if (DEBUG_TRACE) std::cout << "write " << fieldName << "\t" << "vector of size " << vec.size() << "\n";
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
		if (DEBUG_TRACE) std::cout << "write " << fieldName<<"\n";
		Serialization::write(object, *this);
	}
	
	template <typename T>
	void writeArray(const T& object, std::size_t size, const std::string& fieldName = anonymous)
	{
		if (DEBUG_TRACE) std::cout << "write " << fieldName << "\t" << "array of size " << size << "\n";
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
	
	void peek(int64_t& out, const std::string& fieldName = anonymous);
	void peek(uint64_t& out, const std::string& fieldName = anonymous);
	void peek(int32_t& out, const std::string& fieldName = anonymous);
	void peek(uint32_t& out, const std::string& fieldName = anonymous);
	void peek(int16_t& out, const std::string& fieldName = anonymous);
	void peek(uint16_t& out, const std::string& fieldName = anonymous);
	void peek(int8_t& out, const std::string& fieldName = anonymous);
	void peek(bool& out, const std::string& fieldName = anonymous);
	void peek(float& out, const std::string& fieldName = anonymous);
	
	
	template <typename T>
	void read(std::vector<T>& vec, const std::string& fieldName = anonymous)
	{
		int16_t size;
		read(size);
		if (DEBUG_TRACE) std::cout << "read  " << fieldName << "\t" << "^vector of size " << size << "\n";
		vec.clear();
		for (int16_t i = 0; i < size; i++)
			read(vec.emplace_back());
	}
	
	template <typename T>
	void read(T& object, const std::string& fieldName = anonymous)
	{
		if (DEBUG_TRACE) std::cout << "read  " << fieldName<<"\n";
		Serialization::read(object, *this);
	}	
	
	template <typename T>
	void readArray(T& object, size_t size, const std::string& fieldName = anonymous)
	{
		if (DEBUG_TRACE) std::cout << "read  " << fieldName << "\t" << "array of size " << size << "\n";
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
		return s.getLength();
	}
	
private:	
	BinaryStream* m_writingStream; //nuullptr or same as m_readingStream, no need to delete
	std::unique_ptr<BinaryReader> m_readingStream;
};