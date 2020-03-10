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
	static std::unique_ptr<T> copyThroughSerialization(T& object)
	{
		auto result = std::make_unique<T>();
		
		BinarySerializer s;
		s.write(object, s);		
		s.read(*result, s);
		
		return result;
	}
	
private:	
	BinaryStream m_stream;
};
