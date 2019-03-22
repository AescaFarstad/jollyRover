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

	while (state->timeStamp < time - GAME_CONFIG::logicStepSize)
	{
		if (loadGameMsg)
		{
			int32_t execTime = (int32_t)getExecutionStamp(loadGameMsg.get());
			if (loadGameMsg->serverStamp <= execTime && loadGameMsg->serverStamp + GAME_CONFIG::logicStepSize > execTime)
			{
				auto stream = SerializationStream::createExp();
				auto stamp = state->timeStamp;
				TimeState time = state->time;
				stream->write(loadGameMsg->state, loadGameMsg->stateLength); 
				stream->seekAbsolute(0);
				state->deserialize(*stream);
				state->propagatePrototypes(prototypes);
				loadGameMsg = nullptr;
				state->timeStamp = stamp;
				state->time.timeScale = time.timeScale;
				if (time.allowedSteps > 0)
					state->time.allowedSteps = time.allowedSteps - time.performedSteps + state->time.performedSteps;
				
				
				//TODO resolve players joining and quiting
			}			
		}
		
		std::vector<InputMessage*>* inputs = getThisFrameInputs(state->timeStamp, state->timeStamp + GAME_CONFIG::logicStepSize);
		logic.update(state.get(), GAME_CONFIG::logicStepSize, *inputs, prototypes);
		lastValidTimeStamp = state->timeStamp;
		if (state->time.performedSteps % GAME_CONFIG::saveStateInterval == 0)
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
	
	if (input->typeId != MessageTypes::TYPE_LOAD_GAME_MSG)
		inputs.push_back(std::move(input));
	else
		loadGameMsg = std::unique_ptr<LoadGameMessage>(dynamic_cast<LoadGameMessage*>(input.release()));
}

std::unique_ptr<GameState> GameUpdater::getNewStateByStamp(uint32_t stamp)
{
	auto result = std::make_unique<GameState>();
	SerializationStream* s = statesByStamps.lower_bound(stamp)->second.get();
	result->deserialize(*s);
	result->propagatePrototypes(prototypes);

	while (result->timeStamp < stamp - GAME_CONFIG::logicStepSize)
	{
		std::vector<InputMessage*>* inputs = getThisFrameInputs(result->timeStamp, result->timeStamp + GAME_CONFIG::logicStepSize);
		logic.update(result.get(), GAME_CONFIG::logicStepSize, *inputs, prototypes);
		delete inputs;
	}

	return result;
}

std::unique_ptr<GameState> GameUpdater::getNewStateBySteps(int32_t steps)
{
	auto result = std::make_unique<GameState>();
	using IterType = decltype(stepsByStamps.begin());
	using IterType2 = decltype(*stepsByStamps.begin());
	uint32_t stamp = std::min_element(stepsByStamps.begin(), stepsByStamps.end(), [](IterType2& a, IterType2& b){
			return a < b;
		})->second;
	SerializationStream* s = statesByStamps.at(stamp).get();
	result->deserialize(*s);
	s->seekAbsolute(0);
	result->propagatePrototypes(prototypes);

	while (result->time.performedSteps < steps && result->timeStamp < state->timeStamp)
	{
		std::vector<InputMessage*>* inputs = getThisFrameInputs(result->timeStamp, result->timeStamp + GAME_CONFIG::logicStepSize);
		logic.update(result.get(), GAME_CONFIG::logicStepSize, *inputs, prototypes);
		delete inputs;
	}

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
	stepsByStamps[state->timeStamp] = state->time.performedSteps;
}
