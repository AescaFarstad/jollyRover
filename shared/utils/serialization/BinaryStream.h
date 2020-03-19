#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>


/*
	Lifecycle:
	
		init
		allocate, allocate .....
		read, read ...
		resetCursors
		read, read ...
		...
		
	Writing resets reading cursor		
*/

using size_t=std::size_t;
using GrowStrategy=std::function<size_t(size_t previousSize, size_t  allocation)>;

namespace BinaryStreamInternal
{
	class Block
	{
	public:
		Block();
		~Block();
		char* array;
		size_t size;
		size_t occupied;
	};
	
	struct Cursor
	{
		size_t position;
		Block* block;
	};	
}

class BinaryStream
{
public:
	BinaryStream();
	BinaryStream(size_t startingSize, GrowStrategy growStrategy = exponentialGrow);
	
	void init(size_t startingSize, GrowStrategy growStrategy);

	///Resets reading cursor.
	char* allocate(size_t size);
	const char* read(size_t size);
	void resetCursors();
	void reset(size_t startingSize);
	std::vector<char> readAll();
	size_t getLength();	
	
	std::string crc();
	
	
	static constexpr float growExponent = 2;
	static std::size_t exponentialGrow(size_t previousSize, size_t  allocation);
	
private:
	BinaryStreamInternal::Cursor m_writeCursor;
	BinaryStreamInternal::Cursor m_readCursor;
	std::vector<BinaryStreamInternal::Block> m_blocks;
	
	GrowStrategy m_growStrategy;
	
	static std::size_t noGrow(size_t previousSize, size_t  allocation);
};