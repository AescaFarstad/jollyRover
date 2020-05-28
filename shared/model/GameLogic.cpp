#include <GameLogic.h>
#include <Creeps.h>
#include <VisualDebug.h>
#include <Cars.h>
#include <AI.h>
#include <std2.h>
#include <BinarySerializer.h>


namespace GameLogic
{
	using namespace GameLogicInternal;
	
	void update(GameState* state, int32_t timePassed, std::vector<InputMessage*>& inputs, Prototypes* prototypes)
	{
		state->timeStamp += timePassed;
		
		handleInput(state, inputs, prototypes);
		
		TimeState& time = state->time;
		float timeScale = time.forcedTimeScale > 0 ? time.forcedTimeScale : time.timeScale;
		int32_t ingameTimePassed = std::max(1.f, timePassed * timeScale);
		bool isPaused = (time.allowedSteps >= 0 && time.allowedSteps <= time.performedSteps) && time.forcedTimeScale <= 0;
		
		if (isPaused)
		{
			return;
		}
		else
		{
			int32_t stepsAtOnce = state->time.forcedStepsAtOnce > 0 ? state->time.forcedStepsAtOnce : state->time.stepsAtOnce;
			VisualDebug::clear();
			for(int32_t i = 0; i < stepsAtOnce; i++)
			{
				if (time.forcedTimeScale > 0 && time.allowedSteps >= 0)
					time.allowedSteps++;
				state->time.performedSteps++;
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
			if (fabsf(route[i - 1].distanceTo(route[i]) - prototypes->variables.routeStepSize) > 100 *FMath::EPSILON)
			{
				if (i != route.size() - 1 || route[i - 1].distanceTo(route[i]) > prototypes->variables.routeStepSize + 100 *FMath::EPSILON)
				{
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
		std::sort(obstacles.begin(), obstacles.end()); //to group all copies of each obstacle
		
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
			for (Edge& edge : obstacle->edges)
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
		int32_t stepsLeft = route.back().location.distanceTo(target) / prototypes->variables.routeStepSize;
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
	
	PlayerState* playerByLogin(GameState* state, int32_t login)
	{
		auto result = std::find_if(
					state->players.begin(), 
					state->players.end(), 
					[login](PlayerState& player){return player.login == login;}
					);
		return result == state->players.end() ? nullptr : &(*result);
	}
		
	namespace GameLogicInternal 
	{
		void makeLogicStep(GameState* state, int32_t timePassed, Prototypes* prototypes)
		{
			state->time.time += timePassed;
			
			AI::handleAITurn(state, prototypes, timePassed);
			handlePlayerUpdate(state, prototypes, timePassed);
			
			Creeps::handleCreepSpawn(state, prototypes, timePassed);
			
			Cars::updateCarCache(state, prototypes, timePassed);
			Creeps::handleCreepUpdate(state, prototypes, timePassed);
			Cars::handleCarUpdate(state, prototypes, timePassed);
			
			Creeps::handleCreepDeath(state, prototypes, timePassed);
			Cars::handleCarDeath(state, prototypes, timePassed);
		}
		
		void handleInput(GameState* state, std::vector<InputMessage*>& inputs, Prototypes* prototypes)
		{
			for (size_t i = 0; i < inputs.size(); i++)
			{
				S::log.add("exec  input: " + 
					std::to_string((int16_t)inputs[i]->typeId) + 
					"(" + std::to_string(inputs[i]->serverStamp % 100000) + ")"+
					" at " + std::to_string(state->timeStamp % 100000), {LOG_TAGS::INPUT_, LOG_TAGS::GAME});
				switch (inputs[i]->typeId)
				{
				case MESSAGE_TYPE::TYPE_INPUT_ACTION_MSG:
				{
					handleActionInput(state, static_cast<InputActionMessage*>(inputs[i]));
					break;
				}
				case MESSAGE_TYPE::TYPE_INPUT_JOINED_MSG:
				{
					handlePlayerJoinedInput(state, static_cast<InputPlayerJoinedMessage*>(inputs[i]));
					break;
				}
				case MESSAGE_TYPE::TYPE_INPUT_LEFT_MSG:
				{
					handlePlayerLeftInput(state, static_cast<InputPlayerLeftMessage*>(inputs[i]));
					break;
				}
				case MESSAGE_TYPE::TYPE_INPUT_ROUTE_MSG:
				{
					handleRouteInput(state, static_cast<InputRouteMessage*>(inputs[i]), prototypes);
					break;
				}
				case MESSAGE_TYPE::TYPE_INPUT_TIME_MSG:
				{
					handleTimeInput(state, static_cast<InputTimeMessage*>(inputs[i]), prototypes);
					break;
				}
				case MESSAGE_TYPE::TYPE_LOAD_GAME_MSG:
				{
					handleGameLoad(state, static_cast<LoadGameMessage*>(inputs[i]), prototypes);
					break;
				}
				case MESSAGE_TYPE::TYPE_INPUT_IMPULSE_MSG:
				{
					handleInputImpulse(state, static_cast<InputImpulseMessage*>(inputs[i]), prototypes);
					break;
				}
				case MESSAGE_TYPE::TYPE_INPUT_DEBUG_MSG:
				{
					handleInputDebug(state, static_cast<InputDebugMessage*>(inputs[i]), prototypes);
					break;
				}
				default:
					break;
				}
			}
		}
		
		void handlePlayerUpdate(GameState* state, Prototypes* prototypes, int32_t timePassed)
		{
			auto& var = prototypes->variables;
			
			for(auto& player : state->players)
			{
				if (player.repairsLeft > 0)
					player.repairsLeft-= var.repairSpeed * timePassed;
				else
					player.refuelLeft -= var.refuelSpeed * timePassed;
			}
			
			if ((int32_t)state->timeStamp > prototypes->variables.reconnectWindow)
			{
				auto stamp = state->timeStamp - prototypes->variables.reconnectWindow;
				state->players.erase(std::remove_if(
						state->players.begin(),
						state->players.end(),
						[stamp](PlayerState& player){ 
							return player.wentOfflineAt > 0 && player.wentOfflineAt < stamp;
							}
				), state->players.end());
			}
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
			PlayerState* player = playerByLogin(state, input->login);
			if (!player)			
				state->players.emplace_back(input->login);
			else
				player->wentOfflineAt = 0;
			
		}

		void handlePlayerLeftInput(GameState* state, InputPlayerLeftMessage* input)
		{
			PlayerState* player = playerByLogin(state, input->playerLogin);
			player->wentOfflineAt = state->timeStamp;
		}

		void handleRouteInput(GameState* state, InputRouteMessage* input, Prototypes* prototypes)
		{
			auto player = playerByLogin(state, input->login);
			
			if (!Cars::canLaunchCar(*player))
			{
				S::log.add("Attempt to lauch a car before its ready", { LOG_TAGS::GAME });
				return;
			}	
			
			if (testRouteIsValid(input->route, prototypes))
			{
				Cars::launchCar(state, player, input->route, prototypes);
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
			auto stamp = state->timeStamp;
			TimeState time = state->time;
			int32_t loadCount = state->loadCount;
			auto players = state->players;
			
			BinarySerializer bs(input->state);
			bs.read(*state);
			
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
		
		void handleInputImpulse(GameState* state, InputImpulseMessage* input, Prototypes* prototypes)
		{
			switch (input->impulse)
			{
				case INPUT_IMPULSE::ACT_AI :
				{
					auto route = AI::getRandomWalk(state->random, prototypes);
					Cars::launchCar(state, playerByLogin(state, input->login), route, prototypes);
					break;
				}
				case INPUT_IMPULSE::ADD_AI :
				{
					int32_t login = std2::minElement(state->players, [](PlayerState& player){return player.login;})->login - 1;
					if (login > 0)
						login = -1;
					auto& aip = state->players.emplace_back(login);
					aip.isHeadless = true;
					aip.isAI = true;
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
				case INPUT_IMPULSE::TMP_DEBUG :
				{
					for(auto& form : state->formations)
					{
						Creeps::CreepsInternal::compactFormation(form, state);
					}
					break;
				}
				default:
				{
					S::log.add("Unknown input impulse: " + std::to_string((int8_t)input->impulse), {LOG_TAGS::ERROR_, LOG_TAGS::HARD_LOG});
					break;
				}
			}
		}
		
		void handleInputDebug(GameState* state, InputDebugMessage* input, Prototypes* prototypes)
		{
			switch (input->action)
			{
				case DEBUG_ACTION::BOOM :
				{
					Creeps::CreepsInternal::spawnProjectile(input->coords, input->coords, &prototypes->weapons[0], 0, state);
					break;
				}
				case DEBUG_ACTION::KILL_LEFT :
				{
					for(auto& creep : state->creeps)
					{
						if (creep.unit.force == 0)
							creep.unit.health = 0;
					}
					break;
				}
				case DEBUG_ACTION::KILL_RIGHT :
				{
					for(auto& creep : state->creeps)
					{
						if (creep.unit.force == 1)
							creep.unit.health = 0;
					}
					break;
				}
				case DEBUG_ACTION::RETREAT :
				{
					for(auto& form : state->formations)
					{
						form.objectiveID = -1;
						Creeps::CreepsInternal::setObjective(form, SUB_OBJECTIVE::RETREAT, state, prototypes);
					}
					break;
				}
				default:
				{
					S::log.add("Unknown input debug: " + std::to_string((int8_t)input->action), {LOG_TAGS::ERROR_, LOG_TAGS::HARD_LOG});
					break;
				}
			}
		}
	}
}