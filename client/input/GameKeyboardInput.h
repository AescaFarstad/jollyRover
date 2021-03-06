#pragma once
#include <KeyboardInputContext.h>
#include <GameUpdater.h>
#include <GameKeyboardActions.h>


class GameKeyboardInput
{
public:
	GameKeyboardInput();
	
	GAME_KEYBOARD_ACTIONS actionByButton[128];
	uint16_t buttonByAction[128];
		
	void onKeyDown(SDL_Scancode scancode, const KeyboardInputContext& context, GameUpdater& gameUpdater);
	void onKeyUp(SDL_Scancode scancode, const KeyboardInputContext& context, GameUpdater& gameUpdater);
};