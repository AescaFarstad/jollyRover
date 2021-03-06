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
	int debug = 0;
	bool isLoaded = false;
}


void GameConfig::load(nlohmann::json& j)
{
	nlohmann::json jconfig = j.at("globalConfig");
	if (S::config.IS_WEB)	
		S::config.port = jconfig.at("webPort");
	else
		S::config.port = jconfig.at("desktopPort");
	S::config.desyncMode = jconfig.at("desyncMode");
	S::config.enableHardLog = jconfig.at("enableHardLog");
	S::config.hardLogPath = jconfig.at("hardLogPath");
	S::config.demoPath = jconfig.at("demoPath");
	S::config.sendFailedInput = jconfig.at("sendFailedInput");
	S::config.webPort = jconfig.at("webPort");
	S::config.saveStateInterval = jconfig.at("saveStateInterval");
	if (S::config.desyncMode)
		S::config.saveStateInterval = 1;
	S::config.networkUpdateInterval = jconfig.at("networkUpdateInterval");
	S::config.maxMessageSize = jconfig.at("maxMessageSize");
	std::string host = jconfig.at("host");
	std::memcpy(S::config.host, host.c_str(), host.length() + 1);
	S::config.loopBack = jconfig.at("loopBack");
	S::config.runBenchmark = jconfig.at("runBenchmark");
	S::config.window_X = jconfig.at("windowLocation").at("x");
	S::config.window_Y = jconfig.at("windowLocation").at("y");
	S::config.maxSaveStates = jconfig.at("maxSaveStates");
	loadLogTags(j);
}

void GameConfig::loadLogTags(nlohmann::json& j)
{
	auto disabled = j.at("logTags").at("disable").get<std::vector<std::string>>();
	auto enabled = j.at("logTags").at("enable").get<std::vector<std::string>>();
	
	for(auto& iter : disabled)
		setTagByName(iter, false);
	for(auto& iter : enabled)
		setTagByName(iter, true);
}

void GameConfig::setTagByName(std::string name, bool value)
{
	if (name == "UNIQUE")
		setTagByTag(LOG_TAGS::UNIQUE, value);
	else if (name == "ERROR_")
		setTagByTag(LOG_TAGS::ERROR_, value);
	else if (name == "ANONYMOS")
		setTagByTag(LOG_TAGS::ANONYMOS, value);
		
	else if (name == "NET")
		setTagByTag(LOG_TAGS::NET, value);
	else if (name == "GAME")
		setTagByTag(LOG_TAGS::GAME, value);
	else if (name == "TASK")
		setTagByTag(LOG_TAGS::TASK, value);
	else if (name == "INPUT_")
		setTagByTag(LOG_TAGS::INPUT_, value);
		
	else if (name == "NET_MESSAGE")
		setTagByTag(LOG_TAGS::NET_MESSAGE, value);
	else if (name == "NET_BRIEF")
		setTagByTag(LOG_TAGS::NET_BRIEF, value);
	else if (name == "SUBTASK")
		setTagByTag(LOG_TAGS::SUBTASK, value);
		
	else if (name == "Z")
		setTagByTag(LOG_TAGS::Z, value);
}

void GameConfig::setTagByTag(LOG_TAGS tag, bool value)
{
	if (value)
		S::log.enableTags({tag});
	else
		S::log.disableTags({tag});	
}
