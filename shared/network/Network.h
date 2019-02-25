#pragma once
#include <NetworkPacket.h>
#include <TimeSync.h>
#include <NetworkMessageFactory.h>
#include <PacketReader.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>
#include <ResponseBinder.h>
#include <AnonymousBinding.h>

#ifdef __EMSCRIPTEN__
	#include <SimplePacket.h>
	#include <SDL_net.h>
#else
	#include <SDL2/SDL_net.h>
#endif

using MessageHandler = std::function<void(std::unique_ptr<NetworkMessage>)>;
using GenericRequestHandler = std::function<void(std::unique_ptr<GenericRequestMessage>)>;

class Network
{
public:
	Network();
	~Network();

	NetworkMessageFactory factory;
	TimeSync timeSync;
	ResponseBinder binder;
	GenericRequestBinder genericRequestBinder;
	
	virtual void connect();
	virtual void update();
	virtual void send(NetworkPacket* packet);
	virtual void send(NetworkMessage* msg);
	void interceptOnce(int16_t messageType, MessageHandler handler);
	void interceptGenericRequestOnce(int16_t requestType, GenericRequestHandler handler);


private:

	TCPsocket socket;
	PacketReader* packetReader;
	SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1);
	int activeSockets = 0;
	bool isConnected = false;
	std::unordered_map<int32_t, uint32_t> requestTimeByInitiatorId;
	static std::unique_ptr<NetworkPacket> getNewPacket();
	std::unordered_map<int16_t, MessageHandler> interceptors_once;
	std::unordered_map<int16_t, GenericRequestHandler> interceptorsGeneric_once;
	virtual std::unique_ptr<NetworkMessage> poll();



};