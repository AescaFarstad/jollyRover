#pragma once
#include <GameLogic.h>
#include <GameState.h>
#include <InputMessage.h>
#include <LoadGameMessage.h>
#include <Prototypes.h>
#include <BinarySerializer.h>
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include <CRCAccumulator.h>

class GameUpdater
{
public:
	GameUpdater();

	GameState state;
	Prototypes* prototypes;
	CRCAccumulator crcs;


	void update(uint32_t time);
	void load(const GameState& state, Prototypes* prototypes, bool enableEventLogger);
	void addNewInput(std::unique_ptr<InputMessage> input);
	GameState getNewStateByStamp(uint32_t stamp);
	GameState getNewStateBySteps(int32_t steps);
	std::optional<GameState> getSavedStateByStamp(uint32_t stamp);
	GameState getFirstState();
	std::vector<InputMessage*> getLastFrameInputs();

private:
	std::vector<std::unique_ptr<InputMessage>> m_inputs;
	uint32_t m_lastValidTimeStamp;
	std::map<uint32_t, std::unique_ptr<BinarySerializer>, std::greater<uint32_t>> m_statesByStamps;
	std::map<uint32_t, uint32_t, std::greater<uint32_t>> m_stampsBySteps;
	int32_t m_lastSavedSteps;
	bool m_isLoaded;
	std::vector<InputMessage*> m_lastFrameInputs;
	
	
	std::vector<InputMessage*> getThisFrameInputs(uint32_t fromInclusive, uint32_t toExclusive);
	void rewindToPrecedingState(uint32_t stamp);
	void saveState(GameState& state, bool skipCrc = false);
}; 