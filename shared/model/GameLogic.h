#pragma once
#include <GameState.h>
#include <Prototypes.h>
#include <InputMessage.h>
#include <InputActionMessage.h>
#include <InputPlayerJoinedMessage.h>
#include <InputPlayerLeftMessage.h>
#include <InputRouteMessage.h>
#include <CarLogic.h>

class GameLogic
{
public:
	GameLogic();
	~GameLogic();

	CarLogic carLogic;

	void update(GameState* state, int timePassed, std::vector<InputMessage*> &inputs, Prototypes* prototypes);

	bool testRouteIsValid(std::vector<Point> &route);

	static bool testEdgeIsValid(Point &from, Point &to, std::vector<Obstacle> &obstacles);


private:
	GameState* state;
	int timePassed;
	Prototypes* prototypes;

	void handleActionInput(InputActionMessage* input);
	void handlePlayerJoinedInput(InputPlayerJoinedMessage* input);
	void handlePlayerLeftInput(InputPlayerLeftMessage* input);
	void handleRouteInput(InputRouteMessage* input);

	PlayerTest* playerByLogin(int32_t login);
};