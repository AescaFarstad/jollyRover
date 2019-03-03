#pragma once
#include <memory>

#include <functional>
#include <NetworkPacket.h>


#ifdef __EMSCRIPTEN__
	#include <SDL_net.h>
#else
	#include <SDL2/SDL_net.h>
#endif

class PacketReader
{
public:
	PacketReader(TCPsocket* socket, 
		std::function< std::unique_ptr<NetworkPacket> () > packetCreateFunction,
		std::function< bool() > socketNudgeFunction);
	~PacketReader();

	std::unique_ptr<NetworkPacket> poll();
	void setDataAvailable(bool value);

private:
	static const int16_t BUFFER_SIZE = 513;
	char buffer[BUFFER_SIZE];
	int16_t bufferCursor;
	int16_t bytesInBuffer;
	TCPsocket* socket;
	std::unique_ptr<NetworkPacket> currentPacket;
	bool lastReadWasExastive;
	bool hasDataAvailable;
	std::function< std::unique_ptr<NetworkPacket>() > packetCreateFunction;
	std::function< bool() > socketNudgeFunction;

	bool tryToRead();
	bool nudgeSocket();
};
