#include <Cars.h>
#include <FMath.h>
#include <VisualDebug.h>
#include <std2.h>
#include <Field.h>
#include <GameUtil.h>

namespace Cars
{
	using namespace CarsInternal;
	
	void updateCarCache(GameState* state, Prototypes* prototypes, int32_t timePassed)
	{
		if (!state->carMap_.isValid())
		{
			Point BB(prototypes->variables.fieldWidth + 100, prototypes->variables.fieldHeight + 100);
			state->carMap_ = SpatialMap<CarState>(120, true, Point(-100, -100), BB);
		}
		state->carMap_.reset(0);
		for (PlayerState& player : state->players)
		{
			state->carMap_.addUnique(player.activeCars);
		}
	}
	
	void handleCarUpdate(GameState* state, Prototypes* prototypes, int32_t timePassed)
	{
		for (PlayerState& player : state->players)
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
			for (auto& player : state->players)
			{
				for(auto& car : player.activeCars)
				{
					if (car.unit.health <= 0)
					{
						auto base = prototypes->variables.repairRefuelBase;
						
						player.score += car.score;
						player.refuelTotal = base;
						player.refuelLeft = player.refuelTotal;
						player.repairsTotal = base;
						player.repairsLeft = player.repairsTotal;
					
						state->eventLogger.addCarDeath(
							state->time.time, 
							car, 
							player.login,
							Point()
						);
					}
				}				
			}
		}
		
		
		for (auto& player : state->players)
		{
			for (auto& car : player.activeCars)
			{
				if (car.isFinished)
				{
					auto base = prototypes->variables.repairRefuelBase;
					auto fullHealth = prototypes->cars[car.unit.prototypeId].maxHealth;
					
					player.score += car.score;
					player.refuelTotal = base;
					player.refuelLeft = player.refuelTotal;
					player.repairsTotal = FMath::lerp(fullHealth, 0.f, 0, base, car.unit.health);
					player.repairsLeft = player.repairsTotal;
				}
					
			}
			player.activeCars.erase(std::remove_if(player.activeCars.begin(), player.activeCars.end(), [](CarState &car) {
				return car.isFinished || car.unit.health <= 0; 
			}), player.activeCars.end());
		}
	}
	
	void launchCar(GameState* state, PlayerState* player, const std::vector<Point>& route, Prototypes* prototypes)
	{
		player->activeCars.emplace_back();
		CarState& car = player->activeCars.back();
		
		car.route = route;
		car.progress = 0;
		car.routeIndex = 0;
		car.isFinished = false;
		car.score = 0;
		
		CarProto& proto = prototypes->cars[0];
		
		car.speed = proto.speed;
		car.accel = proto.accel;
		car.timeSinceRollover = 0;
		car.agro = 0;
		
		car.unit.id = state->idCounter++;
		car.unit.prototypeId = 0;
		car.unit.force = 2; //TODO create player forces
		car.unit.health = proto.maxHealth;
		car.unit.location = locationFromRouteProgress(car);
		car.unit.voluntaryMovement = Point(0, 0);
	}
	
	bool canLaunchCar(const PlayerState& player)
	{
		return player.activeCars.size() == 0 && player.refuelLeft <= 0 && player.repairsLeft <= 0;
	}
	
	namespace CarsInternal
	{
		void updateCar(CarState& car, PlayerState& player, GameState* state, Prototypes* prototypes, int32_t timePassed)
		{
			car.speed += car.accel * timePassed;
			if (car.agro > 0)
			{
				car.timeSinceRollover += timePassed;
				float scale = FMath::lerp(0, 0, 1000, prototypes->variables.carAgroFadeGainPerSecond, car.timeSinceRollover);
				car.agro = std::max(0.f, car.agro - scale * timePassed);				
			}
			
			Point startingLocation = car.unit.location;
			float passedThisStep = car.speed * timePassed;

			while (passedThisStep > 0 && (size_t)car.routeIndex < car.route.size() - 1)
			{
				passedThisStep = moveCar(passedThisStep, car);
			}
			
			car.unit.location = locationFromRouteProgress(car);
			car.unit.voluntaryMovement = car.unit.location - startingLocation;
			
			CarProto& proto = prototypes->cars[car.unit.prototypeId];
			auto rolledOverCreeps = state->creepMap_.getInRadius(car.unit.location, proto.size + prototypes->variables.maxCreepSize);
			
			for(auto& creep : rolledOverCreeps)
			{
				if (creep->creepProto_->size + proto.size > car.unit.location.distanceTo(creep->unit.location))
				{
					if (!creep->creepProto_->rolloverable)
					{
						car.unit.health = 0;
					}
					else
					{
						creep->unit.health = 0;
						auto formation = GameUtil::formationById(creep->formationId, state);
						formation->carAgro += prototypes->variables.carAgroPerUnit;
						car.score++;
						car.speed *= creep->creepProto_->slowOnRollOver;
						car.timeSinceRollover = 0;
						car.agro += prototypes->variables.carAgroGainPerUnit;
					}				
				}
			}
			
			for (auto& player : state->players)
			{
				for (auto& anotherCar : player.activeCars)
				{
					if (&anotherCar != &car)
					{
						CarProto& anotherProto = prototypes->cars[anotherCar.unit.prototypeId];
						if (anotherCar.unit.location.distanceTo(car.unit.location) < proto.size + anotherProto.size)
						{
							if (anotherCar.speed > car.speed)
							{
								car.unit.health = 0;
								anotherCar.speed /= 2;
							}
							else
							{
								anotherCar.unit.health = 0;
								car.speed /= 2;
							}
							
						}
					}
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