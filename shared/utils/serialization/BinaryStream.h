#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <BinaryReader.h>


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
		Block(size_t size);
		~Block();
		
		Block(Block&& that);
		
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

class BinaryStream : public BinaryReader
{
public:
	BinaryStream();
	BinaryStream(size_t startingSize, GrowStrategy growStrategy = exponentialGrow);
	
	void init(size_t startingSize, GrowStrategy growStrategy);

	///Resets reading cursor.
	char* allocate(size_t size);
	const char* read(size_t size) override;
	const char* peek(size_t size) override;
	void resetCursors() override;
	void reset(size_t startingSize);
	std::vector<char> readAll() const override;
	size_t getLength() const override;
	size_t getDataLeft() const override;
	
	size_t loc();
	
	std::string crc() override;
	
	
	static constexpr float growExponent = 2;
	static std::size_t exponentialGrow(size_t previousSize, size_t  allocation);
	
private:
	BinaryStreamInternal::Cursor m_writeCursor;
	BinaryStreamInternal::Cursor m_readCursor;
	std::vector<BinaryStreamInternal::Block> m_blocks;
	
	GrowStrategy m_growStrategy;
	
	static std::size_t noGrow(size_t previousSize, size_t  allocation);
};