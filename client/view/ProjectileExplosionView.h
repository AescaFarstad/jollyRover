#pragma once
#include <Renderer.h>
#include <GameState.h>
#include <EventLogger.h>
#include <Particle.h>

class ProjectileExplosionView
{
public:
	ProjectileExplosionView() = default;
	
	uint32_t lastUpdate;
	
	template<typename T>
	void render(Renderer* renderer, T& event, GameState* state, Prototypes* prototypes);
	void init(int32_t id, ProjectileExplosionEvent& event, GameState* state);
	void init(int32_t id, UnitDeathEvent& event, GameState* state);
	
private:
	int32_t m_startTime;
	int32_t m_endTime;
	int32_t m_seed;
	std::vector<Particle> particles;
	
};

extern template void ProjectileExplosionView::render<ProjectileExplosionEvent>(Renderer* renderer, ProjectileExplosionEvent& event, GameState* state, Prototypes* prototypes);