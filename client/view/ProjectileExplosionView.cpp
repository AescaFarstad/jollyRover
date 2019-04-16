#include <ProjectileExplosionView.h>
#include <FMath.h>

template<typename T>
void ProjectileExplosionView::render(Renderer* renderer, T& event, GameState* state, Prototypes* prototypes)
{	
	int32_t seed = FMath::q_sdbm(event.id);
	if (m_seed != seed)
		init(seed, event, state);
		
	if (state->time.time > m_endTime)
		return;
	for(auto& p : particles)
	{
		p.render(renderer, state->time.time - m_startTime);
	}	
}

void ProjectileExplosionView::init(int32_t seed, ProjectileExplosionEvent& event, GameState* state)
{	
	m_startTime = state->time.time;
	m_endTime = 0;
	m_seed = seed;
	SeededRandom random(seed);
	
	int32_t numParticles;
	if (event.prototypeId == 0)
		numParticles = random.get(3, 5);
	else
		numParticles = 1;
	
	
	for(int32_t i = 0; i < numParticles; i++)
	{
		Particle p;
		int32_t pictureIndex = random.get(1, 6);
		if (pictureIndex == 1)
			p.texture = &S::textures.tanks_1.Smoke.smokeGrey1;
		else if (pictureIndex == 2)
			p.texture = &S::textures.tanks_1.Smoke.smokeGrey2;
		else if (pictureIndex == 3)
			p.texture = &S::textures.tanks_1.Smoke.smokeGrey3;
		else if (pictureIndex == 4)
			p.texture = &S::textures.tanks_1.Smoke.smokeGrey4;
		else
			p.texture = &S::textures.tanks_1.Smoke.smokeGrey5;
			
		p.delay = i * 30 + random.get(-100, 100);
		if (p.delay < 0 || i < 3)
			p.delay = 0;
			
		p.duration = random.get(400, 1100);
		p.from.location = event.location;
		p.from.location.x += random.get(-10.f, 10.f);
		p.from.location.y += random.get(-10.f, 10.f);
		p.from.rotation = random.get(0.f, 2*M_PI);
		p.from.scale = random.get(0.1f, 0.2f);
		uint8_t darkening = random.get(0, 50);	
		p.from.tint.r = 0xff - darkening;
		p.from.tint.g = 0xff - darkening;
		p.from.tint.b = 0xff - darkening;
		p.from.tint.a = 0xff;
		
		p.to = p.from;
		if (random.get() > 0.7)
		{
			p.to.location.x += random.get(-10, 10);
			p.to.location.y += random.get(-10, 10);
		}
		if (random.get() > 0.7)
		{
			p.to.rotation += random.get(-0.5f, 0.5f);
		}
		
		p.to.scale += random.get(0.1f, 0.4f);
		p.to.tint.a = 0;
		
		m_endTime = std::max(m_endTime, p.delay + p.duration);
		particles.push_back(p);
	}
	m_endTime += m_startTime;
	
}

template void ProjectileExplosionView::render<ProjectileExplosionEvent>(Renderer* renderer, ProjectileExplosionEvent& event, GameState* state, Prototypes* prototypes);
	