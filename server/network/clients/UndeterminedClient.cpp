#include <UndeterminedClient.h>
#include <Global.h>



UndeterminedClient::UndeterminedClient(
	std::function<void(UndeterminedClient* client)> onClientDetermined,
	std::function<int()> globalSocketNudgeFunction) : NetworkClient(globalSocketNudgeFunction)
{	
	this->onClientDetermined = onClientDetermined;
	isSimpleClient = false;
	state = NETWORK_CLIENT_STATE::UNDETERMINED;
}

std::unique_ptr<NetworkPacket> UndeterminedClient::poll()
{
	char buffer[S::config.maxMessageSize];

	int bytes = SDLNet_TCP_Recv(socket, buffer, S::config.maxMessageSize);

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
			isSimpleClient = true;
			onClientDetermined(this);
			return nullptr;
		}
	}

	wasDisconnected = true;
	return nullptr;
}

void UndeterminedClient::sendMessage(const NetworkMessage& msg){THROW_FATAL_ERROR("Cannot send messages to the UndeterminedClient");}
void UndeterminedClient::sendMessage(const char* payload, size_t size){THROW_FATAL_ERROR("Cannot send messages to the UndeterminedClient");}
