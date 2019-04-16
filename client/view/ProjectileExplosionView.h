#pragma once
#include <Renderer.h>
#include <GameState.h>
#include <MultiParticle.h>

class ProjectileExplosionView
{
public:
	ProjectileExplosionView() = default;
	
	uint32_t lastUpdate;
	
	void render(Renderer* renderer, ProjectileExplosionEvent& event, GameState* state, Prototypes* prototypes);
	void init(int32_t id, ProjectileExplosionEvent& event, GameState* state);
	
private:
	int32_t m_startTime;
	int32_t m_endTime;
	int32_t m_seed;
	std::vector<Particle> particles;
	
};
