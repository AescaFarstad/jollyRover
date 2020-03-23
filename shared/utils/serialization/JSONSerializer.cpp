#include <JSONSerializer.h>
#include <base64_2.h>

using json=nlohmann::json;

JSONSerializer::JSONSerializer()
{
	m_stack.push_back(&m_stream);
}

std::string JSONSerializer::toString()
{
	return m_stream.dump(1);
}

#define WRITE(type) 															\
void JSONSerializer::write(const type& value, const std::string& fieldName)		\
{																				\
	if (&fieldName == &anonymous || m_stack.back()->is_array())					\
		*m_stack.back() += value;												\
	else																		\
		(*m_stack.back())[fieldName] = value;									\
}

#define READ(type) 																\
void JSONSerializer::read(type& value, const std::string& fieldName)			\
{																				\
																				\
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
WRITE(std::string)

READ(int64_t)
READ(uint64_t)
READ(int32_t)
READ(uint32_t)
READ(int16_t)
READ(uint16_t)
READ(int8_t)
READ(bool)
READ(float)
READ(std::string)

void JSONSerializer::startObject(std::string fieldName)
{
	if (m_stack.back()->is_null())
	{
		if (&fieldName != &anonymous)
			*m_stack.back() = json::object();
		else
			*m_stack.back() = json::array();
	}
	else
	{
		if (&fieldName != &anonymous && m_stack.back()->is_object())
		{
			*m_stack.back() += {fieldName, json::object()};
			m_stack.push_back(&m_stack.back()->at(fieldName));
		}
		else if (m_stack.back()->is_array() && &fieldName != &anonymous)
		{
			*m_stack.back() += json::object();
			m_stack.push_back(&m_stack.back()->back());
		}
		else
		{
			*m_stack.back() += json::array();
			m_stack.push_back(&m_stack.back()->back());
		}
	}	
}

void JSONSerializer::endObject()
{
	m_stack.pop_back();
}

void JSONSerializer::startArray(std::string fieldName)
{
	if (m_stack.back()->is_null())
	{
		if (&fieldName != &anonymous)
			*m_stack.back() = json::object();
		else
			*m_stack.back() = json::array();
	}
	else
	{
		if (&fieldName != &anonymous && m_stack.back()->is_object())
		{
			*m_stack.back() += {fieldName, json::array()};
			m_stack.push_back(&m_stack.back()->at(fieldName));
		}
		else
		{
			*m_stack.back() += json::array();
			m_stack.push_back(&m_stack.back()->back());
		}
	}
	
}

void JSONSerializer::endArray()
{
	m_stack.pop_back();
}

void JSONSerializer::write(const std::vector<char>& vec, const std::string& fieldName)
{
	auto data = base64_encode((const unsigned char*)&vec[0], vec.size());
	write(data, fieldName);
}


void JSONSerializer::read(std::vector<char>& vec, const std::string& fieldName)
{
	//TODO
}