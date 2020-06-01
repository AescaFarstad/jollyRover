#pragma once
#include <string>
#include <Point.h>
#include <NFont.h>
#include <SDL_gpu.h>


class FlyingMessage
{
public:
	FlyingMessage();
	FlyingMessage(std::string message, Point origin, int32_t stamp, NFont::AlignEnum aligment = NFont::AlignEnum::LEFT);
	
	void render(int32_t stamp, NFont& font, GPU_Target* screen);
	
private:
	std::string m_message;
	Point m_origin;
	int32_t m_stamp;
	NFont::Effect m_effect;
	
	static constexpr int32_t LIFETIME = 2000;
	static constexpr int32_t MOVEMENT = -170;
	static constexpr float ACCEL = 3.5;
	static constexpr float FADE_POINT = 0.65;
	

};
