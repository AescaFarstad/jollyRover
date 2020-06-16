#include <SerialAnimationView.h>
#include <FMath.h>

template<typename T>
void SerialAnimationView::render(Renderer* renderer, T& event, GameState* state, Prototypes* prototypes, int32_t thisPlayer)
{	
	int32_t seed = FMath::q_sdbm(event.id * event.stamp);
	if (m_seed != seed)
		init(seed, event, state, prototypes, thisPlayer);
		
	if (state->time.time > m_endTime)
		return;
	for(auto& p : particles)
	{
		p.render(renderer, state->time.time - m_startTime);
	}	
}

void SerialAnimationView::initFragAnimation(UnitDeathEvent& event, SeededRandom& random, GPU_Rect& origin, int32_t startTime)
{
	Particle p;
	p.sequence = &S::sequences.explosion;
	p.delay = 0;
	p.duration = 300;
	m_endTime = startTime + p.duration;
	p.from.location = event.location;
	p.from.location.x += random.get(-10.f, 10.f);
	p.from.location.y += random.get(-10.f, 10.f);
	p.from.rotation = random.get(0.f, 2*M_PI);
	p.from.scale = 1;
	p.from.tint = ViewUtil::colorFromHex(0xffffff, 0xff);
	p.to = p.from;
	particles.push_back(p);
	
	int32_t wShreds = random.get(8, 11);
	int32_t hShreds = random.get(8, 11);
	
	p.sequence = nullptr;
	shreds.reserve(wShreds * hShreds);
	
	int32_t baseDuration = random.get(400, 900);
		
	for(int32_t i = 0; i < wShreds; i++)
		{
			for(int32_t j = 0; j < hShreds; j++)
			{
				shreds.emplace_back();
				TextureDef& td = shreds.back();
				
				td.rect.x = origin.x + origin.w * i / wShreds;
				td.rect.y = origin.y + origin.h * j / hShreds;
				td.rect.w = origin.w / wShreds;
				td.rect.h = origin.h / hShreds;
				
				p.sequence = nullptr;
				p.texture = &td;
				p.duration = baseDuration + random.get(0, 600);
				
				p.from.rotation = event.rotation;
				
				Point c2c = Point(origin.w * (i + 0.5) / wShreds,  origin.h * (j + 0.5) / hShreds) - Point(origin.w/2, origin.h/2);
				c2c.rotate(p.from.rotation, c2c);  
				p.from.location = event.location + c2c;
				
				p.from.scale = 1;
				p.from.tint = ViewUtil::colorFromHex(0xdddddd, 0xff);
				
				if (event.impact.getLength() > 0)
				{
					auto imp = event.impact;
					imp.scaleTo(origin.w/3 + origin.h/3);
					c2c += imp;
				}	
				c2c.scaleBy(random.get(1.f, 3.f));
				c2c.rotate(random.get(-0.05f, 0.05f));
				
				p.to = p.from;
				p.to.location = event.location + c2c;
				p.to.tint = ViewUtil::colorFromHex(0x666666, 0x11);
				if (random.get() > 0.7)
					p.to.rotation += random.get((float)-M_PI * 10, (float)M_PI * 10); 
				
				m_endTime = std::max(m_endTime, startTime + p.delay + p.duration);
				particles.push_back(p);
			}
		}
}

void SerialAnimationView::init(int32_t seed, CreepDeathEvent& event, GameState* state, Prototypes* prototypes, int32_t thisPlayer)
{	
	m_startTime = event.stamp;
	m_seed = seed;
	SeededRandom random(seed);
	
	if (event.unitDeath.prototypeId == 0 || event.unitDeath.prototypeId == 4)
	{		
		GPU_Rect& origin = prototypes->creeps[event.unitDeath.prototypeId].hullTexture[event.force]->rect;
		initFragAnimation(event.unitDeath, random, origin, m_startTime);
	}
	else
	{
		Particle p;
		p.texture = &S::textures.tanks_1.Smoke.smokeOrange0;
		p.delay = 0;
		p.duration = random.get(4500, 14500);
		m_endTime = m_startTime + p.duration;
		p.from.location = event.unitDeath.location;
		p.from.rotation = random.get(0.f, 2*M_PI);
		p.from.scale = 0.1;
		p.from.tint = ViewUtil::colorFromHex(0xffffff, 0xff);		
		p.to = p.from;
		p.to.tint.a = 0x10;
		p.to.scale = 0.2;
		particles.push_back(p);
	}
}

void SerialAnimationView::init(int32_t seed, CarDeathEvent& event, GameState* state, Prototypes* prototypes, int32_t thisPlayer)
{	
	m_startTime = event.stamp;
	m_seed = seed;
	SeededRandom random(seed);
	
	GPU_Rect& origin = event.player == thisPlayer ?
			prototypes->cars[event.unitDeath.prototypeId].playerCarHullTexture->rect :
			prototypes->cars[event.unitDeath.prototypeId].opponentCarHullTexture->rect;
			
	initFragAnimation(event.unitDeath, random, origin, m_startTime);
}



void SerialAnimationView::init(int32_t seed, ProjectileExplosionEvent& event, GameState* state, Prototypes* prototypes, int32_t thisPlayer)
{
	m_startTime = event.stamp;
	m_endTime = 0;
	m_seed = seed;
	auto& weapon = prototypes->weapons[event.prototypeId];
	SeededRandom random(seed);
	
	int32_t numParticles;
	if (weapon.splash < 3)
		numParticles = random.get() > 0.8 ? 1 : 0;
	else if (weapon.splash < 10)		
		numParticles = random.get(2, 4);
	else		
		numParticles = random.get(3, 6);
	
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
		
		if (weapon.splash < 3)
		{
			p.from.scale *= 0.7;
			p.from.tint.a *= 0.7;
			p.to.scale *= 0.7;
		}
		
		m_endTime = std::max(m_endTime, p.delay + p.duration);
		particles.push_back(p);
	}
	m_endTime += m_startTime;
	
}


template void SerialAnimationView::render<ProjectileExplosionEvent>(Renderer* renderer, ProjectileExplosionEvent& event, GameState* state, Prototypes* prototypes, int32_t thisPlayer);
template void SerialAnimationView::render<CreepDeathEvent>(Renderer* renderer, CreepDeathEvent& event, GameState* state, Prototypes* prototypes, int32_t thisPlayer);
template void SerialAnimationView::render<CarDeathEvent>(Renderer* renderer, CarDeathEvent& event, GameState* state, Prototypes* prototypes, int32_t thisPlayer);
	