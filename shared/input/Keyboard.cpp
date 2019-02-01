#include <Keyboard.h>



Keyboard::Keyboard()
{
	for (size_t i = 0; i < 128; i++)
	{
		isDown[i] = false;
		actionByButton[i] = KEYBOARD_ACTIONS::NONE;
	}

	actionByButton[4] = KEYBOARD_ACTIONS::LEFT;
	actionByButton[26] = KEYBOARD_ACTIONS::FORWARD;
	actionByButton[7] = KEYBOARD_ACTIONS::RIGHT;
	actionByButton[22] = KEYBOARD_ACTIONS::BACKWARD;

}


Keyboard::~Keyboard()
{
}
