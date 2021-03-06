#include <GameState.h>

namespace Cars
{
	void updateCarCache(GameState* state, Prototypes* prototypes, int32_t timePassed);
	void handleCarUpdate(GameState* state, Prototypes* prototypes, int32_t timePassed);
	void handleCarDeath(GameState* state, Prototypes* prototypes, int32_t timePassed);	
			
	void launchCar(GameState* state, PlayerState* player, const std::vector<Point>& route, Prototypes* prototypes);
	bool canLaunchCar(const PlayerState& player);
	
	namespace CarsInternal
	{
		void updateCar(CarState& ride, PlayerState& player, GameState* state, Prototypes* prototypes, int32_t timePassed);
		float moveCar(float maxDistance, CarState& ride);
		Point locationFromRouteProgress(CarState& ride);
	}
}