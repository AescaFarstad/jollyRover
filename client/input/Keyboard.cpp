#include <Keyboard.h>
Keyboard::Keyboard()
{
	for (size_t i = 0; i < SDL_NUM_SCANCODES; i++)
	{
		isDown[i] = false;
	}
}
