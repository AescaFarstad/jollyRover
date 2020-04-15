#include <NetworkClient.h>
#include <SerializeSimpleTypes.h>
#include <Global.h>

NetworkClient::NetworkClient(std::function< int32_t() > globalSocketNudgeFunction)
{
	wasConnected = false;
	wasDisconnected = false;
	isInitialized = false;
	hasDesynced = false;
	connectionStamp = 0;
	this->globalSocketNudgeFunction = globalSocketNudgeFunction;
}

std::unique_ptr<NetworkPacket> NetworkClient::poll()
{
	if (!isInitialized)
		init();
	packetReader->setDataAvailable(SDLNet_SocketReady(socket));
	auto packet = packetReader->poll();
	if (packet && packet->isDisconnectNotice)
	{
		wasDisconnected = true;
		return nullptr;
	}
	return std::move(packet);
}

void NetworkClient::init()
{
	packetReader = std::make_unique<PacketReader>(&socket, getPacket, [this]() {
		int32_t activeSockets = globalSocketNudgeFunction();
		return activeSockets > 0 && SDLNet_SocketReady(socket); });
	isInitialized = true;
}

void NetworkClient::logSend(std::string messageName, NetworkPacket& packet)
{
	S::log.add("< SEND " + messageName + " to " + std::to_string(login) + ":\n\t" +
		Serializer::toHex(packet.payload, packet.payloadSize),
		{ LOG_TAGS::NET, LOG_TAGS::NET_MESSAGE });
	S::log.add("< SEND " + messageName + "[" + std::to_string(packet.payloadSize) + "]" + " to " + std::to_string(login),
		{LOG_TAGS::NET_BRIEF });
}

std::unique_ptr<NetworkPacket> NetworkClient::getPacket()
{
	return std::make_unique<NetworkPacket>();
}
