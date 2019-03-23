#include <UndeterminedClient.h>



UndeterminedClient::UndeterminedClient(
	std::function<void(UndeterminedClient* client)> onClientDetermined,
	std::function<int()> globalSocketNudgeFunction) : NetworkClient(globalSocketNudgeFunction)
{	
	this->onClientDetermined = onClientDetermined;
	isSimpleClient = false;
	state = NETWORK_CLIENT_STATE::UNDETERMINED;
}


UndeterminedClient::~UndeterminedClient()
{
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

void UndeterminedClient::clear()
{

}
