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
	template<typename T>
	void render(T begin, T end, uint32_t stamp, std::function<void(typename std::iterator_traits<T>::reference, V& )> renderMethod)
	{
		size_t updateCount = 0;
		while(begin != end)
		{
			auto& view = m_views[begin->getId()];
			renderMethod((*begin), view);
			view.lastUpdate = stamp;
			++updateCount;
			++begin;
		}
		if (updateCount < m_views.size() - CLEAN_UP_SIZE)
		{
			for(auto it = m_views.begin(); it != m_views.end(); )
			{
				if(it->second.lastUpdate != stamp)
					it = m_views.erase(it);
				else
					++it;
			}
		}
		
		m_lastUpdateStamp = stamp;
	}
	
	V* getView(int32_t id)
	{
		auto result = m_views.find(id);
		if (result != m_views.end()) // && result->lastUpdate == m_lastUpdateStamp
			return &result->second;
		return nullptr;
	}
	
	void clear()
	{
		m_views.clear();
	}
	
	
private:
	static constexpr size_t CLEAN_UP_SIZE = 100;
	std::unordered_map<int32_t, V> m_views;
	int32_t m_lastUpdateStamp = 0;
};
