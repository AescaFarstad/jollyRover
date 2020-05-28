#include <GameInputBinding.h>
#include <std2.h>


GameInputBinding::GameInputBinding(GameUpdater* gameUpdater, std::string name) : ResponseBinding(name)
{
	this->gameUpdater = gameUpdater;	
}

void GameInputBinding::handle(std::unique_ptr<NetworkMessage> message)
{
	gameUpdater->addNewInput(std2::unique_ptr_cast<InputMessage>(std::move(message)));
}
