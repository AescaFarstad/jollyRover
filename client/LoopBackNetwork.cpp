#include <LoopBackNetwork.h>
#include <GameStateMessage.h>
#include <GreetingMessage.h>
#include <BinarySerializer.h>

LoopBackNetwork::LoopBackNetwork(GameUpdater* gameUpdater) : Network(nullptr) 
{
	this->gameUpdater = gameUpdater;
	idCounter = 0;	
}

void LoopBackNetwork::connect()
{
	timeSync.addMeasurement(SDL_GetTicks(), 0, SDL_GetTicks());
	GenericRequestMessage grm;
	grm.request = REQUEST_TYPE::REQUEST_GREETING;
	addToIncoming(grm);
}

bool LoopBackNetwork::isConnected()
{
	return true;
}

void LoopBackNetwork::update()
{
	auto lastBuffer = buffer;
	buffer.clear();
	for(NetworkPacket* &packet : lastBuffer)
	{
		std::unique_ptr<NetworkMessage> parsedMessage = processIncomingPacket(std::unique_ptr<NetworkPacket>(packet));
		if (parsedMessage)
		{
			std::string name = parsedMessage->getName();
			if (!binder.process(std::move(parsedMessage)))
			{
				S::log.add("NetworkMessage is not handled: " + name, { LOG_TAGS::NET, LOG_TAGS::NET_BRIEF });
			}
		}
	}
}

void LoopBackNetwork::send(const NetworkPacket& packet)
{
	std::unique_ptr<NetworkMessage> msg = factory.parse(packet);
	send(*msg);
}

void LoopBackNetwork::send(const NetworkMessage& message)
{
	NetworkPacket tmpPacket;
	tmpPacket.setPayloadFromSerializable(message);
	auto copy = factory.parse(tmpPacket);
	
	copy->login = LOGIN;
	switch (copy->typeId)
	{
		case MESSAGE_TYPE::TYPE_INPUT_ACTION_MSG:
		case MESSAGE_TYPE::TYPE_INPUT_JOINED_MSG:
		case MESSAGE_TYPE::TYPE_INPUT_LEFT_MSG:
		case MESSAGE_TYPE::TYPE_INPUT_ROUTE_MSG:
		case MESSAGE_TYPE::TYPE_INPUT_TIME_MSG:
		case MESSAGE_TYPE::TYPE_INPUT_IMPULSE_MSG:
		case MESSAGE_TYPE::TYPE_INPUT_DEBUG_MSG:
		case MESSAGE_TYPE::TYPE_LOAD_GAME_MSG:
		{
			InputMessage* iMsg = dynamic_cast<InputMessage*>(copy.get());
			iMsg->serverStamp = gameUpdater->state.timeStamp;
			iMsg->serverId = idCounter++;
			addToIncoming(*copy);
			break;
		}
		case MESSAGE_TYPE::TYPE_REQUEST_MSG:
		{
			GenericRequestMessage* genericRequestMsg = dynamic_cast<GenericRequestMessage*>(copy.get());
			switch (genericRequestMsg->request)
			{
				case REQUEST_TYPE::REQUEST_JOIN_GAME:
				{
					GameStateMessage gsMsg;
					GameState tempState(1934);
					gsMsg.inResponseTo = genericRequestMsg->initiator_id;
					BinarySerializer::copyThroughSerialization(tempState, gsMsg.states.emplace_back());
					addToIncoming(gsMsg);

					InputPlayerJoinedMessage pjMsg;
					pjMsg.serverId = idCounter++;
					pjMsg.serverStamp = tempState.timeStamp;
					pjMsg.login = genericRequestMsg->login;
					addToIncoming(pjMsg);
					
					InputImpulseMessage msg;
					msg.impulse = INPUT_IMPULSE::ADD_AI;
					msg.serverId = idCounter++;
					msg.serverStamp = tempState.timeStamp + 5000;
					msg.login = genericRequestMsg->login;
					addToIncoming(msg);					
	
					break;
				}
				case REQUEST_TYPE::REQUEST_GAME_STATE:
				{
					GameStateMessage gsMsg;
					gsMsg.inResponseTo = genericRequestMsg->initiator_id;
					BinarySerializer::copyThroughSerialization(gameUpdater->state, gsMsg.states.emplace_back());
					addToIncoming(gsMsg);
					break;
				}
				case REQUEST_TYPE::REQUEST_PING:
				{
					GenericRequestMessage grMsg;
					grMsg.request = REQUEST_TYPE::REQUEST_PONG;
					grMsg.inResponseTo = genericRequestMsg->initiator_id;
					addToIncoming(grMsg);
					break;
				}
				default:
				{
					S::log.add("Request type not handled: " + std::to_string((int)genericRequestMsg->request), {LOG_TAGS::ERROR_});
					break;
				}
			}
			break;
		}
		case MESSAGE_TYPE::TYPE_GREETING_MSG:
		{
			GreetingMessage* gMsg = dynamic_cast<GreetingMessage*>(copy.get());
			gMsg->login = LOGIN;
			gMsg->password = {1, 2, 3, 4, 5};
			addToIncoming(*copy);
			break;
		}
		default:
		{
			S::log.add("Message type not handled: " + std::to_string((int16_t)copy->typeId), {LOG_TAGS::ERROR_});
			break;
		}
	}
}

void LoopBackNetwork::addToIncoming(NetworkMessage& message)
{
	message.login = LOGIN;
	message.stamp = SDL_GetTicks();
	message.inResponseTo = message.initiator_id;
	NetworkPacket* newPacket = new NetworkPacket();
	buffer.push_back(newPacket);
	newPacket->setPayloadFromSerializable(message);
}


