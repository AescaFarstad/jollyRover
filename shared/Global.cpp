#include <Global.h>
namespace S
{
	Logger log;
	int32_t idCounter = 0;
	int32_t getId()
	{
		return idCounter++;
	}
	GameConfig config;
}

void GameConfig::load(nlohmann::json& j)
{
	nlohmann::json jconfig = j.at("globalConfig");
	if (S::config.IS_WEB)	
		S::config.port = jconfig.at("webPort");
	else
		S::config.port = jconfig.at("desktopPort");
	S::config.webPort = jconfig.at("webPort");
	S::config.saveStateInterval = jconfig.at("saveStateInterval");
	S::config.networkUpdateInterval = jconfig.at("networkUpdateInterval");
	S::config.maxMessageSize = jconfig.at("maxMessageSize");
	std::string host = jconfig.at("host");
	memcpy(S::config.host, host.c_str(), host.length() + 1);
}