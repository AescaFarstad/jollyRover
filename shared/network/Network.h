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
	virtual ~Network();

	NetworkMessageFactory factory;
	TimeSync timeSync;
	ResponseBinder binder;
	GenericRequestBinder genericRequestBinder;
	
	virtual void connect();
	virtual void update();
	virtual void send(NetworkPacket* packet);
	virtual void send(NetworkMessage* msg);
	
	void interceptOnce(MESSAGE_TYPE messageType, MessageHandler handler);
	void interceptGenericRequestOnce(REQUEST_TYPE requestType, GenericRequestHandler handler);

protected:

	std::unique_ptr<NetworkMessage> processIncomingPacket(std::unique_ptr<NetworkPacket> packet);

private:

	TCPsocket socket;
	PacketReader* packetReader;
	SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(1);
	int activeSockets = 0;
	bool isConnected = false;
	std::unordered_map<int32_t, uint32_t> requestTimeByInitiatorId;
	std::unordered_map<MESSAGE_TYPE, MessageHandler> interceptors_once;
	std::unordered_map<REQUEST_TYPE, GenericRequestHandler> interceptorsGeneric_once;
	
	static std::unique_ptr<NetworkPacket> getNewPacket();
	
	virtual std::unique_ptr<NetworkMessage> poll();



};