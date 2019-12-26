#include <FlyingMessage.h>

FlyingMessage::FlyingMessage(std::string message, Point origin, int32_t stamp)
{
	m_message = message;
	m_origin = origin;
	m_stamp = stamp;
}

FlyingMessage::FlyingMessage()
{
	m_stamp = -99999;
}

void FlyingMessage::render(int32_t stamp, NFont& font, GPU_Target* screen)
{
	if (stamp > m_stamp + LIFETIME)
		return;
		
	Point loc = m_origin;
	loc.y = FMath::nlerp(m_stamp, m_origin.y, m_stamp + LIFETIME, m_origin.y + MOVEMENT, stamp, ACCEL);
	
	NFont::Color c(0, 0, 0, 255);
	float fadeRatio = (float)(stamp - m_stamp) / LIFETIME;
	if (fadeRatio > FADE_POINT)
	{
		c.a = FMath::lerp(FADE_POINT, (uint8_t)255, 1.f, (uint8_t)0, fadeRatio);
	}
	font.draw(screen, loc.x, loc.y, c, "%s", m_message.c_str());
}