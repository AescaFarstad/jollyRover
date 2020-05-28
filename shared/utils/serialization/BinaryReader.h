#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>

using size_t=std::size_t;

class BinaryReader
{
public:
	BinaryReader();
	BinaryReader(const char* data, size_t size);
	BinaryReader(const std::vector<char>& data); //vector<char> must not reallocate...
	virtual ~BinaryReader() = default;

	virtual const char* read(size_t size);
	virtual const char* peek(size_t size);
	virtual std::vector<char> readAll() const;
	virtual void resetCursors();
	virtual size_t getLength() const;
	virtual size_t getDataLeft() const;
	size_t loc();
	
	virtual std::string crc();
	
private:
	size_t m_readCursor;
	const char* m_data;
	size_t m_size;
};