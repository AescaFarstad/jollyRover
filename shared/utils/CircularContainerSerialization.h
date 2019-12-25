
	
#include <CircularContainer.h>
#include <ISerializable.h>

	template <typename T, uint32_t arraySize>
	void read(CircularContainer<T, arraySize>& out, SerializationStream& stream)
	{
		read(out.cursor, stream);
		read(out.total, stream);
		read(out.size, stream);
		int32_t readLength = std::min(sizeof(out.array), (long unsigned int)out.total);
		memcpy(&out.array, stream.read(out.array, readLength), readLength);		
	}
	
	template <typename T, uint32_t arraySize>
	void write(const CircularContainer<T, arraySize> value, SerializationStream& stream)
	{	
		write(value.cursor, stream);
		write(value.total, stream);
		write(value.size, stream);
		stream.write(value.array, std::min((int)sizeof(value.array), (int)value.total));
	}