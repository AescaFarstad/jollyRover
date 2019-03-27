#include <SpatialMap.h>
#include <SeededRandom.h>
#include "SDL2/SDL.h"

class TestObject
{
public:
	TestObject(float x, float y)
	{
		location.x = x;
		location.y = y;
	}
	Point location;
	
		
	const Point& getLocation(){return location;}
};

void runTest()
{
	SpatialMap<TestObject> map(100, Point(-1000, -1000), Point(1000, 1000));
	SeededRandom random(124124);
	
	const int numObjects = 10000;
	std::vector<TestObject> vec;
	vec.reserve(numObjects);
	for(size_t i = 0; i < numObjects; i++)
	{
		vec.push_back(TestObject(random.get(-1040, 1040), random.get(-1040, 1040)));
	}
	
	const int numQueries = 10000;
	
	Point points[numQueries];
	int32_t rads[numQueries];
	
	for(size_t i = 0; i < numQueries; i++)
	{
		points[i] = (random.get(-1040, 1040), random.get(-1040, 1040));
		rads[i] = random.get(1, std::sqrt(std::sqrt(300)));
		rads[i]*=rads[i];
		rads[i]*=rads[i];
	}
	
	int start = SDL_GetTicks();
	map.set(vec);
	
	int mid = SDL_GetTicks();
	for(size_t i = 0; i < numQueries; i++)
	{
		auto result = map.getInRadius(points[i], rads[i]);
	}
	
	int op = SDL_GetTicks();
	for(size_t i = 0; i < numQueries; i++)
	{
		std::vector<TestObject*> brute;
		for(auto& it : vec)
		{
			if (points[i].distanceTo(it.location) < rads[i])
				brute.push_back(&it);
		}
	}
	
	int end = SDL_GetTicks();
	
	std::cout << "init:  " << mid - start << "\n";
	std::cout << "map:   " << op - mid << "\n";
	std::cout << "brute: " << end - op << "\n";
	
	for(size_t i = 0; i < numQueries; i++)
	{
		
		auto result = map.getInRadius(points[i], rads[i]);
		std::vector<TestObject*> brute;
		for(auto& it : vec)
		{
			if (points[i].distanceTo(it.location) < rads[i])
				brute.push_back(&it);
		}
		
		if (result.size() != brute.size())
		{
			std::cout << "ERRORS DETECTED!\n";
			std::cout << points[i].toString() << " " << rads[i] << ":\t" << result.size() << " vs " << brute.size() << "\n";
			for(auto& it : result)
				std::cout << "\t" << it->location.toString() << "\n";
			std::cout << "\n";
			for(auto& it : brute)
				std::cout << "\t" << it->location.toString() << "\n";
			std::cout << "\n";
			std::cout << "\n";
		}
	}
	
	
}