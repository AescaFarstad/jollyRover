#include <GameLogic.h>
#include <Keyboard.h>
#include <Creeps.h>
#include <InputTimeMessage.h>
#include <VisualDebug.h>
#include <GameKeyboardActions.h>
#include <AI.h>
#include <std2.h>


namespace GameLogic
{		
	void update(GameState* state, int32_t timePassed, std::vector<InputMessage*> &inputs, Prototypes* prototypes)
	{
		if (state->timeStamp < 20)
		{
			
		}
		
		
		state->timeStamp += timePassed;
		
		for (size_t i = 0; i < inputs.size(); i++)
		{
			if (inputs[i]->typeId == MessageTypes::TYPE_INPUT_ACTION_MSG)
				GameLogicInternal::handleActionInput(state, static_cast<InputActionMessage*>(inputs[i]));
			else if (inputs[i]->typeId == MessageTypes::TYPE_INPUT_JOINED_MSG)
				GameLogicInternal::handlePlayerJoinedInput(state, static_cast<InputPlayerJoinedMessage*>(inputs[i]));
			else if (inputs[i]->typeId == MessageTypes::TYPE_INPUT_LEFT_MSG)
				GameLogicInternal::handlePlayerLeftInput(state, static_cast<InputPlayerLeftMessage*>(inputs[i]));
			else if (inputs[i]->typeId == MessageTypes::TYPE_INPUT_ROUTE_MSG)
				GameLogicInternal::handleRouteInput(state, static_cast<InputRouteMessage*>(inputs[i]), prototypes);
			else if (inputs[i]->typeId == MessageTypes::TYPE_INPUT_TIME_MSG)
				GameLogicInternal::handleTimeInput(state, static_cast<InputTimeMessage*>(inputs[i]), prototypes);
			else if (inputs[i]->typeId == MessageTypes::TYPE_LOAD_GAME_MSG)
				GameLogicInternal::handleGameLoad(state, static_cast<LoadGameMessage*>(inputs[i]), prototypes);
			else if (inputs[i]->typeId == MessageTypes::TYPE_INPUT_IMPULSE_MSG)
				GameLogicInternal::handleInputImpulse(state, static_cast<InputImpulseMessage*>(inputs[i]), prototypes);
			//S::log.add(std::to_string(prototypes->variables.fieldWidth), { LOG_TAGS::UNIQUE });
		}
		
		TimeState& time = state->time;
		float timeScale = time.forcedTimeScale > 0 ? time.forcedTimeScale : time.timeScale;
		int32_t ingameTimePassed = timePassed * timeScale;
		bool isPaused = (time.allowedSteps >= 0 && time.allowedSteps <= time.performedSteps) && time.forcedTimeScale <= 0;
		
		if (isPaused)
		{
			return;
		}
		else
		{
			int32_t stepsAtOnce = state->time.forcedStepsAtOnce > 0 ? state->time.forcedStepsAtOnce : state->time.stepsAtOnce;
			for(int32_t i = 0; i < stepsAtOnce; i++)
			{
				if (time.forcedTimeScale > 0 && time.allowedSteps >= 0)
					time.allowedSteps++;
				state->time.performedSteps++;
				VisualDebug::clear();
				GameLogicInternal::makeLogicStep(state, ingameTimePassed, prototypes);
			}
		}
	}

	bool testRouteIsValid(std::vector<Point>& route, Prototypes* prototypes)
	{
		if (route.size() < (size_t)prototypes->variables.minRouteSteps || 
			route.size() > (size_t)prototypes->variables.maxRouteSteps)
		{
			return false;
		}
		if (route.front().y != prototypes->variables.fieldHeight ||
			route.back().y != prototypes->variables.fieldHeight)
		{
			return false;
		}

		for (size_t i = 1; i < route.size(); i++)
		{
			if (fabsf(route[i - 1].distanceTo(route[i]) - prototypes->variables.routeStepSize) > 10 *FMath::EPSILON)
			{
				if (i != route.size() - 1 || route[i - 1].distanceTo(route[i]) > prototypes->variables.routeStepSize + 10 *FMath::EPSILON)
				{
					//float ds = route[i - 1].distanceTo(route[i]);
					//float max = prototypes->variables.routeStepSize + FMath::EPSILON;
					//float abs = std::fabsf(route[i - 1].distanceTo(route[i]) - prototypes->variables.routeStepSize);
					S::log.add("INVALID step distance");
					return false;
				}
			}
			if (!testEdgeIsValid(route[i - 1], route[i], prototypes))
			{
				return false;
			}
		}
		//TODO check angles
		return true;
	}

