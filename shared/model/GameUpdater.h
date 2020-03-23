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

	bool isLoaded;

	void update(uint32_t time);
	void load(const GameState& state, Prototypes* prototypes, bool enableEventLogger);
	void addNewInput(std::unique_ptr<InputMessage> input);
	GameState getNewStateByStamp(uint32_t stamp);
	GameState getNewStateBySteps(int32_t steps);
	GameState getFirstState();

private:
	std::vector<std::unique_ptr<InputMessage>> inputs;
	uint32_t lastValidTimeStamp;
	std::vector<InputMessage*> getThisFrameInputs(uint32_t fromInclusive, uint32_t toExclusive);
	std::map<uint32_t, std::unique_ptr<BinarySerializer>, std::greater<uint32_t>> statesByStamps;
	std::map<uint32_t, uint32_t, std::greater<uint32_t>> stampsBySteps;
	void rewindToPrecedingState(uint32_t stamp);
	void saveState(const GameState& state, bool skipCrc = false);
	int32_t lastSavedSteps;
	CRCAccumulator crcs;
}; 