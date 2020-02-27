#include <SimpleClient.h>
#include <NetworkPacket.h>

SimpleClient::SimpleClient(
	std::function<void(NetworkClient& client)> onHandshakeDone, 
	std::function< int32_t() > globalSocketNudgeFunction) : NetworkClient(globalSocketNudgeFunction)
{
	m_onHandshakeDone = onHandshakeDone;
	m_isHandshakeDone = false;
}

std::unique_ptr<NetworkPacket> SimpleClient::poll()
{
	if (m_isHandshakeDone)
	{
		return NetworkClient::poll();
	}
	else
	{
		char buffer[S::config.maxMessageSize];

		int32_t bytes = SDLNet_TCP_Recv(socket, buffer, S::config.maxMessageSize);

		if (bytes <= 0)
		{
			wasDisconnected = true;
			return nullptr;
		}
		if (bytes >= 4)
		{
			int32_t code;
			Serializer::read(code, buffer);
			if (code == S::config.simpleClientCode)
			{
				m_isHandshakeDone = true;
				m_onHandshakeDone(*this);
				return nullptr;
			}
		}

		wasDisconnected = true;
		return nullptr;
	}

}


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


