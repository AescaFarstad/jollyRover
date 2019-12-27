#pragma once
#include <GameState.h>
#include <Prototypes.h>
#include <InputMessage.h>
#include <InputActionMessage.h>
#include <InputPlayerJoinedMessage.h>
#include <InputPlayerLeftMessage.h>
#include <InputRouteMessage.h>
#include <InputTimeMessage.h>
#include <LoadGameMessage.h>
#include <InputImpulseMessage.h>
#include <Cars.h>

namespace GameLogic
{
	void update(GameState* state, int32_t timePassed, std::vector<InputMessage*>& inputs, Prototypes* prototypes);

	bool testEdgeIsValid(Point& from, Point& to, Prototypes* prototypes);
	bool testRouteIsValid(std::vector<Point>& route, Prototypes* prototypes);
	void buildRouteToTarget(Point& target, std::vector<RoutePoint>& route, Prototypes* prototypes);
	bool isRouteAnglePositive(std::vector<RoutePoint>& route, Point& target, Prototypes* prototypes);
	PlayerState* playerByLogin(GameState* state, int32_t login);

	namespace GameLogicInternal
	{
		void makeLogicStep(GameState* state, int32_t timePassed, Prototypes* prototypes);
		void handleActionInput(GameState* state, InputActionMessage* input);
		void handlePlayerJoinedInput(GameState* state, InputPlayerJoinedMessage* input);
		void handlePlayerLeftInput(GameState* state, InputPlayerLeftMessage* input);
		void handleRouteInput(GameState* state, InputRouteMessage* input, Prototypes* prototypes);
		void handleTimeInput(GameState* state, InputTimeMessage* input, Prototypes* prototypes);
		void handleGameLoad(GameState* state, LoadGameMessage* input, Prototypes* prototypes);
		void handleInputImpulse(GameState* state, InputImpulseMessage* input, Prototypes* prototypes);
		
		void handlePlayerUpdate(GameState* state, Prototypes* prototypes, int32_t timePassed);

	}
};