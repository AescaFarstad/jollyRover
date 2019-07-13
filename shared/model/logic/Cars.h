#include <GameState.h>

namespace Cars
{
	void handleCarUpdate(GameState* state, Prototypes* prototypes, int32_t timePassed);
	void handleCarDeath(GameState* state, Prototypes* prototypes, int32_t timePassed);		
	
	namespace CarsInternal
	{
		void updateCar(CarRide& ride, PlayerTest& player, GameState* state, Prototypes* prototypes, int32_t timePassed);
		float moveCar(float maxDistance, CarRide& ride);
	}
}