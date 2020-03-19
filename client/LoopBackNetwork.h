#pragma once

#include <GameUpdater.h>
#include <Network.h>
#include <vector>

class LoopBackNetwork : public Network
{
public:
	LoopBackNetwork(GameUpdater* gameUpdater);
	virtual ~LoopBackNetwork() = default;

	void connect() override;
	void update() override;
	void send(const NetworkPacket& packet) override;
	void send(const NetworkMessage& msg) override;
	bool isConnected() override;
	

private:
	std::vector<NetworkPacket*> buffer;
	GameUpdater* gameUpdater;
	int16_t idCounter;
	
	void addToIncoming(NetworkMessage& message);
	
	const int16_t LOGIN = 102;
};