#pragma once
#include <Serializer.h>

class ISerializable
{
public:
	virtual ~ISerializable();

	virtual void deserialize(SerializationStream& stream) = 0;
	virtual void serialize(SerializationStream& stream) const = 0;
};

namespace Serializer {

	void write(ISerializable& value, SerializationStream& stream);
	void read(ISerializable& value, SerializationStream& stream);
	void write(ISerializable* value, SerializationStream& stream);
	void read(ISerializable* value, SerializationStream& stream);	
	
	template <typename T>
	void write(const T& value, SerializationStream& stream)
	{
		value.serialize(stream);
	}
	
	template <typename T>
	void read(T& value, SerializationStream& stream)
	{
		value.deserialize(stream);
	}
	template <typename T>
	void write(const T* value, SerializationStream& stream)
	{
		value->serialize(stream);
	}
	template <typename T>
	void read(T* value, SerializationStream& stream)
	{
		value->deserialize(stream);
	}

	template <typename T>
	void writeVector(const std::vector<T> &value, SerializationStream& stream)
	{
		int16_t size = (int16_t)value.size();
		write(size, stream);
		for (int16_t i = 0; i < size; i++)
		{
			write(value[i], stream);
		}
	}

	template <typename T>
	void readVector(std::vector<T> &value, SerializationStream& stream)
	{
		int16_t size;
		read(size, stream);
		value.clear();
		for (int16_t i = 0; i < size; i++)
		{
			value.emplace_back();
			read(value[i], stream);
		}
	}
	
	template <typename T>
	std::unique_ptr<T> copyThroughSerialization(const T& that)
	{
		auto result = std::make_unique<T>();
		
		auto s = SerializationStream::createExp();
		Serializer::write(that, s);
		s.seekAbsolute(0);
		
		Serializer::read(*result, s);
		
		return std::move(result);
	}
}

