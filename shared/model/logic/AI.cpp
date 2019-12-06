#include <AI.h>
#include <GameLogic.h>

namespace AI
{		
	int32_t boundary = 100;
	float boundaryPower = 3;
	float desireBase = 10;
	
	std::vector<Point> getRandomWalk(GameState* state, Prototypes* prototypes)
	{
		std::vector<Point> result;
		
		while(!GameLogic::testRouteIsValid(result, prototypes))
		{
			result.clear();
			result.push_back(Point(state->random.get(0, prototypes->variables.fieldWidth), prototypes->variables.fieldHeight));
			result.push_back(Point(result[0].x, result[0].y - prototypes->variables.routeStepSize));
			AIInternal::makeStep(result, 1, 0, state->random, prototypes);
		}		
		
		return result;
	}
	
	
	
	namespace AIInternal
	{
		
	
		void makeStep(std::vector<Point>& route, size_t index, int32_t failureStreak, SeededRandom& random,  Prototypes* prototypes)
		{			
			Point attraction = Point(0, 0);
			auto& last = route[index];
			if (last.y > prototypes->variables.fieldHeight)
			{
				last.y = prototypes->variables.fieldHeight;
				return;
			}
			
			if (last.x < boundary)
				attraction += Point(FMath::nlerp(0, 0, boundary, 100, boundary - last.x, boundaryPower), 0);
			if (last.x > prototypes->variables.fieldWidth - boundary)
				attraction += Point(- FMath::nlerp(0, 0, boundary, 100, boundary - (prototypes->variables.fieldWidth - last.x), boundaryPower), 0);
			if (last.y < boundary)
				attraction += Point(0, FMath::nlerp(0, 0, boundary, 100, boundary - last.y, boundaryPower));
			/*if (last.x > prototypes->variables.fieldHeight - boundary)
				attraction += Point(- FMath::nlerp(0, 0, boundary, 100, boundary - (prototypes->variables.fieldHeight - last.y), boundaryPower), 0);*/
			
			if (index > (size_t)prototypes->variables.maxRouteSteps / 20)
			{
				attraction += Point(0, 1);
			}
			if (index < (size_t)prototypes->variables.minRouteSteps)
			{
				attraction += Point(0, -10);
			}
			if (index < 50)
			{
				attraction += Point(0, -1);
			}
			
			float stepAngle;
			float attractionAngle;
			int32_t attractionStrength = 1;
			float angle = (route[index] - route[index - 1]).asAngle();
			if (attraction.getLength() != 0)
			{
				attractionStrength = 2 + std::sqrt(attraction.getLength());
				attractionAngle = attraction.asAngle();
			}
			
			
			for(int32_t i = 0; i < attractionStrength; i++)
			{
				float newStepAngle = random.get(angle - prototypes->variables.stepAngleWindow / 4, angle + prototypes->variables.stepAngleWindow / 4);
				if (i == 0 || 
						std::fabs(FMath::angleDelta(newStepAngle, attractionAngle)) < 
						std::fabs(FMath::angleDelta(stepAngle, attractionAngle))
					) 
				{
					stepAngle = newStepAngle;
				}
			}
			Point step = Point::fromAngle(stepAngle, prototypes->variables.routeStepSize) + route[index];
				
			
			if (prototypes->obstacleMap.getInCell(step).size() > 0)
			{
				index--;
				failureStreak++;
				makeStep(route, index, failureStreak, random, prototypes);
			}
			else
			{
				index++;
				if (route.size() > index)
					route[index] = step;
				else
					route.push_back(step);
				if (failureStreak > 0)
					S::log.add("fail streak: " + std::to_string(failureStreak));
				failureStreak = 0;
				makeStep(route, index, failureStreak, random, prototypes);
			}
			
			
			//сделать шаг
			//проверить препятствие
			//если всё плохо, то инкремент и откат
			//и переделать рекурсию в итерации
		}
	}
}