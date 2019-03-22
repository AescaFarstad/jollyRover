#include <GameLogic.h>
#include <Keyboard.h>
#include <Creeps.h>
#include <InputTimeMessage.h>


void GameLogic::update(GameState* state, int timePassed, std::vector<InputMessage*> &inputs, Prototypes* prototypes)
{
	this->state = state;

	state->timeStamp += timePassed;
	
	for (size_t i = 0; i < inputs.size(); i++)
	{
		if (inputs[i]->typeId == MessageTypes::TYPE_INPUT_ACTION_MSG)
			handleActionInput(static_cast<InputActionMessage*>(inputs[i]));
		else if (inputs[i]->typeId == MessageTypes::TYPE_INPUT_JOINED_MSG)
			handlePlayerJoinedInput(static_cast<InputPlayerJoinedMessage*>(inputs[i]));
		else if (inputs[i]->typeId == MessageTypes::TYPE_INPUT_LEFT_MSG)
			handlePlayerLeftInput(static_cast<InputPlayerLeftMessage*>(inputs[i]));
		else if (inputs[i]->typeId == MessageTypes::TYPE_INPUT_ROUTE_MSG)
			handleRouteInput(static_cast<InputRouteMessage*>(inputs[i]), prototypes);
		else if (inputs[i]->typeId == MessageTypes::TYPE_INPUT_TIME_MSG)
			handleTimeInput(static_cast<InputTimeMessage*>(inputs[i]), prototypes);
		else if (inputs[i]->typeId == MessageTypes::TYPE_LOAD_GAME_MSG)
			handleGameLoad(static_cast<LoadGameMessage*>(inputs[i]), prototypes);
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
		if (time.forcedTimeScale > 0 && time.allowedSteps >= 0)
			time.allowedSteps++;
		state->time.performedSteps++;
	}
		
	state->time.time += ingameTimePassed;

	for (size_t i = 0; i < state->players.size(); i++)
	{
		PlayerTest &player = state->players[i];
		if (player.buttonDown_Left)
			player.speedX -= 0.15f;
		if (player.buttonDown_Right)
			player.speedX += 0.15f;
		if (player.buttonDown_Forward)
			player.speedY -= 0.15f;
		if (player.buttonDown_Backward)
			player.speedY += 0.15f;

		player.speedX *= 0.95f;
		player.speedY *= 0.95f;


		player.x += player.speedX;
		player.y += player.speedY;

		if (player.x > 200 && player.speedX > 0)
			player.speedX *= -1;

		if (player.y > 200 && player.speedY > 0)
			player.speedY *= -1;

		if (player.y < -200 && player.speedY < 0)
			player.speedY *= -1;

		if (player.x < -200 && player.speedX < 0)
			player.speedX *= -1;

		for (CarRide &ride : player.activeCars)
		{
			carLogic.update(ride, player, state, prototypes, ingameTimePassed);
		}

		auto finishedCars = std::remove_if(player.activeCars.begin(), player.activeCars.end(), [](CarRide &ride) {
			//bool isf = ride.isFinished || ride.car.health <= 0;
			//printf("has finish %d, %d\n", isf, ride.ui);
			return ride.isFinished || ride.car.health <= 0; 
		});
		if (finishedCars != player.activeCars.end())
		{
			player.activeCars.erase(finishedCars);
		}

	}
	Creeps::handleCreeps(state, prototypes, ingameTimePassed);
	/*
	Creeps::spawnCreeps(state, prototypes, timePassed);
	Creeps::processFormations(state, prototypes, timePassed);
	Creeps::processCreeps(state, prototypes, timePassed);
	Creeps::processProjectiles(state, prototypes, timePassed);
	Creeps::removeDeadProjectiles(state);
	Creeps::removeDeadCreeps(state);*/
}

bool GameLogic::testRouteIsValid(std::vector<Point>& route, Prototypes* prototypes)
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
		if (fabsf(route[i - 1].distanceTo(route[i]) - prototypes->variables.routeStepSize) > FMath::EPSILON)
		{
			if (i != route.size() - 1 || route[i - 1].distanceTo(route[i]) > prototypes->variables.routeStepSize + FMath::EPSILON)
			{
				//float ds = route[i - 1].distanceTo(route[i]);
				//float max = prototypes->variables.routeStepSize + FMath::EPSILON;
				//float abs = std::fabsf(route[i - 1].distanceTo(route[i]) - prototypes->variables.routeStepSize);
				return false;
			}
		}
		if (!testEdgeIsValid(route[i - 1], route[i], prototypes->obstacles))
			return false;
	}
	//TODO check angles
	return true;
}

bool GameLogic::testEdgeIsValid(Point& from, Point& to, std::vector<Obstacle>& obstacles)
{
	Edge testEdge(&from, &to);
	Point AA(std::min(from.x, to.x), std::min(from.y, to.y));
	Point BB(std::max(from.x, to.x), std::max(from.y, to.y));

	for (Obstacle& obstacle : obstacles)
	{
		if (obstacle.AA.x <= BB.x && obstacle.AA.y <= BB.y && obstacle.BB.x >= AA.x && obstacle.BB.y >= AA.y)
		{
			if (obstacle.isInside(to) || obstacle.isInside(from))
			{
				return false;
			}
			else
			{
				//S::log.add(to.toString() + " is outside");
			}

			for (Edge &edge : obstacle.edges)
			{
				Point intersection = FMath::getEdgeIntersection(testEdge, edge);
				if (!std::isnan(intersection.x))
				{
					return false;
				}
			}
		}
		else
		{
			//S::log.add(to.toString() + "outside of AABB of " + obstacle.AA.toString() + obstacle.BB.toString());
		}
	}
	return true;
}

