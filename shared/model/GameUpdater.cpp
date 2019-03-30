#include <GameUpdater.h>
#include <algorithm>

//input with time T is not yet applied to state with stamp T

GameUpdater::GameUpdater()
{
	lastValidTimeStamp = 0;
	isLoaded = false;
}


void GameUpdater::update(uint32_t time)
{
	if (lastValidTimeStamp < state->timeStamp)
		rewindToPrecedingState(lastValidTimeStamp);

	while (state->timeStamp < time - prototypes->variables.fixedStepDuration)
	{		
		std::vector<InputMessage*>* inputs = getThisFrameInputs(state->timeStamp, state->timeStamp + prototypes->variables.fixedStepDuration);
		logic.update(state.get(), prototypes->variables.fixedStepDuration, *inputs, prototypes);
		lastValidTimeStamp = state->timeStamp;
		if (state->time.performedSteps % S::config.saveStateInterval == 0)
			saveState(state.get());
		delete inputs;
	}
}

void GameUpdater::load(std::unique_ptr<GameState> state, Prototypes* prototypes)
{
	this->state = std::move(state);
	lastValidTimeStamp = this->state->timeStamp;
	saveState(this->state.get());
	this->state->propagatePrototypes(prototypes);

	this->prototypes = prototypes;

	isLoaded = true;
}

void GameUpdater::addNewInput(std::unique_ptr<InputMessage> input)
{
	lastValidTimeStamp = std::min(lastValidTimeStamp, getExecutionStamp(input.get()));
	inputs.push_back(std::move(input));
}

std::unique_ptr<GameState> GameUpdater::getNewStateByStamp(uint32_t stamp)
{
	auto result = std::make_unique<GameState>();
	SerializationStream* s = statesByStamps.lower_bound(stamp)->second.get();
	result->deserialize(*s);
	result->propagatePrototypes(prototypes);

	while (result->timeStamp < stamp - prototypes->variables.fixedStepDuration)
	{
		std::vector<InputMessage*>* inputs = getThisFrameInputs(result->timeStamp, result->timeStamp + prototypes->variables.fixedStepDuration);
		logic.update(result.get(), prototypes->variables.fixedStepDuration, *inputs, prototypes);
		delete inputs;
	}

	return result;
}

std::unique_ptr<GameState> GameUpdater::getNewStateBySteps(int32_t steps)
{
	auto result = std::make_unique<GameState>();
	
	uint32_t stamp = stampsBySteps.lower_bound(steps)->second;
	SerializationStream* s = statesByStamps.at(stamp).get();
	result->deserialize(*s);
	s->seekAbsolute(0);
	result->propagatePrototypes(prototypes);

	while (result->time.performedSteps < steps && result->timeStamp < state->timeStamp)
	{
		std::vector<InputMessage*>* inputs = getThisFrameInputs(result->timeStamp, result->timeStamp + prototypes->variables.fixedStepDuration);
		logic.update(result.get(), prototypes->variables.fixedStepDuration, *inputs, prototypes);
		delete inputs;
	}

	return result;
}

std::unique_ptr<GameState> GameUpdater::getFirstState()
{
	auto result = std::make_unique<GameState>();
	 
	SerializationStream* s = std::min_element(statesByStamps.begin(), statesByStamps.end())->second.get();
	result->deserialize(*s);
	s->seekAbsolute(0);
	result->propagatePrototypes(prototypes);

	return result;
}

uint32_t GameUpdater::getExecutionStamp(InputMessage* input)
{
	return input->serverStamp;
}

std::vector<InputMessage*>* GameUpdater::getThisFrameInputs(uint32_t fromInclusive, uint32_t toExclusive)
{
	std::vector<InputMessage*>* result = new std::vector<InputMessage*>();

	for (size_t i = 0; i < inputs.size(); i++)
	{
		uint32_t execTime = getExecutionStamp(inputs[i].get());
		if (execTime >= fromInclusive && execTime < toExclusive)
		{
			result->push_back(inputs[i].get());
		}
	}
	return result;
}

void GameUpdater::rewindToPrecedingState(uint32_t stamp)
{
	SerializationStream* s = statesByStamps.lower_bound(stamp)->second.get();
	state->deserialize(*s);
	state->propagatePrototypes(prototypes);
	s->seekAbsolute(0);
}

void GameUpdater::saveState(GameState* state)
{
	auto stream = SerializationStream::createExp();
	state->serialize(*stream.get());
	stream->seekAbsolute(0);
	statesByStamps[state->timeStamp] = std::move(stream);
	stampsBySteps[state->time.performedSteps] = state->timeStamp;
}
