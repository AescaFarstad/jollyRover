#include <Particle.h>
#include <FMath.h>

void Particle::render(Renderer* renderer, int32_t time)
{
	time -= delay;
	if (time < 0 || time > duration)
		return;
		
	current.location = FMath::lerp(0, from.location, duration, to.location, time);
	current.rotation = FMath::lerp(0, from.rotation, duration, to.rotation, time);
	current.scale = FMath::lerp(0, from.scale, duration, to.scale, time);
	current.tint = FMath::lerp(0, from.tint, duration, to.tint, time);
		
	renderer->blit(*texture, current);
		
}