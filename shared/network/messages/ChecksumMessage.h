#pragma once
#include <cstdint>
#include <utility>
#include <string>
#include <Serialization.h>

namespace Serialization
{
	
	//std::pair<uint32_t, std::string>-------------------------------------------------------
	
	template <typename T>
	void write(const std::pair<uint32_t, std::string>& object, T& serializer)
	{
		WRITE_FIELD(object, serializer, first);
		WRITE_FIELD(object, serializer, second);
	}
	
	template <typename T>
	void read(std::pair<uint32_t, std::string>& object, T& serializer)
	{
		READ__FIELD(object, serializer, first);
		READ__FIELD(object, serializer, second);
	}
}

#include <NetworkMessage.h>

class ChecksumMessage :
	public NetworkMessage
{
public:
	ChecksumMessage();

	std::vector<std::pair<uint32_t, std::string>> checksums;
	
	std::string getName() const override;
	
	void serialize(BinarySerializer& serializer) const override;
	void deserialize(BinarySerializer& serializer) override;
};
