#pragma once
#include <Renderer.h>
#include <GameState.h>
#include <EventLogger.h>
#include <Particle.h>
#include <CreepView.h>

class SerialAnimationView
{
public:
	uint32_t lastUpdate;
	
	void init(int32_t id, ProjectileExplosionEvent& event, GameState* state, Prototypes* prototypes, int32_t thisPlayer);
	void init(int32_t id, CreepDeathEvent& event, GameState* state, Prototypes* prototypes, int32_t thisPlayer, CreepView* creepView = nullptr);
	void init(int32_t id, CarDeathEvent& event, GameState* state, Prototypes* prototypes, int32_t thisPlayer);
	
	template<typename T>
	void render(Renderer* renderer, T& event, uint32_t stamp, std::function<void(T& event, uint32_t seed, SerialAnimationView& view)> init)
	{	
		uint32_t seed = FMath::q_sdbm(event.id * event.stamp);
		if (m_seed != seed)
			init(event, seed, *this);
			
		if (stamp > m_endTime)
			return;
		for(auto& p : particles)
		{
			p.render(renderer, stamp - m_startTime);
		}	
	}
	
private:
	uint32_t m_startTime;
	uint32_t m_endTime;
	uint32_t m_seed;
	std::vector<Particle> particles;
	std::vector<TextureDef> shreds;
	
	void initFragAnimation(UnitDeathEvent& event, SeededRandom& random, GPU_Rect& origin, uint32_t startTime);
	
};