#pragma once
#include <unordered_map>
#include <GameState.h>
#include <Prototypes.h>
#include <Renderer.h>
#include <SDL_gpu.h>

template <typename V>
class ViewController
{
public:
	ViewController() = default;
	~ViewController() = default;
	
	
	template<typename T>
	void render(Renderer* renderer, T begin, T end, GameState* state, Prototypes* prototypes, int32_t thisPlayer)
	{
		size_t updateCount = 0;
		while(begin != end)
		{
			auto& view = m_views[begin->getId()];
			view.render(renderer, *begin, state, prototypes, thisPlayer);
			view.lastUpdate = state->timeStamp;
			++updateCount;
			++begin;
		}
		if (updateCount < m_views.size() - CLEAN_UP_SIZE)
		{
			for(auto it = m_views.begin(); it != m_views.end(); )
			{
				if(it->second.lastUpdate != state->timeStamp)
					it = m_views.erase(it);
				else
					++it;
			}
		}
	}
	
	void clear()
	{
		m_views.clear();
	}
	
	
private:
	static const size_t CLEAN_UP_SIZE = 100;
	std::unordered_map<int16_t, V> m_views;
};
