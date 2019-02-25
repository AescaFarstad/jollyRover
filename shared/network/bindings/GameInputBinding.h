#pragma once
#include <ResponseBinding.h>
#include <InputMessage.h>
#include <GameUpdater.h>

class GameInputBinding :
	public ResponseBinding
{
public:
	GameInputBinding(GameUpdater* gameUpdater, std::string name);
	~GameInputBinding();

	void handle(std::unique_ptr<NetworkMessage> message) override;
private:
	GameUpdater* gameUpdater;
};

