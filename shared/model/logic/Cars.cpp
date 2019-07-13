#include <Cars.h>
#include <FMath.h>
#include <VisualDebug.h>
#include <std2.h>
#include <Field.h>

namespace Cars
{
	using namespace CarsInternal;
	
	void handleCarUpdate(GameState* state, Prototypes* prototypes, int32_t timePassed)
	{
		for (PlayerTest& player : state->players)
		{
			for (CarRide& ride : player.activeCars)
			{
				updateCar(ride, player, state, prototypes, timePassed);
			}
		}
	}
	
	void handleCarDeath(GameState* state, Prototypes* prototypes, int32_t timePassed)
	{
		for (PlayerTest& player : state->players)
		{
			player.activeCars.erase(std::remove_if(player.activeCars.begin(), player.activeCars.end(), [](CarRide &ride) {
				return ride.isFinished || ride.car.health <= 0; 
			}), player.activeCars.end());
		}
	}
	
	namespace CarsInternal
	{
		void updateCar(CarRide& ride, PlayerTest& player, GameState* state, Prototypes* prototypes, int32_t timePassed)
		{
			float passedThisStep = prototypes->cars[ride.car.prototypeId].speed * timePassed;

			while (passedThisStep > 0 && (size_t)ride.routeIndex < ride.route.size() - 1)
			{
				passedThisStep = moveCar(passedThisStep, ride);
			}
			if ((uint16_t)ride.routeIndex == ride.route.size() - 1)
			{
				ride.isFinished = true;
			}
		}
		
		float moveCar(float maxDistance, CarRide& ride)
		{
			float thisDistance = ride.route[ride.routeIndex + 1].distanceTo(ride.route[ride.routeIndex]);
			if (thisDistance * (1 - ride.progress) < maxDistance)
			{
				ride.progress += maxDistance / thisDistance;
			}
			else
			{
				maxDistance -= thisDistance * (1 - ride.progress);
				ride.routeIndex++;
				ride.progress = 0;
			}
			return maxDistance;
		}
	}
}