#include <Particle.h>
#include <FMath.h>

Particle::Particle()
{
	texture = nullptr;
	sequence = nullptr;
}

void Particle::render(Renderer* renderer, int32_t time)
{
	time -= delay;
	if (time < 0 || time > duration)
		return;
		
	current.location = FMath::lerp(0, from.location, duration, to.location, time);
	current.rotation = FMath::lerp(0, from.rotation, duration, to.rotation, time);
	current.scale = FMath::lerp(0, from.scale, duration, to.scale, time);
	current.tint = FMath::lerp(0, from.tint, duration, to.tint, time);
	
	if (sequence)
	{
		int32_t frame = FMath::lerp(0.f, 0.f, duration, sequence->frames.size(), time);
		renderer->blit(sequence->frames[frame], current);
	}
	else
	{
		renderer->blit(*texture, current);
	}
}