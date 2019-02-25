#include <GameInputBinding.h>


GameInputBinding::GameInputBinding(GameUpdater* gameUpdater, std::string name) : ResponseBinding(name)
{
	this->gameUpdater = gameUpdater;	
}


GameInputBinding::~GameInputBinding()
{
}

void GameInputBinding::handle(std::unique_ptr<NetworkMessage> message)
{
	InputMessage* t = dynamic_cast<InputMessage*>(message.release());
	gameUpdater->addNewInput(std::unique_ptr<InputMessage>(t));
}
