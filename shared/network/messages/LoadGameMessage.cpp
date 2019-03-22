#include <LoadGameMessage.h>

LoadGameMessage::LoadGameMessage()
{
	typeId = MessageTypes::TYPE_LOAD_GAME_MSG;
	state = nullptr;
}

LoadGameMessage::~LoadGameMessage()
{
	if (state != nullptr)
		delete[] state;
}

void LoadGameMessage::deserialize(SerializationStream& stream)
{
	InputMessage::deserialize(stream);
	Serializer::read(stateLength, stream);
	
	state = new char[stateLength];
	auto content = stream.read(stateLength);
	memcpy(state, content, stateLength);
}

void LoadGameMessage::serialize(SerializationStream& stream)
{
	InputMessage::serialize(stream);
	Serializer::write(stateLength, stream);
	stream.write(state, stateLength);
}

std::string LoadGameMessage::getName()
{
	return "I-Load";
}