#pragma once
#include <cstdint>
#include <stdlib.h>

enum class KEYBOARD_ACTIONS : int8_t {
	NONE,
	LEFT,
	RIGHT,
	FORWARD,
	BACKWARD,
	TIME_STEP1,
	TIME_STEP5,
	TIME_STEP25,
	TIME_SCALE_T_03,
	TIME_SCALE_T_1,
	TIME_SCALE_T_5,
	TIME_SCALE03,
	TIME_SCALE1,
	TIME_SCALE5,
	TIME_TOGGLE_PAUSE,
	REVERT1,
	REVERT5,
	REVERT25,
	REVERT125,
	REVERT_FULL
	
	
};

class Keyboard
{
public:
	Keyboard();
	~Keyboard() = default;
	bool isDown[128];
	KEYBOARD_ACTIONS actionByButton[128];
	uint16_t buttonByAction[128];
};
