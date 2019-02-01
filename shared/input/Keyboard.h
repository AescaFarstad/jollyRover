#pragma once
#include <cstdint>
#include <stdlib.h>

enum class KEYBOARD_ACTIONS : int8_t {
	NONE,
	LEFT,
	RIGHT,
	FORWARD,
	BACKWARD
};

class Keyboard
{
public:
	Keyboard();
	~Keyboard();
	bool isDown[128];
	KEYBOARD_ACTIONS actionByButton[128];
};
