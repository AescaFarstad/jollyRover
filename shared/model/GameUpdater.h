#pragma once
#include <GameLogic.h>
#include <GameState.h>
#include <InputMessage.h>
#include <Prototypes.h>
#include <memory>
#include <vector>
#include <map>
#include <functional>

class GameUpdater
{
public:
	GameUpdater();
	~GameUpdater();

	std::unique_ptr<GameState> state;

	GameLogic logic;
	bool isLoaded;

	void update(uint32_t time);
	void load(std::unique_ptr<GameState> state, Prototypes* prototypes);
	void addNewInput(std::unique_ptr<InputMessage> input);
	std::unique_ptr<GameState> getNewStateByStamp(uint32_t stamp);

private:
	std::vector<std::unique_ptr<InputMessage>> inputs;
	uint32_t lastValidTimeStamp;
	uint32_t getExecutionStamp(InputMessage* input);
	std::vector<InputMessage*>* getThisFrameInputs(uint32_t fromInclusive, uint32_t toExclusive);
	std::map<uint32_t, std::unique_ptr<SerializationStream>, std::greater<uint32_t>> states;
	void rewindToPrecedingState(uint32_t stamp);
	void saveState(GameState* state);
	Prototypes* prototypes;
}; 