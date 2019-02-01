#include <CarLogic.h>

CarLogic::CarLogic()
{
}

CarLogic::~CarLogic()
{
}

void CarLogic::update(CarRide &ride, PlayerTest &player, GameState* state, Prototypes* prototypes, int timePassed)
{
	this->ride = &ride;
	this->player = &player;
	this->state = state;
	this->prototypes = prototypes;

	float passedThisStep = prototypes->cars[ride.car.prototypeId].speed * timePassed;

	while (passedThisStep > 0 && (size_t)ride.routeIndex < ride.route.size() - 1)
	{
		passedThisStep = moveCar(passedThisStep);
	}
	if (ride.routeIndex == ride.route.size() - 1)
	{
		ride.isFinished = true;
	}	
}

float CarLogic::moveCar(float maxDistance)
{
	float thisDistance = ride->route[ride->routeIndex + 1].distanceTo(ride->route[ride->routeIndex]);
	if (thisDistance * (1 - ride->progress) < maxDistance)
	{
		ride->progress += maxDistance / thisDistance;
	}
	else
	{
		maxDistance -= thisDistance * (1 - ride->progress);
		ride->routeIndex++;
		ride->progress = 0;
	}
	return maxDistance;
}
