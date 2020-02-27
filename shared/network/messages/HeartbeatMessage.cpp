#include <HeartbeatMessage.h>

HeartbeatMessage::HeartbeatMessage()
{
	typeId = MESSAGE_TYPE::TYPE_HEARTBEAT_MSG;
}

std::string HeartbeatMessage::getName() const
{
	return "heartM";
}