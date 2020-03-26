#include <GameUpdater.h>
#include <EventLogger.h>
#include <algorithm>

//input with time T is not yet applied to the state with stamp T

GameUpdater::GameUpdater()
{
	lastValidTimeStamp = 0;
	isLoaded = false;
	lastSavedSteps = -1;
}

void GameUpdater::update(uint32_t time)
{
	if (lastValidTimeStamp < state.timeStamp)
	{
		rewindToPrecedingState(lastValidTimeStamp);
		lastSavedSteps = state.time.performedSteps;
	}
	int32_t iters = 0;
	while (state.timeStamp + prototypes->variables.fixedStepDuration < time && iters < prototypes->variables.maxLogicUpdatesPerFrame)
	{
		std::vector<InputMessage*> inputs = getThisFrameInputs(state.timeStamp, state.timeStamp + prototypes->variables.fixedStepDuration);
				
		GameLogic::update(&state, prototypes->variables.fixedStepDuration, inputs, prototypes);
		
		
		lastValidTimeStamp = state.timeStamp;
		if (lastSavedSteps != state.time.performedSteps && state.time.performedSteps % S::config.saveStateInterval == 0)
			saveState(state);
		iters++;
		
		if (state.timeStamp % (prototypes->variables.fixedStepDuration*500) == 0)
		{
			S::log.add("crc               " + BinarySerializer::crc(state) + " at " + std::to_string(state.timeStamp));
		}
	}
}

void GameUpdater::load(const GameState& state, Prototypes* prototypes, bool enableEventLogger)
{
	crcs.init(prototypes->variables.fixedStepDuration * S::config.saveStateInterval);
	
	BinarySerializer::copyThroughSerialization(state, this->state);
	lastValidTimeStamp = this->state.timeStamp;
	saveState(this->state, true);
	this->state.isEventLoggerEnabled = enableEventLogger;
	
	this->state.propagatePrototypes(prototypes);

	this->prototypes = prototypes;
	isLoaded = true;	
	
}

void GameUpdater::addNewInput(std::unique_ptr<InputMessage> input)
{
	S::log.add("\naddNewInput: " + 
			std::to_string((int16_t)input->typeId) + 
			"(" + std::to_string(input->serverStamp % 100000) + ")"+
			" at " + std::to_string(state.timeStamp % 100000));
	lastValidTimeStamp = std::min(lastValidTimeStamp, (uint32_t)input.get()->serverStamp);
	inputs.push_back(std::move(input));
}

GameState GameUpdater::getNewStateByStamp(uint32_t stamp)
{
	GameState result;
	BinarySerializer* s = statesByStamps.lower_bound(stamp)->second.get();
	s->read(result);
	s->resetCursors();
	result.propagatePrototypes(prototypes);

	while (result.timeStamp < stamp - prototypes->variables.fixedStepDuration)
	{
		std::vector<InputMessage*> inputs = getThisFrameInputs(result.timeStamp, result.timeStamp + prototypes->variables.fixedStepDuration);
		GameLogic::update(&result, prototypes->variables.fixedStepDuration, inputs, prototypes);
	}

	return result;
}

GameState GameUpdater::getNewStateBySteps(int32_t steps)
{
	GameState result;
	
	uint32_t stamp = stampsBySteps.lower_bound(steps)->second;
	auto streamRef = statesByStamps.find(stamp);
	if (streamRef == statesByStamps.end())
	{
		result = false;
	}
	else
	{
		BinarySerializer* s = streamRef->second.get();
		s->read(result);
		s->resetCursors();
		result.propagatePrototypes(prototypes);

		while (result.time.performedSteps < steps && result.timeStamp < state.timeStamp)
		{
			std::vector<InputMessage*> inputs = getThisFrameInputs(result.timeStamp, result.timeStamp + prototypes->variables.fixedStepDuration);
			GameLogic::update(&result, prototypes->variables.fixedStepDuration, inputs, prototypes);
		}
	}	

	return result;
}

GameState GameUpdater::getFirstState()
{
	GameState result;
	 
	BinarySerializer* s = std::min_element(statesByStamps.begin(), statesByStamps.end())->second.get();
	s->read(result);
	s->resetCursors();
	result.propagatePrototypes(prototypes);

	return result;
}

std::vector<InputMessage*> GameUpdater::getThisFrameInputs(uint32_t fromInclusive, uint32_t toExclusive)
{
	std::vector<InputMessage*> result;

	for (size_t i = 0; i < inputs.size(); i++)
	{
		uint32_t execTime = inputs[i].get()->serverStamp;
		if (execTime >= fromInclusive && execTime < toExclusive)
		{
			//S::log.add("this frame inputs: " + S::crc(*state));
			result.push_back(inputs[i].get());
		}
	}
	return result;
}

void GameUpdater::rewindToPrecedingState(uint32_t stamp)
{
	auto from = state.timeStamp;
	
	BinarySerializer* s = statesByStamps.lower_bound(stamp)->second.get();
	
	if (s == nullptr)
		THROW_FATAL_ERROR("Unable to rewind game state");
	s->read(state);
	s->resetCursors();
	state.propagatePrototypes(prototypes);
	S::log.add("\nrewind from " + std::to_string(from % 100000) + 
			" to " + std::to_string(state.timeStamp % 100000) + "\n");
}

void GameUpdater::saveState(GameState& state, bool skipCrc)
{
	auto stream = std::make_unique<BinarySerializer>();
	stream->write(state);
	stream->resetCursors();
	
	if (!skipCrc)
		crcs.add(state.timeStamp, stream->crc());
	
	statesByStamps[state.timeStamp] = std::move(stream);
	stampsBySteps[state.time.performedSteps] = state.timeStamp;
	lastSavedSteps = state.time.performedSteps;
	
	if (statesByStamps.size() > S::config.maxSaveStates * 2)
	{
		auto min = std::min_element(statesByStamps.begin(), statesByStamps.end());
		auto cutOffPoint = state.timeStamp - S::config.maxSaveStates * S::config.saveStateInterval * prototypes->variables.fixedStepDuration;
		decltype(statesByStamps) newStatesByStamps;
		for(auto i = statesByStamps.begin(); i != statesByStamps.end(); i++)
		{
			if (i->first > cutOffPoint || i == min)
				newStatesByStamps[i->first] = std::move(i->second);
		}
		statesByStamps = std::move(newStatesByStamps);
	}
}

std::optional<GameState> GameUpdater::getSavedStateByStamp(uint32_t stamp)
{
	auto streamRef = statesByStamps.find(stamp);
	if (streamRef == statesByStamps.end())
		return {};
		
	std::optional<GameState> result = GameState();
	
	BinarySerializer* s = streamRef->second.get();
	s->read(result.value());
	s->resetCursors();

	return result;
}
