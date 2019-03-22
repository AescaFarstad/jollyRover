#pragma once
#include <GameState.h>
#include <Prototypes.h>
#include <InputMessage.h>
#include <InputActionMessage.h>
#include <InputPlayerJoinedMessage.h>
#include <InputPlayerLeftMessage.h>
#include <InputRouteMessage.h>
#include <InputTimeMessage.h>
#include <CarLogic.h>

class GameLogic
{
public:
	GameLogic() = default;
	~GameLogic() = default;

	CarLogic carLogic;

	void update(GameState* state, int timePassed, std::vector<InputMessage*>& inputs, Prototypes* prototypes);


	static bool testEdgeIsValid(Point& from, Point& to, std::vector<Obstacle>& obstacles);
	static bool testRouteIsValid(std::vector<Point>& route, Prototypes* prototypes);
	static bool buildRouteToTarget(Point& target, std::vector<RoutePoint>& route, Prototypes* prototypes);
	static bool isRouteAnglePositive(std::vector<RoutePoint>& route, Point& target, Prototypes* prototypes);

private:
	GameState* state;

	void handleActionInput(InputActionMessage* input);
	void handlePlayerJoinedInput(InputPlayerJoinedMessage* input);
	void handlePlayerLeftInput(InputPlayerLeftMessage* input);
	void handleRouteInput(InputRouteMessage* input, Prototypes* prototypes);
	void handleTimeInput(InputTimeMessage* input, Prototypes* prototypes);

	PlayerTest* playerByLogin(int32_t login);
};