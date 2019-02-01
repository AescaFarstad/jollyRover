#pragma once
#include <Serializer.h>

class ISerializable
{
public:
	ISerializable();
	virtual ~ISerializable();

	virtual void deserialize(SerializationStream& stream);
	virtual void serialize(SerializationStream& stream);
};

namespace Serializer {

	void write(ISerializable& value, SerializationStream& stream);
	void read(ISerializable& value, SerializationStream& stream);
	void write(ISerializable* value, SerializationStream& stream);
	void read(ISerializable* value, SerializationStream& stream);

	template <typename T>
	void writeVector(std::vector<T> &value, SerializationStream& stream)
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
}

