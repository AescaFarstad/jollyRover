#include <GameState.h>


namespace AI
{
	void handleAITurn(GameState* state, Prototypes* prototypes, int32_t timePassed);
	std::vector<Point> getRandomWalk(GameState* state, Prototypes* prototypes);
	
	namespace AIInternal
	{
		
		void makeStep(std::vector<Point>& route, size_t index, int32_t failureStreak, SeededRandom& random,  Prototypes* prototypes);
	}
}