#include <GameUpdater.h>
#include <EventLogger.h>
#include <algorithm>

//input with time T is not yet applied to the state with stamp T

GameUpdater::GameUpdater()
{
	m_lastValidTimeStamp = 0;
	m_isLoaded = false;
	m_lastSavedSteps = -1;
}

void GameUpdater::update(uint32_t time)
{
	m_lastFrameInputs.clear();
	if (m_lastValidTimeStamp < state.timeStamp)
	{
		rewindToPrecedingState(m_lastValidTimeStamp);
		m_lastSavedSteps = state.time.performedSteps;
	}
	int32_t iters = 0;
	while (state.timeStamp + prototypes->variables.fixedStepDuration < time && iters < prototypes->variables.maxLogicUpdatesPerFrame)
	{
		std::vector<InputMessage*> inputs = getThisFrameInputs(state.timeStamp, state.timeStamp + prototypes->variables.fixedStepDuration);
		for(auto& i : inputs)
			m_lastFrameInputs.push_back(i);
		
		GameLogic::update(&state, prototypes->variables.fixedStepDuration, inputs, prototypes);
		
		
		m_lastValidTimeStamp = state.timeStamp;
		if (m_lastSavedSteps != state.time.performedSteps && state.time.performedSteps % S::config.saveStateInterval == 0)
			saveState(state);
		iters++;
	}
}

void GameUpdater::load(const GameState& state, Prototypes* prototypes, bool enableEventLogger)
{
	crcs.init(prototypes->variables.fixedStepDuration * S::config.saveStateInterval);
	
	BinarySerializer::copyThroughSerialization(state, this->state);
	m_lastValidTimeStamp = this->state.timeStamp;
	saveState(this->state, true);
	this->state.isEventLoggerEnabled = enableEventLogger;
	
	this->state.propagatePrototypes(prototypes);

	this->prototypes = prototypes;
	m_isLoaded = true;	
	
}

void GameUpdater::addNewInput(std::unique_ptr<InputMessage> input)
{
	m_lastValidTimeStamp = std::min(m_lastValidTimeStamp, (uint32_t)input.get()->serverStamp);
	m_inputs.push_back(std::move(input));
}

GameState GameUpdater::getNewStateByStamp(uint32_t stamp)
{
	GameState result;
	BinarySerializer* s = m_statesByStamps.lower_bound(stamp)->second.get();
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
	
	uint32_t stamp = m_stampsBySteps.lower_bound(steps)->second;
	auto streamRef = m_statesByStamps.find(stamp);
	if (streamRef == m_statesByStamps.end())
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
	 
	BinarySerializer* s = std::min_element(m_statesByStamps.begin(), m_statesByStamps.end())->second.get();
	s->read(result);
	s->resetCursors();
	result.propagatePrototypes(prototypes);

	return result;
}

std::vector<InputMessage*> GameUpdater::getThisFrameInputs(uint32_t fromInclusive, uint32_t toExclusive)
{
	std::vector<InputMessage*> result;

	for (size_t i = 0; i < m_inputs.size(); i++)
	{
		uint32_t execTime = m_inputs[i].get()->serverStamp;
		if (execTime >= fromInclusive && execTime < toExclusive)
		{
			//S::log.add("this frame inputs: " + S::crc(*state));
			result.push_back(m_inputs[i].get());
		}
	}
	return result;
}

void GameUpdater::rewindToPrecedingState(uint32_t stamp)
{
	auto from = state.timeStamp;
	
	BinarySerializer* s = m_statesByStamps.lower_bound(stamp)->second.get();
	
	if (s == nullptr)
		THROW_FATAL_ERROR("Unable to rewind game state");
	s->read(state);
	s->resetCursors();
	state.propagatePrototypes(prototypes);
	S::log.add("\nrewind from " + std::to_string(from % 100000) + 
			" to " + std::to_string(state.timeStamp % 100000) + "\n", {LOG_TAGS::NET);
}

void GameUpdater::saveState(GameState& state, bool skipCrc)
{
	auto stream = std::make_unique<BinarySerializer>();
	stream->write(state);
	stream->resetCursors();
	
	if (!skipCrc)
		crcs.add(state.timeStamp, stream->crc());
	
	m_statesByStamps[state.timeStamp] = std::move(stream);
	m_stampsBySteps[state.time.performedSteps] = state.timeStamp;
	m_lastSavedSteps = state.time.performedSteps;
	
	if (m_statesByStamps.size() > S::config.maxSaveStates * 2)
	{
		auto min = std::min_element(m_statesByStamps.begin(), m_statesByStamps.end());
		auto cutOffPoint = state.timeStamp - S::config.maxSaveStates * S::config.saveStateInterval * prototypes->variables.fixedStepDuration;
		decltype(m_statesByStamps) newStatesByStamps;
		for(auto i = m_statesByStamps.begin(); i != m_statesByStamps.end(); i++)
		{
			if (i->first > cutOffPoint || i == min)
				newStatesByStamps[i->first] = std::move(i->second);
		}
		m_statesByStamps = std::move(newStatesByStamps);
	}
}

std::optional<GameState> GameUpdater::getSavedStateByStamp(uint32_t stamp)
{
	auto streamRef = m_statesByStamps.find(stamp);
	if (streamRef == m_statesByStamps.end())
		return {};
		
	std::optional<GameState> result = GameState();
	
	BinarySerializer* s = streamRef->second.get();
	s->read(result.value());
	s->resetCursors();

	return result;
}

std::vector<InputMessage*> GameUpdater::getLastFrameInputs()
{
	return m_lastFrameInputs;
}
