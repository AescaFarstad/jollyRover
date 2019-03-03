#include <WebClient.h>



WebClient::WebClient(
	std::function<void(NetworkClient* client)> onHttpHandshakeDone, 
	std::function< int() > globalSocketNudgeFunction) : NetworkClient(globalSocketNudgeFunction)
{
	this->onHttpHandshakeDone = onHttpHandshakeDone;
	isHandshakeDone = false;
}


WebClient::~WebClient()
{
}


static inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
	}));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
	ltrim(s);
	rtrim(s);
}

std::unique_ptr<NetworkPacket> WebClient::poll()
{
	if (isHandshakeDone)
	{
		return NetworkClient::poll();
	}
	else
	{
		char buffer[GAME_CONFIG::maxMessageSize];
		int bytes = SDLNet_TCP_Recv(socket, buffer, GAME_CONFIG::maxMessageSize);
		if (bytes <= 0)
		{
			wasDisconnected = true;
			return nullptr;
		}

		if (buffer[0] == 'G' && buffer[1] == 'E' && buffer[2] == 'T')
		{
			std::string requestString;
			requestString.assign(buffer);
			std::regex code("Sec-WebSocket-Key: (.*)");
			std::smatch match;
			std::regex_search(requestString, match, code);
			std::string hashParam = match[1].str();
			std::string debug = "requestcode:" + hashParam + "\n";
			//printf(debug.c_str());
			trim(hashParam);
			char* responseHash = new char[29];
			WebSocketHandshake::generate(hashParam.c_str(), responseHash);
			responseHash[28] = '\0';
			std::string responseHashStr;
			responseHashStr.assign(responseHash);

			std::string response = "HTTP/1.1 101 Switching Protocols\r\n";
			response += "Connection: Upgrade\r\n";
			response += "Upgrade: websocket\r\n";
			response += "Sec-WebSocket-Protocol: binary\r\n";
			response += "Sec-WebSocket-Accept: " + responseHashStr + "\r\n\r\n";

			//printf(response.c_str());
			//printf("\n");
			//char* unconst = const_cast<char*>(response.c_str());
			//unsigned char* tt = reinterpret_cast<unsigned char*>(unconst);
			//printf(base64_encode(tt, response.size()).c_str());
			//printf("\n");

			SDLNet_TCP_Send(socket, response.c_str(), response.size());

			isHandshakeDone = true;

			delete[] responseHash;
			onHttpHandshakeDone(this);
		}
		return nullptr;
	}

}


void WebClient::sendMessage(NetworkMessage* msg)
{
	NetworkPacket packet;
	packet.setPayloadFromSerializable(msg);
	/*
	std::cout << "\ntrace of packet d size\n";
		std::cout << Serializer::toHex((char*)&tmp, sizeof(uint16_t));
		std::cout << Serializer::toDec((char*)&tmp, sizeof(uint16_t));
		std::cout << "\n\n";*/

	logSend(msg->getName(), packet);
	
	
	std::cout << "\nraw data before web\n";
		std::cout << Serializer::toHex(packet.rawData, 8);
		std::cout << "\n\n";/**/

	WebNetworkPacket wpacket;
	wpacket.setPayloadFromRawData(packet.rawData, packet.rawSize);

	SDLNet_TCP_Send(socket, wpacket.rawData, wpacket.rawSize);
}

void WebClient::sendMessage(const char* payload, size_t size)
{
	NetworkPacket packet;
	packet.setPayloadFromRawData(payload, size);

	logSend("______", packet);

	WebNetworkPacket wpacket;
	wpacket.setPayloadFromRawData(packet.rawData, packet.rawSize);
	
	SDLNet_TCP_Send(socket, wpacket.rawData, wpacket.rawSize);
}

void WebClient::init()
{
	packetReader = new PacketReader(&socket, getPacket, [this]() {
		return SDLNet_SocketReady(socket); });
	isInitialized = true;
}

std::unique_ptr<NetworkPacket> WebClient::getPacket()
{
	return std::make_unique<WebNetworkPacket>();
}