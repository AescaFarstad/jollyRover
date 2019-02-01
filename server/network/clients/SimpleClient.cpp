#include <SimpleClient.h>



SimpleClient::SimpleClient(std::function< int() > globalSocketNudgeFunction) : NetworkClient(globalSocketNudgeFunction)
{
}


SimpleClient::~SimpleClient()
{
}


void SimpleClient::sendMessage(NetworkMessage* msg)
{
	NetworkPacket packet;
	packet.setPayloadFromSerializable(msg);

	logSend(msg->getName(), packet);

	SDLNet_TCP_Send(socket, packet.rawData, packet.rawSize);
}

void SimpleClient::sendMessage(const char* payload, size_t size)
{
	NetworkPacket packet;
	packet.setPayloadFromRawData(payload, size);

	logSend("______", packet);

	SDLNet_TCP_Send(socket, packet.rawData, packet.rawSize);
}


