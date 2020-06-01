#pragma once
#include <GameUpdater.h>
#include <DemoWriter.h>
#include <Prototypes.h>
#include <ServerNetwork.h>

class Root
{
public:
	Root() = default;
	void init();
	bool mainLoop();
	void quit();	
	
private:

	static constexpr int32_t MIN_TIME_PER_FRAME = 10;

	ServerNetwork m_network;	
	GameUpdater m_gameUpdater;
	MessageBuffer m_messageBuffer;
	Prototypes m_prototypes;
	DemoWriter m_demoWriter;
	
	int32_t m_lastTicks = 0;
	int32_t m_inputIdCounter = 0;
	int32_t m_timeWithoutClients = 0;
	bool m_isIdle = false;
	
	static std::string getCurrentTimeString();
	
	void activeLoop();
	void idleLoop();
	void loadPrototypes();
	void loadConfig();
	void handleNetworkMessage(std::unique_ptr<NetworkMessage> message);

};
