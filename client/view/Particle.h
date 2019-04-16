#pragma once
#include <Renderer.h>


class Particle
{
public:
	Particle() = default;
	
	int32_t delay;
	int32_t duration;
	TextureDef* texture;
	SequenceDef* sequence;
	TweenParams from;
	TweenParams current;
	TweenParams to;
	
	void render(Renderer* renderer, int32_t time);
	
};
