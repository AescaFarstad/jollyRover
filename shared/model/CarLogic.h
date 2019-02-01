#pragma once
#include <Car.h>
#include <CarRide.h>
#include <FMath.h>
#include <Point.h>
#include <PlayerTest.h>
#include <GameState.h>
#include <Prototypes.h>

class CarLogic
{
public:
	CarLogic();
	~CarLogic();

	CarRide* ride;
	PlayerTest* player;
	GameState* state;
	Prototypes* prototypes;

	void update(CarRide &ride, PlayerTest &player, GameState* state, Prototypes* prototypes, int timePassed);

private:

	float moveCar(float maxDistance);

};