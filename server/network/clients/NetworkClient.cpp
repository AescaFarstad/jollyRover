#include <NetworkClient.h>



NetworkClient::NetworkClient(std::function< int() > globalSocketNudgeFunction)
{
	wasConnected = false;
	wasDisconnected = false;
	isInitialized = false;
	this->globalSocketNudgeFunction = globalSocketNudgeFunction;
}


NetworkClient::~NetworkClient()
{
	if (isInitialized)
		delete packetReader;
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

void NetworkClient::sendMessage(NetworkMessage * msg)
{
}

void NetworkClient::sendMessage(const char * payload, size_t size)
{
}

void NetworkClient::init()
{
	packetReader = new PacketReader(&socket, getPacket, [this]() {
		int activeSockets = globalSocketNudgeFunction();
		return activeSockets > 0 && SDLNet_SocketReady(socket); });
	isInitialized = true;
}

void NetworkClient::logSend(std::string messageName, NetworkPacket &packet)
{
	S::log.add("< SEND " + messageName + " to " + std::to_string(login) + ":\n\t" +
		Serializer::toHex(packet.payload, packet.payloadSize),
		{ LOG_TAGS::NET, LOG_TAGS::NET_MESSAGE });
}

std::unique_ptr<NetworkPacket> NetworkClient::getPacket()
{
	return std::make_unique<NetworkPacket>();
}
