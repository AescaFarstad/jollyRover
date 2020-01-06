#include <SimpleClient.h>

SimpleClient::SimpleClient(std::function< int() > globalSocketNudgeFunction) : NetworkClient(globalSocketNudgeFunction){}

void SimpleClient::sendMessage(const NetworkMessage& msg)
{
	NetworkPacket packet;
	packet.setPayloadFromSerializable(msg);

	logSend(msg.getName(), packet);

	auto sentSize = SDLNet_TCP_Send(socket, packet.rawData, packet.rawSize);
	if (packet.rawSize != sentSize)
		wasDisconnected = true;
}

void SimpleClient::sendMessage(const char* payload, size_t size)
{
	NetworkPacket packet;
	packet.setPayloadFromRawData(payload, size);

	logSend("______", packet);

	auto sentSize = SDLNet_TCP_Send(socket, packet.rawData, packet.rawSize);
	if (packet.rawSize != sentSize)
		wasDisconnected = true;
}


