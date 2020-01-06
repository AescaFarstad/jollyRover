#include <PacketReader.h>

PacketReader::PacketReader(TCPsocket* socket, 
	std::function< std::unique_ptr<NetworkPacket>() > packetCreateFunction,
	std::function< bool() > socketNudgeFunction)
{
	this->socket = socket;
	bytesInBuffer = 0;
	bufferCursor = 0;

	this->packetCreateFunction = packetCreateFunction;
	this->socketNudgeFunction = socketNudgeFunction;
}

std::unique_ptr<NetworkPacket> PacketReader::poll()
{
	if (!currentPacket)
		currentPacket = packetCreateFunction();
	
	while (true)
	{
		bool socketIsEmpty = tryToRead();
		if (currentPacket->isFullyLoaded())
			return std::move(currentPacket);
		if (socketIsEmpty)
			return nullptr;
		if (bytesInBuffer == -1)
		{
			currentPacket->isDisconnectNotice = true;
			return std::move(currentPacket);
		}
	}
}

void PacketReader::setDataAvailable(bool value)
{
	hasDataAvailable = value;
	if (value)
		lastReadWasExhaustive = false;
}

bool PacketReader::tryToRead()
{
	//Parse packet using already received data
	if (bufferCursor != bytesInBuffer)
	{
		int16_t bytesBefore = currentPacket->bytesLoaded;
		currentPacket->loadFromRawData(buffer + bufferCursor, bytesInBuffer - bufferCursor);
		bufferCursor += currentPacket->bytesLoaded - bytesBefore;
	}

	if (currentPacket->isFullyLoaded())
		return false;

	//Get new block of data from the socket
	//If the last time data in the socket was smaller than the buffer size -> there isn't anything else, don't try to received anything
	//If no data has been received during this update -> socket must have some data in it, can receive it once
	//If socket had at least as much data as its buffer size -> we don't know if it has data or not -> need to nudge
	if (lastReadWasExhaustive)
		return true;

	if (!hasDataAvailable ? nudgeSocket() : true)
	{
		bufferCursor = 0;
		bytesInBuffer = SDLNet_TCP_Recv(*socket, buffer, BUFFER_SIZE);
		lastReadWasExhaustive = bytesInBuffer < BUFFER_SIZE;
		hasDataAvailable = false;
		return false;
	}
	else
	{
		return true;
	}
}

bool PacketReader::nudgeSocket()
{
	lastReadWasExhaustive = false;
	return socketNudgeFunction();

}
