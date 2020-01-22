#include <GameState.h>


namespace AI
{
	void handleAITurn(GameState* state, Prototypes* prototypes, int32_t timePassed);
	std::vector<Point> getRandomWalk(GameState* state, Prototypes* prototypes);
	
	namespace AIInternal
	{
		struct RouteStruct
		{
			std::vector<Point> route;
			size_t index;
			uint32_t failureStreak;
			int32_t stepCount;
			bool isFinished;
		};
		
		void makeStep(RouteStruct& data, SeededRandom& random,  Prototypes* prototypes);
	}
}