bool GameLogic::buildRouteToTarget(Point& target, std::vector<RoutePoint>& route, Prototypes* prototypes)
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

		iter->isValid_ = GameLogic::testEdgeIsValid(*last_p, newPoint, prototypes->obstacles);
		//std::cout << "+point:" + newPoint.toString() + " " + (newPoint.isValid ? "true" : "false") << "\n";
		//S::log.add("+point:" + newPoint.toString() + " " + (iter->isValid_ ? "true" : "false"), { LOG_TAGS::INPUT_ });

		last_p = &newPoint;
		iter++;
	}
}

bool GameLogic::isRouteAnglePositive(std::vector<RoutePoint>& route, Point& target, Prototypes* prototypes)
{
	auto iter = --route.end();

	Point ongoingVector = iter->location - (iter - 1)->location;
	Point finishingVector = target - iter->location;

	float angleDelta = FMath::angleDelta(ongoingVector.asAngle(), finishingVector.asAngle());
	return fabsf(angleDelta) < prototypes->variables.stepAngleWindow / 2;
}

void GameLogic::handleActionInput(InputActionMessage* input)
{
	PlayerTest* player = playerByLogin(input->login);
	if (player == nullptr)
		return;
	for (size_t j = 0; j < input->downedButtons.size(); j++)
	{
		if (input->downedButtons[j] == (int8_t)KEYBOARD_ACTIONS::LEFT)
			player->buttonDown_Left = true;
		else if (input->downedButtons[j] == (int8_t)KEYBOARD_ACTIONS::RIGHT)
			player->buttonDown_Right = true;
		else if (input->downedButtons[j] == (int8_t)KEYBOARD_ACTIONS::FORWARD)
			player->buttonDown_Forward = true;
		else if (input->downedButtons[j] == (int8_t)KEYBOARD_ACTIONS::BACKWARD)
			player->buttonDown_Backward = true;
	}
	for (size_t j = 0; j < input->uppedButtons.size(); j++)
	{
		if (input->uppedButtons[j] == (int8_t)KEYBOARD_ACTIONS::LEFT)
			player->buttonDown_Left = false;
		else if (input->uppedButtons[j] == (int8_t)KEYBOARD_ACTIONS::RIGHT)
			player->buttonDown_Right = false;
		else if (input->uppedButtons[j] == (int8_t)KEYBOARD_ACTIONS::FORWARD)
			player->buttonDown_Forward = false;
		else if (input->uppedButtons[j] == (int8_t)KEYBOARD_ACTIONS::BACKWARD)
			player->buttonDown_Backward = false;
	}
}

void GameLogic::handlePlayerJoinedInput(InputPlayerJoinedMessage* input)
{
	state->players.emplace_back();
	PlayerTest& p = *(state->players.end() - 1);
	p.login = input->login;
	p.x = (float)state->random.get(-50, 50);
	p.y = (float)state->random.get(-50, 50);

	std::string logStr  = "spawn player " + std::to_string(p.login) + " at ";
	logStr += "[";
	logStr += std::to_string(p.x);
	logStr += ":";
	logStr += std::to_string(p.y);
	logStr += "] ";

	S::log.add(logStr, { LOG_TAGS::GAME });
}

void GameLogic::handlePlayerLeftInput(InputPlayerLeftMessage* input)
{
	for (auto it = state->players.begin(); it != state->players.end(); it++) {
		if (it->login == input->login)
		{
			state->players.erase(it);
			break;
		}
	}
}

void GameLogic::handleRouteInput(InputRouteMessage* input, Prototypes* prototypes)
{
	if (testRouteIsValid(input->route, prototypes))
	{
		playerByLogin(input->login)->activeCars.emplace_back(input->route, prototypes->cars[0]);
	}
	else
	{
		S::log.add("Route not valid", { LOG_TAGS::GAME });
	}
}

void GameLogic::handleTimeInput(InputTimeMessage* input, Prototypes* prototypes)
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
}

void GameLogic::handleGameLoad(LoadGameMessage* input, Prototypes* prototypes)
{
	auto stream = SerializationStream::createExp();
	auto stamp = state->timeStamp;
	TimeState time = state->time;
	stream->write(input->state, input->stateLength); 
	stream->seekAbsolute(0);
	state->deserialize(*stream);
	state->propagatePrototypes(prototypes);
	
	state->timeStamp = stamp;
	if (time.allowedSteps > 0)
		state->time.allowedSteps = time.allowedSteps - time.performedSteps + state->time.performedSteps;
		
	//TODO keep existing players in game
}

PlayerTest* GameLogic::playerByLogin(int32_t login)
{
	for (size_t i = 0; i < state->players.size(); i++)
	{
		if (state->players[i].login == login)
			return &state->players[i];
	}
	return nullptr;
}
