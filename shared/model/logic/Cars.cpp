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
			for (CarState& car : player.activeCars)
			{
				updateCar(car, player, state, prototypes, timePassed);
			}
		}
	}
	
	void handleCarDeath(GameState* state, Prototypes* prototypes, int32_t timePassed)
	{
		if (state->isEventLoggerEnabled)
		{
			for (PlayerTest& player : state->players)
			{
				for(auto& car : player.activeCars)
				{
					if (car.unit.health <= 0)
					{
						state->eventLogger.addUnitDeath(
							state->time.time, 
							car.object.id, 
							car.object.prototypeId, 
							car.unit.location, 	
							car.unit.voluntaryMovement.asAngle(), 
							Point()
						);
					}
				}				
			}
		}
		
		
		for (PlayerTest& player : state->players)
		{
			player.activeCars.erase(std::remove_if(player.activeCars.begin(), player.activeCars.end(), [](CarState &car) {
				return car.isFinished || car.unit.health <= 0; 
			}), player.activeCars.end());
		}
	}
	
	void launchCar(GameState* state, PlayerTest* player, std::vector<Point>& route, Prototypes* prototypes)
	{
		player->activeCars.emplace_back();
		CarState& car = player->activeCars.back();
		
		car.route = route;
		car.progress = 0;
		car.routeIndex = 0;
		car.isFinished = false;
		
		CarProto& proto = prototypes->cars[0];
		
		car.object.id = state->idCounter++;
		car.object.prototypeId = 0;
		car.unit.force = 2; //TODO create player forces
		car.unit.health = proto.maxHealth;
		car.unit.location = locationFromRouteProgress(car);
		car.unit.voluntaryMovement = Point(0, 0);
	}
	
	namespace CarsInternal
	{
		void updateCar(CarState& car, PlayerTest& player, GameState* state, Prototypes* prototypes, int32_t timePassed)
		{
			Point startingLocation = car.unit.location;
			float passedThisStep = prototypes->cars[car.object.prototypeId].speed * timePassed;

			while (passedThisStep > 0 && (size_t)car.routeIndex < car.route.size() - 1)
			{
				passedThisStep = moveCar(passedThisStep, car);
			}
			
			car.unit.location = locationFromRouteProgress(car);
			car.unit.voluntaryMovement = car.unit.location - startingLocation;
			
			CarProto& proto = prototypes->cars[car.object.prototypeId];
			auto rolledOverCreeps = state->creepMap_.getInRadius(car.unit.location, proto.size + prototypes->variables.maxCreepSize);
			
			for(auto& creep : rolledOverCreeps)
			{
				if (creep->creepProto_->size + proto.size > car.unit.location.distanceTo(creep->unit.location))
				{
					if (creep->creepProto_->weight > 100)
						car.unit.health = 0;
					else
						creep->unit.health = 0;					
				}
			}
			
			if ((uint16_t)car.routeIndex == car.route.size() - 1)
			{
				car.isFinished = true;
			}
		}
		
		float moveCar(float maxDistance, CarState& car)
		{
			float thisDistance = car.route[car.routeIndex + 1].distanceTo(car.route[car.routeIndex]);
			if (thisDistance * (1 - car.progress) > maxDistance)
			{
				car.progress += maxDistance / thisDistance;
				return 0;
			}
			else
			{
				maxDistance -= thisDistance * (1 - car.progress);
				car.routeIndex++;
				car.progress = 0;
				return maxDistance;
			}
		}
		
		Point locationFromRouteProgress(CarState& car)
		{
			Point result = car.route[car.routeIndex + 1] - car.route[car.routeIndex];
			result.scaleBy(car.progress);
			result += car.route[car.routeIndex];
			return result;
		}
	}
}