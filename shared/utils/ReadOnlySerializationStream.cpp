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
	m_grower = new StreamNoGrower();
	CharBlock* newBlock = new CharBlock(length);
	newBlock->block = const_cast<char*>(data);
	newBlock->occupied = length;
	m_blocks.push_back(newBlock);
	m_cursor.block = newBlock;
	m_cursor.index = 0;
}

ReadOnlySerializationStream::~ReadOnlySerializationStream()
{
	m_blocks.clear();
}

void ReadOnlySerializationStream::write(const char * data, size_t length)
{
	THROW_FATAL_ERROR("Can't write to ReadOnlyStream");
}
