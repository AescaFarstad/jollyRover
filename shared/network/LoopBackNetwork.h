#pragma once
#include <Network.h>
#include <GameUpdater.h>
#include <MessageTypes.h>
#include <vector>

class LoopBackNetwork : public Network
{
public:
	LoopBackNetwork(GameUpdater* gameUpdater);
	~LoopBackNetwork();



	virtual void connect() override;
	virtual void update() override;
	virtual void send(NetworkPacket* packet) override;
	virtual void send(NetworkMessage* msg) override;
	

private:
	std::vector<NetworkPacket*> buffer;
	GameUpdater* gameUpdater;
	int16_t idCounter;
	
	void addToIncoming(NetworkMessage* msg);
	
	const int16_t LOGIN = 102;
};