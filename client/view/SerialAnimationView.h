#pragma once
#include <Renderer.h>
#include <GameState.h>
#include <EventLogger.h>
#include <Particle.h>

class SerialAnimationView
{
public:
	uint32_t lastUpdate;
	
	template<typename T>
	void render(Renderer* renderer, T& event, GameState* state, Prototypes* prototypes, int32_t thisPlayer);
	void init(int32_t id, ProjectileExplosionEvent& event, GameState* state, Prototypes* prototypes, int32_t thisPlayer);
	void init(int32_t id, CreepDeathEvent& event, GameState* state, Prototypes* prototypes, int32_t thisPlayer);
	void init(int32_t id, CarDeathEvent& event, GameState* state, Prototypes* prototypes, int32_t thisPlayer);
	
private:
	int32_t m_startTime;
	int32_t m_endTime;
	int32_t m_seed;
	std::vector<Particle> particles;
	std::vector<TextureDef> shreds;
	
	void initFragAnimation(UnitDeathEvent& event, SeededRandom& random, GPU_Rect& origin, int32_t startTime);
	
};

extern template void SerialAnimationView::render<ProjectileExplosionEvent>(Renderer* renderer, ProjectileExplosionEvent& event, GameState* state, Prototypes* prototypes, int32_t thisPlayer);
extern template void SerialAnimationView::render<CreepDeathEvent>(Renderer* renderer, CreepDeathEvent& event, GameState* state, Prototypes* prototypes, int32_t thisPlayer);
extern template void SerialAnimationView::render<CarDeathEvent>(Renderer* renderer, CarDeathEvent& event, GameState* state, Prototypes* prototypes, int32_t thisPlayer);