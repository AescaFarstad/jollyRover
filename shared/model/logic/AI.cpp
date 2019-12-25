#include <AI.h>
#include <GameLogic.h>

namespace AI
{		
	int32_t boundary = 100;
	float boundaryPower = 3;
	float desireBase = 10;
	
	void handleAITurn(GameState* state, Prototypes* prototypes, int32_t timePassed)
	{
		for(auto& p : state->players)
		{
			if (p.isAI && p.activeCars.size() == 0)
			{
				Cars::launchCar(state, &p, getRandomWalk(state, prototypes), prototypes);
			}
		}
	}
	
	
	std::vector<Point> getRandomWalk(GameState* state, Prototypes* prototypes)
	{
		AIInternal::RouteStruct data;
		
		while(!GameLogic::testRouteIsValid(data.route, prototypes))
		{
			Point start = { (float)state->random.get(0, prototypes->variables.fieldWidth), (float)prototypes->variables.fieldHeight };
			Point firstStep = { start.x, start.y - prototypes->variables.routeStepSize };
			data = {
				.route = { start, firstStep },
				.index = 0,
				.failureStreak = 0,
				.stepCount = 0,
				.isFinished = false
			};
			while (!data.isFinished && data.stepCount < 5000)
				AIInternal::makeStep(data, state->random, prototypes);
		}
		data.route.erase(data.route.begin() + data.index + 1, data.route.end());
		
		S::log.add(
				"[Route] len: " + std::to_string(data.stepCount) + 
				" f: " + std::to_string(data.stepCount - data.index) + 
				" r: " + std::to_string((float)data.index / data.stepCount),
			{LOG_TAGS::GAME});			
		return data.route;
	}
	
	namespace AIInternal
	{
	
		void makeStep(RouteStruct& data, SeededRandom& random,  Prototypes* prototypes)
		{
			data.stepCount++;
			
			Point attraction = Point(0, 0);
			auto& last = data.route[data.index];
			if (last.y > prototypes->variables.fieldHeight)
			{
				last.y = prototypes->variables.fieldHeight;
				data.isFinished = true;
				return;
			}
			
			if (last.x < boundary)
				attraction += Point(FMath::nlerp(0, 0, boundary, 100, boundary - last.x, boundaryPower), 0);
			if (last.x > prototypes->variables.fieldWidth - boundary)
				attraction += Point(- FMath::nlerp(0, 0, boundary, 100, boundary - (prototypes->variables.fieldWidth - last.x), boundaryPower), 0);
			if (last.y < boundary)
				attraction += Point(0, FMath::nlerp(0, 0, boundary, 100, boundary - last.y, boundaryPower));
			
			if (data.index > prototypes->variables.maxRouteSteps / 30)
			{
				attraction += Point(0, 1);
			}
			if (data.index < prototypes->variables.minRouteSteps)
			{
				attraction += Point(0, -10);
			}
			if (data.index < 50)
			{
				attraction += Point(0, -1);
			}
			
			float stepAngle = 0;
			float attractionAngle = 0;
			int32_t attractionStrength = 1;
			float angle = (data.route[data.index] - data.route[data.index - 1]).asAngle();
			if (attraction.getLength() != 0)
			{
				attractionStrength = 2 + std::sqrt(attraction.getLength());
				attractionAngle = attraction.asAngle();
			}
			
			
			for(int32_t i = 0; i < attractionStrength; i++)
			{
				float allowedAngleVariation = FMath::lerpClipped(
						0, prototypes->variables.stepAngleWindow / 5, 
						5, prototypes->variables.stepAngleWindow / 2, 
						data.failureStreak
					);
				float newStepAngle = random.get(angle - allowedAngleVariation, angle + allowedAngleVariation);
				if (i == 0 || 
						std::fabs(FMath::angleDelta(newStepAngle, attractionAngle)) < 
						std::fabs(FMath::angleDelta(stepAngle, attractionAngle))
					) 
				{
					stepAngle = newStepAngle;
				}
			}
			Point step = Point::fromAngle(stepAngle, prototypes->variables.routeStepSize) + data.route[data.index];
				
			
			if (prototypes->obstacleMap.getInCell(step).size() > 0)
			{
				data.index--;
				data.failureStreak++;
			}
			else
			{
				data.index++;
				if (data.route.size() > (size_t)data.index)
					data.route[data.index] = step;
				else
					data.route.push_back(step);
				data.failureStreak = 0;
			}
		}
	}
}