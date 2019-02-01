#include <LoopBackNetwork.h>
#include <MessageTypes.h>

LoopBackNetwork::LoopBackNetwork(GameUpdater* gameUpdater)
{
	this->gameUpdater = gameUpdater;
	idCounter = 0;
}

LoopBackNetwork::~LoopBackNetwork()
{
}

void LoopBackNetwork::connect()
{
	
}

void LoopBackNetwork::update()
{
	
}

void LoopBackNetwork::send(NetworkPacket* packet)
{
	std::unique_ptr<NetworkMessage> msg = factory.parse(packet);
	send(msg.get());
}

void LoopBackNetwork::send(NetworkMessage* message)
{
	switch (message->typeId)
	{
		case MessageTypes::TYPE_INPUT_ACTION_MSG:
		case MessageTypes::TYPE_INPUT_JOINED_MSG:
		case MessageTypes::TYPE_INPUT_LEFT_MSG:
		{
			InputMessage* iMsg = dynamic_cast<InputMessage*>(message);
			iMsg->serverStamp = gameUpdater->state->timeStamp;
			iMsg->serverId = idCounter++;
			break;
		}
		default:
			break;
	}

	std::unique_ptr<NetworkPacket> newPacket = std::make_unique<NetworkPacket>();
	newPacket->setPayloadFromSerializable(message);
	buffer.push_back(std::move(newPacket));
}

std::unique_ptr<NetworkMessage> LoopBackNetwork::poll()
{
	return nullptr;
	/*
	if (buffer.empty())
	{
		return nullptr;
	}
	else
	{
		std::unique_ptr<NetworkMessage> result = std::move(buffer.front());
		buffer.erase(buffer.begin());
		return std::move(result);
	}*/
}
