#include <Keyboard.h>
Keyboard::Keyboard()
{
	for (size_t i = 0; i < 128; i++)
	{
		isDown[i] = false;
	}
}
