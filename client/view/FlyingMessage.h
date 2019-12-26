#include <string>
#include <Point.h>
#include <NFont.h>
#include <SDL_gpu.h>


class FlyingMessage
{
public:
	FlyingMessage();
	FlyingMessage(std::string message, Point origin, int32_t stamp);
	
	void render(int32_t stamp, NFont& font, GPU_Target* screen);
	
private:
	std::string m_message;
	Point m_origin;
	int32_t m_stamp;
	
	static constexpr int32_t LIFETIME = 2000;
	static constexpr int32_t MOVEMENT = -170;
	static constexpr int32_t ACCEL = 3.5;
	static constexpr float FADE_POINT = 0.65;
	

};
