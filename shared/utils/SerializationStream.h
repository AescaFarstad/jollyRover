#pragma once
#include <iostream>
#include <vector>
#include <string> 
#include <memory> 
#include <Global.h>

class StreamGrower;
class CharBlock;

struct BlockCursor
{
	CharBlock* block;
	size_t index;
};

class SerializationStream 
{
	friend class StreamGrower;
public:
	SerializationStream(StreamGrower* grower);
	SerializationStream();
	~SerializationStream();

	const char* read(size_t length);
	char* readAll();
	char* c_str();
	char* readAllAsHex();
	char* readAllAsHex_c();
	char* readAllAsBase64();
	char* peek();
	void seekAbsolute(size_t position);
	void seekRelative(size_t position);
	void seekEnd();
	static bool AGA;
	size_t getLength();

	void write(const char* data, size_t length);

	static std::unique_ptr<SerializationStream> createExp(size_t base  = 128, size_t exponent = 2);

protected:
	std::vector<CharBlock*> blocks;
	size_t totalLength = 0;
	StreamGrower* grower;
	BlockCursor cursor;

	void grow(size_t blockSize); 
	void writeSimple(const char* data, size_t length);
	size_t getAbsolutePosition();
	CharBlock* getNextBlock(CharBlock* currentBlock);
	char* nullTerminate(char* source, size_t length);


};

class CharBlock
{
public:
	CharBlock(size_t size);
	~CharBlock();

	char* block;
	size_t capacity;
	size_t occupied;
};

class StreamGrower
{
public:
	StreamGrower();
	~StreamGrower();

	virtual void grow(SerializationStream* stream, size_t minimum);
	size_t getBlockCount(SerializationStream* stream);
	CharBlock* getLastBlock(SerializationStream* stream);
private:


};

class StreamGrowerExp : public StreamGrower
{
public:
	StreamGrowerExp(size_t base, size_t exponent);
	~StreamGrowerExp();

	virtual void grow(SerializationStream* stream, size_t minimum);

private:
	size_t base;
	size_t exponent;

};