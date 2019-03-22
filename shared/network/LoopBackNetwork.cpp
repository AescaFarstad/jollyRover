#include <LoopBackNetwork.h>

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
	timeSync.addMeasurement(0, 0, 0);
	GenericRequestMessage grm;
	grm.request = RequestTypes::REQUEST_GREETING;
	addToIncoming(&grm);
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
				S::log.add("NetworkMessage is not handled: " + name, { LOG_TAGS::NET });
			}
		}
	}
}

void LoopBackNetwork::send(NetworkPacket* packet)
{
	std::unique_ptr<NetworkMessage> msg = factory.parse(packet);
	send(msg.get());
}

void LoopBackNetwork::send(NetworkMessage* message)
{
	message->login = LOGIN;
	switch (message->typeId)
	{
		case MessageTypes::TYPE_INPUT_ACTION_MSG:
		case MessageTypes::TYPE_INPUT_JOINED_MSG:
		case MessageTypes::TYPE_INPUT_LEFT_MSG:
		case MessageTypes::TYPE_INPUT_ROUTE_MSG:
		case MessageTypes::TYPE_INPUT_TIME_MSG:
		case MessageTypes::TYPE_LOAD_GAME_MSG:
		{
			InputMessage* iMsg = dynamic_cast<InputMessage*>(message);
			iMsg->serverStamp = gameUpdater->state->timeStamp;
			iMsg->serverId = idCounter++;
			addToIncoming(message);
			break;
		}
		case MessageTypes::TYPE_REQUEST_MSG:
		{
			GenericRequestMessage* t = dynamic_cast<GenericRequestMessage*>(message);
			std::unique_ptr<GenericRequestMessage> genericRequestMsg = std::make_unique<GenericRequestMessage>(*t);
			switch (genericRequestMsg->request)
			{
				case RequestTypes::REQUEST_JOIN_GAME:
				{
					GameStateMessage gsMsg;
					GameState tempState(1934);
					gsMsg.inResponseTo = genericRequestMsg->initiator_id;
					gsMsg.state = &tempState;
					addToIncoming(&gsMsg);

					InputPlayerJoinedMessage pjMsg;
					pjMsg.serverId = idCounter++;
					pjMsg.serverStamp = tempState.timeStamp;
					pjMsg.login = genericRequestMsg->login;
					addToIncoming(&pjMsg);
					break;
				}
				case RequestTypes::REQUEST_GAME_STATE:
				{
					GameStateMessage gsMsg;
					gsMsg.inResponseTo = genericRequestMsg->initiator_id;
					gsMsg.state = gameUpdater->state.get(); 
					addToIncoming(&gsMsg);
					break;
				}
				case RequestTypes::REQUEST_PING:
				{
					GenericRequestMessage grMsg;
					grMsg.request = RequestTypes::REQUEST_PONG;
					grMsg.inResponseTo = genericRequestMsg->initiator_id;
					addToIncoming(&grMsg);
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
		case MessageTypes::TYPE_GREETING_MSG:
		{
			GreetingMessage* gMsg = dynamic_cast<GreetingMessage*>(message);
			gMsg->password = {1, 2, 3, 4, 5};
			addToIncoming(message);
			break;
		}
		default:
		{
			S::log.add("Message type not handled: " + std::to_string((int)message->typeId), {LOG_TAGS::ERROR_});
			break;
		}
	}
}


void LoopBackNetwork::addToIncoming(NetworkMessage* message)
{
	message->login = LOGIN;
	message->stamp = SDL_GetTicks();
	message->inResponseTo = message->initiator_id;
	NetworkPacket* newPacket = new NetworkPacket();
	buffer.push_back(newPacket);
	newPacket->setPayloadFromSerializable(message);
}