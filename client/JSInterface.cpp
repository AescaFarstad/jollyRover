#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>

#include <Global.h>
#include <DrawSettings.h>

auto& globalLog = S::log;
auto& drawSettings = S::drawSettings;
auto& isGameLoaded = S::isLoaded;


extern "C" {

	void EMSCRIPTEN_KEEPALIVE toggleLogSetting(int32_t index) 
	{
		globalLog.add("toggleLogSetting " + std::to_string(index), {LOG_TAGS::UNIQUE});
		switch (index)
		{
			case 0: globalLog.toggleTag(LOG_TAGS::UNIQUE); break;
			case 1: globalLog.toggleTag(LOG_TAGS::ERROR_); break;
			case 2: globalLog.toggleTag(LOG_TAGS::ANONYMOS); break;
			case 3: globalLog.toggleTag(LOG_TAGS::NET); break;
			case 4: globalLog.toggleTag(LOG_TAGS::GAME); break;
			case 5: globalLog.toggleTag(LOG_TAGS::TASK); break;
			case 6: globalLog.toggleTag(LOG_TAGS::INPUT_); break;
			case 7: globalLog.toggleTag(LOG_TAGS::NET_MESSAGE); break;
			case 8: globalLog.toggleTag(LOG_TAGS::NET_BRIEF); break;
			case 9: globalLog.toggleTag(LOG_TAGS::SUBTASK); break;
			case 10: globalLog.toggleTag(LOG_TAGS::FILE_SYSTEM); break;
			case 11: globalLog.toggleTag(LOG_TAGS::HARD_LOG); break;
			case 12: globalLog.toggleTag(LOG_TAGS::Z); break;
			
			default:
				globalLog.add("Wrong log type index: " + std::to_string(index), {LOG_TAGS::ERROR_});
				break;
		}
	}
	
	int8_t* EMSCRIPTEN_KEEPALIVE getLogSettings(int8_t *buf, int32_t bufSize) 
	{
		int8_t result[13];
		
		result[0] = globalLog.isEnabled(LOG_TAGS::UNIQUE) ? 1 : 0;
		result[1] = globalLog.isEnabled(LOG_TAGS::ERROR_) ? 1 : 0;
		result[2] = globalLog.isEnabled(LOG_TAGS::ANONYMOS) ? 1 : 0;
		result[3] = globalLog.isEnabled(LOG_TAGS::NET) ? 1 : 0;
		result[4] = globalLog.isEnabled(LOG_TAGS::GAME) ? 1 : 0;
		result[5] = globalLog.isEnabled(LOG_TAGS::TASK) ? 1 : 0;
		result[6] = globalLog.isEnabled(LOG_TAGS::INPUT_) ? 1 : 0;
		result[7] = globalLog.isEnabled(LOG_TAGS::NET_MESSAGE) ? 1 : 0;
		result[8] = globalLog.isEnabled(LOG_TAGS::NET_BRIEF) ? 1 : 0;
		result[9] = globalLog.isEnabled(LOG_TAGS::SUBTASK) ? 1 : 0;
		result[10] = globalLog.isEnabled(LOG_TAGS::FILE_SYSTEM) ? 1 : 0;
		result[11] = globalLog.isEnabled(LOG_TAGS::HARD_LOG) ? 1 : 0;
		result[12] = globalLog.isEnabled(LOG_TAGS::Z) ? 1 : 0;
		
		globalLog.add("getLogSettings " + 
				std::to_string(result[0]) + "," +
				std::to_string(result[1]) + "," +
				std::to_string(result[2]) + "," +
				std::to_string(result[3]) + "," +
				std::to_string(result[4]) + "," +
				std::to_string(result[5]) + "," +
				std::to_string(result[6]) + "," +
				std::to_string(result[7]) + "," +
				std::to_string(result[8]) + "," +
				std::to_string(result[9]) + "," +
				std::to_string(result[10]) + "," + 
				std::to_string(result[11]) + "," + 
				std::to_string(result[12]) + ",", 
				{LOG_TAGS::UNIQUE});
		
		return &result[0]; //This is OK
	}
	
	void EMSCRIPTEN_KEEPALIVE toggleDrawSetting(int32_t index) 
	{
		globalLog.add("toggleDrawSetting " + std::to_string(index), {LOG_TAGS::UNIQUE});
		switch (index)
		{
			case 0: drawSettings.layer1 = !drawSettings.layer1; break;
			case 1: drawSettings.remnants = !drawSettings.remnants; break;
			case 2: drawSettings.layer2 = !drawSettings.layer2; break;
			case 3: drawSettings.creeps = !drawSettings.creeps; break;
			case 4: drawSettings.cars = !drawSettings.cars; break;
			case 5: drawSettings.projectiles = !drawSettings.projectiles; break;
			case 6: drawSettings.layer3 = !drawSettings.layer3; break;
			case 7: drawSettings.formations_D = !drawSettings.formations_D; break;
			case 8: drawSettings.connections_D = !drawSettings.connections_D; break;
			case 9: drawSettings.formAgro_D = !drawSettings.formAgro_D; break;
			case 10: drawSettings.explosions = !drawSettings.explosions; break;
			case 11: drawSettings.debug_D = !drawSettings.debug_D; break;
			case 12: drawSettings.threat_D = !drawSettings.threat_D; break;
			case 13: drawSettings.obstacles_D = !drawSettings.obstacles_D; break;
			case 14: drawSettings.input = !drawSettings.input; break;
			case 15: drawSettings.hud = !drawSettings.hud; break;
			case 16: drawSettings.flyingMessage = !drawSettings.flyingMessage; break;
			case 17: drawSettings.fps_D = !drawSettings.fps_D; break;
			case 18: drawSettings.timing_D = !drawSettings.timing_D; break;
			case 19: drawSettings.scores = !drawSettings.scores; break;
			
			default:
				globalLog.add("Wrong draw setting index: " + std::to_string(index), {LOG_TAGS::ERROR_});
				break;
		}
	}
	
	int8_t* EMSCRIPTEN_KEEPALIVE getDrawSettings(int8_t *buf, int32_t bufSize) 
	{
		int8_t result[20];
		result[0] = drawSettings.layer1 ? 1 : 0;
		result[1] = drawSettings.remnants ? 1 : 0;
		result[2] = drawSettings.layer2 ? 1 : 0;
		result[3] = drawSettings.creeps ? 1 : 0;
		result[4] = drawSettings.cars ? 1 : 0;
		result[5] = drawSettings.projectiles ? 1 : 0;
		result[6] = drawSettings.layer3 ? 1 : 0;
		result[7] = drawSettings.formations_D ? 1 : 0;
		result[8] = drawSettings.connections_D ? 1 : 0;
		result[9] = drawSettings.formAgro_D ? 1 : 0;
		result[10] = drawSettings.explosions ? 1 : 0;
		result[11] = drawSettings.debug_D ? 1 : 0;
		result[12] = drawSettings.threat_D ? 1 : 0;
		result[13] = drawSettings.obstacles_D ? 1 : 0;
		result[14] = drawSettings.input ? 1 : 0;
		result[15] = drawSettings.hud ? 1 : 0;
		result[16] = drawSettings.flyingMessage ? 1 : 0;
		result[17] = drawSettings.fps_D ? 1 : 0;
		result[18] = drawSettings.timing_D ? 1 : 0;
		result[19] = drawSettings.scores ? 1 : 0;
		
		globalLog.add("getDrawSettings " + 
				std::to_string(result[0]) + "," +
				std::to_string(result[1]) + "," +
				std::to_string(result[2]) + "," +
				std::to_string(result[3]) + "," +
				std::to_string(result[4]) + "," +
				std::to_string(result[5]) + "," +
				std::to_string(result[6]) + "," +
				std::to_string(result[7]) + "," +
				std::to_string(result[8]) + "," +
				std::to_string(result[9]) + "," +
				std::to_string(result[10]) + "," +
				std::to_string(result[11]) + "," +
				std::to_string(result[12]) + "," +
				std::to_string(result[13]) + "," +
				std::to_string(result[14]) + "," +
				std::to_string(result[15]) + "," +
				std::to_string(result[16]) + "," +
				std::to_string(result[17]) + "," + 
				std::to_string(result[18]) + "," + 
				std::to_string(result[19]) + ",", 
				{LOG_TAGS::UNIQUE});
		
		return &result[0]; // this is OK
	}
	
	bool EMSCRIPTEN_KEEPALIVE isLoaded() 
	{
		return isGameLoaded;
	}
	 
}

#endif