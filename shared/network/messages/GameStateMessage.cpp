#include <GameStateMessage.h>



GameStateMessage::GameStateMessage()
{
	typeId = MessageTypes::TYPE_GAME_STATE_MSG;
	ownsState = false;
}


GameStateMessage::~GameStateMessage()
{
	if (ownsState && state != nullptr)
		delete state;
}


void GameStateMessage::deserialize(SerializationStream& stream)
{
	NetworkMessage::deserialize(stream);
	state = new GameState();
	state->deserialize(stream);
	ownsState = true;
}

void GameStateMessage::serialize(SerializationStream& stream) const
{
	NetworkMessage::serialize(stream);
	state->serialize(stream);
}


std::string GameStateMessage::getName()
{
	return "GState";
}
