#include <SerialAnimationView.h>
#include <FMath.h>



void SerialAnimationView::initFragAnimation(UnitDeathEvent& event, SeededRandom& random, GPU_Rect& origin, uint32_t startTime)
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
				c2c = c2c.rotate(p.from.rotation);
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
				c2c = c2c.rotate(random.get(-0.05f, 0.05f));
				
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

void SerialAnimationView::init(int32_t seed, CreepDeathEvent& event, GameState* state, Prototypes* prototypes, int32_t thisPlayer, CreepView* originalView)
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
		Particle blood;
		blood.texture = &S::textures.tanks_1.Smoke.smokeOrange0;
		blood.delay = 0;
		blood.duration = random.get(4500, 14500);
		m_endTime = m_startTime + blood.duration;
		blood.from.location = event.unitDeath.location;
		blood.from.rotation = random.get(0.f, 2*M_PI);
		blood.from.scale = 0.1;
		blood.from.tint = ViewUtil::colorFromHex(0xffffff, 0xff);		
		blood.to = blood.from;
		blood.to.tint.a = 0x10;
		blood.to.scale = 0.2;
		particles.push_back(blood);
		
		auto rotation = originalView ? originalView->getRotation() : 0;
		
		Particle corpse;
		corpse.texture = prototypes->creeps[event.unitDeath.prototypeId].hullTexture[event.force];
		corpse.delay = 0;
		corpse.duration = random.get(600, 1200);
		corpse.from.location = event.unitDeath.location;
		corpse.from.rotation = rotation;
		corpse.from.scale = 0.8;
		corpse.from.tint = ViewUtil::colorFromHex(0xfff5f5, 0xff);		
		corpse.to = corpse.from;
		corpse.to.tint.a = 0x0;
		corpse.to.scale = 0.1;
		corpse.to.tint = ViewUtil::colorFromHex(0xffcccc, 0xaa);	
		corpse.to.rotation = rotation + random.get(-(float)M_PI, (float)M_PI) / 3;
		particles.push_back(corpse);
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
		numParticles = random.get(3, 5);
	else		
		numParticles = random.get(4, 8);
	
	for(int32_t i = 0; i < numParticles; i++)
	{
		Particle p;
		int32_t pictureIndex = random.get(1, 15);
		if (pictureIndex < 2)
			p.texture = &S::textures.tanks_1.Smoke.smokeGrey1;
		else if (pictureIndex < 5)
			p.texture = &S::textures.tanks_1.Smoke.smokeGrey2;
		else if (pictureIndex < 8)
			p.texture = &S::textures.tanks_1.Smoke.smokeGrey3;
		else if (pictureIndex < 11)
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
		
		m_endTime = std::max(m_endTime, (uint32_t)(p.delay + p.duration));
		particles.push_back(p);
	}
	m_endTime += m_startTime;
	
}
