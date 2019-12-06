#include <GameState.h>


namespace AI
{
	std::vector<Point> getRandomWalk(GameState* state, Prototypes* prototypes);
	
	namespace AIInternal
	{
		
		void makeStep(std::vector<Point>& route, size_t index, int32_t failureStreak, SeededRandom& random,  Prototypes* prototypes);
	}
}