#include <unordered_map>
#include <GameState.h>
#include <Prototypes.h>
#include <SDL_gpu.h>

template <typename T, typename V>
class ViewController
{
public:
	ViewController() = default;
	~ViewController() = default;
	
	
	void render(GPU_Target* screen, std::vector<T> &array, GameState* state, Prototypes* prototypes)
	{
		for(T& i : array)
			m_views[i.object.id].render(screen, i, state, prototypes);
	}
	
private:

	std::unordered_map<int16_t, V> m_views;
};
