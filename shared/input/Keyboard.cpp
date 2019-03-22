#include <Keyboard.h>

#ifdef __EMSCRIPTEN__
	#include <SDL_scancode.h>
#else 
	#include <SDL2/SDL_scancode.h>
#endif


Keyboard::Keyboard()
{
	for (size_t i = 0; i < 128; i++)
	{
		isDown[i] = false;
		actionByButton[i] = KEYBOARD_ACTIONS::NONE;
	}

	actionByButton[SDL_SCANCODE_A] = KEYBOARD_ACTIONS::LEFT;
	actionByButton[SDL_SCANCODE_W] = KEYBOARD_ACTIONS::FORWARD;
	actionByButton[SDL_SCANCODE_D] = KEYBOARD_ACTIONS::RIGHT;
	actionByButton[SDL_SCANCODE_S] = KEYBOARD_ACTIONS::BACKWARD;
	
	actionByButton[SDL_SCANCODE_KP_3] = KEYBOARD_ACTIONS::TIME_STEP1;
	actionByButton[SDL_SCANCODE_KP_6] = KEYBOARD_ACTIONS::TIME_STEP5;
	actionByButton[SDL_SCANCODE_KP_9] = KEYBOARD_ACTIONS::TIME_STEP25;
	
	actionByButton[SDL_SCANCODE_KP_2] = KEYBOARD_ACTIONS::TIME_SCALE_T_03;
	actionByButton[SDL_SCANCODE_KP_5] = KEYBOARD_ACTIONS::TIME_SCALE_T_1;
	actionByButton[SDL_SCANCODE_KP_8] = KEYBOARD_ACTIONS::TIME_SCALE_T_3;
	
	actionByButton[SDL_SCANCODE_KP_1] = KEYBOARD_ACTIONS::TIME_SCALE03;
	actionByButton[SDL_SCANCODE_KP_4] = KEYBOARD_ACTIONS::TIME_SCALE1;
	actionByButton[SDL_SCANCODE_KP_7] = KEYBOARD_ACTIONS::TIME_SCALE3;
	
	actionByButton[SDL_SCANCODE_KP_0] = KEYBOARD_ACTIONS::TIME_TOGGLE_PAUSE;
	actionByButton[SDL_SCANCODE_SPACE] = KEYBOARD_ACTIONS::TIME_TOGGLE_PAUSE;
	
	actionByButton[SDL_SCANCODE_KP_DIVIDE] = KEYBOARD_ACTIONS::REVERT1;
	actionByButton[SDL_SCANCODE_KP_MULTIPLY] = KEYBOARD_ACTIONS::REVERT5;
	actionByButton[SDL_SCANCODE_KP_MINUS] = KEYBOARD_ACTIONS::REVERT25;
	actionByButton[SDL_SCANCODE_KP_PLUS] = KEYBOARD_ACTIONS::REVERT125;
	
	for(uint16_t i = 0; i < 128; i++)
		buttonByAction[(int)actionByButton[i]] = i;

}
