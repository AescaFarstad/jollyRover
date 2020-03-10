#pragma once
#include <Serialization.h>
#include <json.hpp>

const std::string not_anonymous;

class JSONSerializer;
namespace Serialization
{
	template <typename T>
	void write(const JSONSerializer& object, T& serializer)
	{
		
	}
	
	template <typename T>
	void read(JSONSerializer& object, T& serializer)
	{
		
	}
}


class JSONSerializer
{
public:
	JSONSerializer();
	std::string toString();
		
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
		startArray(DEBUG_MODE ? fieldName : anonymous);
		for (int16_t i = 0; i < vec.size(); i++)
		{
			write(vec[i], fieldName == anonymous ? anonymous : not_anonymous);
		}
		endArray();
	}
	
	template <typename T>
	void write(const T& object, const std::string& fieldName = anonymous)
	{
		startObject(DEBUG_MODE ? fieldName : anonymous);
		Serialization::write(object, *this);
		endObject();
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
		//TODO	
	}
	
	template <typename T>
	void read(T& object, const std::string& fieldName = anonymous)
	{
		//TODO	
	}
private:
	nlohmann::json m_stream;
	std::vector<nlohmann::json*> m_stack;

	void startObject(std::string fieldName);
	void endObject();
	void startArray(std::string fieldName);
	void endArray();
	
};