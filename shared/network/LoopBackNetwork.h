#pragma once
#include <Network.h>
#include <GameUpdater.h>
#include <list>

class LoopBackNetwork : public Network
{
public:
	LoopBackNetwork(GameUpdater* gameUpdater);
	~LoopBackNetwork();



	void connect();
	void update();
	virtual void send(NetworkPacket* packet);
	virtual void send(NetworkMessage* msg);
	virtual std::unique_ptr<NetworkMessage> poll();


private:
	std::list<std::unique_ptr<NetworkPacket>> buffer;
	GameUpdater* gameUpdater;
	int16_t idCounter;
};