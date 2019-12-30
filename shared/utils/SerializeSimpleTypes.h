#include <cstdint>
#include <string.h>
#include <string>

namespace Serializer {
	
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
	
}