	bool testEdgeIsValid(Point& from, Point& to, Prototypes* prototypes)
	{
		Edge testEdge(&from, &to);
		Point AA(std::min(from.x, to.x), std::min(from.y, to.y));
		Point BB(std::max(from.x, to.x), std::max(from.y, to.y));
		
		//TODO replace rect with traceLine?
		auto obstacles = prototypes->obstacleMap.getInCellsIntersectingRect(AA, BB);
		std::sort(obstacles.begin(), obstacles.end());
		
		Obstacle* previous = nullptr;
		for (Obstacle* obstacle : obstacles)
		{
			if (obstacle == previous) //getInRect may return several entries for the same obstacle
				continue;
			previous = obstacle;
			if (obstacle->isInside(to) || obstacle->isInside(from))
			{
				return false;
			}
			for (Edge &edge : obstacle->edges)
			{
				Point intersection = FMath::getEdgeIntersection(testEdge, edge);
				if (!std::isnan(intersection.x))
				{
					return false;
				}
			}
		}
		return true;
	}

	void buildRouteToTarget(Point& target, std::vector<RoutePoint>& route, Prototypes* prototypes)
	{
		int stepsLeft = route.back().location.distanceTo(target) / prototypes->variables.routeStepSize;
		route.resize(route.size() + stepsLeft);	
		
		auto iter = route.end() - stepsLeft;
		Point& last_ref = (route.end() - stepsLeft - 1)->location;
		Point* last_p = &last_ref;
		
		Point step = target - last_ref;
		step.scaleTo(prototypes->variables.routeStepSize);		

		while (stepsLeft--)
		{
			Point& newPoint = iter->location;

			newPoint.x = last_p->x + step.x;
			newPoint.y = last_p->y + step.y;

			iter->isValid_ = testEdgeIsValid(*last_p, newPoint, prototypes);
			//std::cout << "+point:" + newPoint.toString() + " " + (newPoint.isValid ? "true" : "false") << "\n";
			//S::log.add("+point:" + newPoint.toString() + " " + (iter->isValid_ ? "true" : "false"), { LOG_TAGS::INPUT_ });

			last_p = &newPoint;
			iter++;
		}
	}

	bool isRouteAnglePositive(std::vector<RoutePoint>& route, Point& target, Prototypes* prototypes)
	{
		auto iter = --route.end();

		Point ongoingVector = iter->location - (iter - 1)->location;
		Point finishingVector = target - iter->location;

		float angleDelta = FMath::angleDelta(ongoingVector.asAngle(), finishingVector.asAngle());
		return fabsf(angleDelta) < prototypes->variables.stepAngleWindow / 2;
	}
	namespace GameLogicInternal 
	{
		void makeLogicStep(GameState* state, int32_t timePassed, Prototypes* prototypes)
		{
			state->time.time += timePassed;
			
			Creeps::handleCreepSpawn(state, prototypes, timePassed);
			
			Creeps::handleCreepUpdate(state, prototypes, timePassed);
			Cars::handleCarUpdate(state, prototypes, timePassed);
			
			Creeps::handleCreepDeath(state, prototypes, timePassed);
			Cars::handleCarDeath(state, prototypes, timePassed);
		}

		void handleActionInput(GameState* state, InputActionMessage* input)
		{
			PlayerState* player = playerByLogin(state, input->login);
			if (player == nullptr)
				return;
			//...
		}

		void handlePlayerJoinedInput(GameState* state, InputPlayerJoinedMessage* input)
		{
			state->players.emplace_back();
			PlayerState& p = *(state->players.end() - 1);
			p.login = input->login;
		}

		void handlePlayerLeftInput(GameState* state, InputPlayerLeftMessage* input)
		{
			for (auto it = state->players.begin(); it != state->players.end(); it++) {
				if (it->login == input->login)
				{
					state->players.erase(it);
					break;
				}
			}
		}

		void handleRouteInput(GameState* state, InputRouteMessage* input, Prototypes* prototypes)
		{
			if (testRouteIsValid(input->route, prototypes))
			{
				Cars::launchCar(state, playerByLogin(state, input->login), input->route, prototypes);
			}
			else
			{
				S::log.add("Route not valid", { LOG_TAGS::GAME });
			}
		}

