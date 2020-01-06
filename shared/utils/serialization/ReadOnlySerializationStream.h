#pragma once
#include <SerializationStream.h>
#include <Global.h>

class ReadOnlySerializationStream :
	public SerializationStream
{
public:
	ReadOnlySerializationStream(const char* data, size_t length);
	~ReadOnlySerializationStream() override;

	void write(const char* data, size_t length) override;
};

