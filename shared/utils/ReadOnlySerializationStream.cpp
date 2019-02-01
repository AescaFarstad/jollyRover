#include <ReadOnlySerializationStream.h>



class StreamNoGrower : public StreamGrower
{
public:
	StreamNoGrower()
	{
	}

	~StreamNoGrower()
	{
	}

	void grow(SerializationStream * stream, size_t minimum)
	{
	}

private:
};




ReadOnlySerializationStream::ReadOnlySerializationStream(const char* data, size_t length)
{
	grower = new StreamNoGrower();
	CharBlock* newBlock = new CharBlock(length);
	newBlock->block = const_cast<char*>(data);
	newBlock->occupied = length;
	blocks.push_back(newBlock);
	cursor.block = newBlock;
	cursor.index = 0;
}

ReadOnlySerializationStream::~ReadOnlySerializationStream()
{
	blocks.clear();
}

void ReadOnlySerializationStream::write(const char * data, size_t length)
{
	THROW_FATAL_ERROR("Can't write to ReadOnlyStream");
}
