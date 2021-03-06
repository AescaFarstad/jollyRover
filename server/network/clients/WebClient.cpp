#include <WebClient.h>
#include <libwshandshake.hpp>
#include <regex>
#include <algorithm> 
#include <cctype>
#include <locale>

#include <WebNetworkPacket.h>
#include <Global.h>

WebClient::WebClient(
	std::function<void(NetworkClient& client)> onHttpHandshakeDone, 
	std::function< int32_t() > globalSocketNudgeFunction) : NetworkClient(globalSocketNudgeFunction)
{
	m_onHttpHandshakeDone = onHttpHandshakeDone;
	m_isHandshakeDone = false;
}


void ltrim(std::string& s) 
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int32_t ch) {
		return !std::isspace(ch);
	}));
}
void rtrim(std::string& s) 
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int32_t ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}
void trim(std::string& s) 
{
	ltrim(s);
	rtrim(s);
}

std::unique_ptr<NetworkPacket> WebClient::poll()
{
	if (m_isHandshakeDone)
	{
		return NetworkClient::poll();
	}
	else
	{
		char buffer[S::config.maxMessageSize];
		int32_t bytes = SDLNet_TCP_Recv(socket, buffer, S::config.maxMessageSize);
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

			SDLNet_TCP_Send(socket, response.c_str(), response.size());

			m_isHandshakeDone = true;

			delete[] responseHash;
			m_onHttpHandshakeDone(*this);
		}
		return nullptr;
	}

}


void WebClient::sendMessage(const NetworkMessage& msg)
{
	NetworkPacket packet;
	packet.setPayloadFromSerializable(msg);

	logSend(msg.getName(), packet);

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
	packetReader = std::make_unique<PacketReader>(&socket, getPacket, [this]() {
		return SDLNet_SocketReady(socket); });
	isInitialized = true;
}

std::unique_ptr<NetworkPacket> WebClient::getPacket()
{
	return std::make_unique<WebNetworkPacket>();
}