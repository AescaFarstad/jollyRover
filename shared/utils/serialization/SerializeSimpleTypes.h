#include <cstdint>
#include <string.h>
#include <string>

namespace Serializer {
	
	#ifdef __EMSCRIPTEN__
	void write(const size_t& value, char buffer[]);
	#endif
	void write(const int64_t& value, char buffer[]);
	void write(const uint64_t& value, char buffer[]);
	void write(const int32_t& value, char buffer[]);
	void write(const uint32_t& value, char buffer[]);
	void write(const int16_t& value, char buffer[]);
	void write(const uint16_t& value, char buffer[]);
	void write(const int8_t& value, char buffer[]);
	void write(const bool& value, char buffer[]);
	void write(const float& value, char buffer[]);
	void write(const std::string& value, char buffer[]);
	size_t sizeOfString(const std::string& value);
	
	#ifdef __EMSCRIPTEN__
	void read(size_t& out, const char* value);
	#endif
	void read(int64_t& out, const char* value);
	void read(uint64_t& out, const char* value);
	void read(int32_t& out, const char* value);
	void read(uint32_t& out, const char* value);
	void read(int16_t& out, const char* value);
	void read(uint16_t& out, const char* value);
	void read(int8_t& out, const char* value);
	void read(bool& out, const char* value);
	void read(float& out, const char* value);
	void read(std::string& out, const char* value);
	
	
	std::string toHex(const char* source, size_t size);
	std::string toDec(const char* source, size_t size);
	
	template <typename T>
	void swapBytes(T* target)
	{
		T tmp = *target;
		unsigned char* ptmp = (unsigned char*)&tmp;
		unsigned char* ttmp = (unsigned char*)target;
		for(size_t i = 0; i < sizeof(T); i++)
			ttmp[i] = ptmp[sizeof(T) - 1 - i];
	}
	
}