		void handleTimeInput(GameState* state, InputTimeMessage* input, Prototypes* prototypes)
		{
			TimeState& time = state->time;
			if (input->modifyAllowSteps)
			{
				if (input->allowSteps < 0)
				{
					time.allowedSteps = -1;
				}
				else if (time.allowedSteps > 0)
				{
					time.allowedSteps += input->allowSteps;
				}
				else
				{
					time.allowedSteps = time.performedSteps + input->allowSteps;
				}
				S::log.add("allows steps: " + std::to_string(time.performedSteps) + " / " + std::to_string(time.allowedSteps), {LOG_TAGS::INPUT_});
			}
			if (input->modifyForcedTimeScale)
			{
				time.forcedTimeScale = input->forcedTimeScale;
				S::log.add("tmp time scale: " + std::to_string(time.forcedTimeScale), {LOG_TAGS::INPUT_});
			}
			if (input->modifyTimeScale)
			{
				time.timeScale = input->timeScale;
				S::log.add("time scale: " + std::to_string(time.timeScale), {LOG_TAGS::INPUT_});
			}
			if (input->modifyStepsAtOnce)
			{
				time.stepsAtOnce = input->stepsAtOnce;
				S::log.add("steps at once: " + std::to_string(time.stepsAtOnce), {LOG_TAGS::INPUT_});
			}
			if (input->modifyForcedStepsAtOnce)
			{
				time.forcedStepsAtOnce = input->forcedStepsAtOnce;
				S::log.add("tmp steps at once: " + std::to_string(time.forcedStepsAtOnce), {LOG_TAGS::INPUT_});
			}
		}

		void handleGameLoad(GameState* state, LoadGameMessage* input, Prototypes* prototypes)
		{
			auto stream = SerializationStream::createExp();
			auto stamp = state->timeStamp;
			TimeState time = state->time;
			int32_t loadCount = state->loadCount;
			auto players = state->players;
			
			stream->write(input->state, input->stateLength); 
			stream->seekAbsolute(0);
			state->deserialize(*stream);
			
			
			state->timeStamp = stamp;
			if (time.allowedSteps > 0)
				state->time.allowedSteps = time.allowedSteps - time.performedSteps + state->time.performedSteps;
			state->time.forcedTimeScale = time.forcedTimeScale;	
			state->time.timeScale = time.timeScale;
			state->loadCount = loadCount + 1;
			
			for (size_t i = 0; i < players.size(); i++)
			{
				if (!playerByLogin(state, players[i].login))
				{
					PlayerState restoredPlayer = players[i];
					restoredPlayer.activeCars.clear();
					
					state->players.push_back(restoredPlayer);
				}
			}
			state->players.erase(std::remove_if(state->players.begin(), state->players.end(), [&players](PlayerState& sp){
				return players.end() == std::find_if(players.begin(), players.end(), [login = sp.login](PlayerState& p){ 
					return p.login == login; 
				});
			}), state->players.end());	
			
			state->propagatePrototypes(prototypes);
		}


		PlayerState* playerByLogin(GameState* state, int32_t login)
		{
			auto result = std::find_if(
						state->players.begin(), 
						state->players.end(), 
						[login](PlayerState& player){return player.login == login;}
						);
			return result == state->players.end() ? nullptr : &(*result);
		}

		void handleInputImpulse(GameState* state, InputImpulseMessage* input, Prototypes* prototypes)
		{
			switch (input->impulse)
			{
				case INPUT_IMPULSE::ACT_AI :
				{
					auto route = AI::getRandomWalk(state, prototypes);
					Cars::launchCar(state, playerByLogin(state, input->login), route, prototypes);
					break;
				}
				case INPUT_IMPULSE::ADD_AI :
				{
					int32_t login = std2::minElement(state->players, [](PlayerState& player){return player.login;})->login - 1;
					state->players.push_back({ 
						.login = login, 
						.isHeadless = true, 
						.isAI = true 
						});
					break;
				}
				case INPUT_IMPULSE::CLEAR_AI :
				{
					state->players.erase(std::remove_if(
						state->players.begin(), 
						state->players.end(), 
						[](PlayerState& player){return player.isHeadless;}
						));
					for(auto& p : state->players)
						p.isAI = false;
					break;
				}
				case INPUT_IMPULSE::TOGGLE_AI :
				{
					auto thisPlayer = playerByLogin(state, input->login);
					thisPlayer->isAI = !thisPlayer->isAI;
					break;
				}
				default:
				{
					S::log.add("Unknown input impulse: " + std::to_string((int8_t)input->impulse), {LOG_TAGS::ERROR_});
					break;
				}
			}
		}
	}
}