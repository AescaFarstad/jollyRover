#pragma once
#include <Keyboard.h>
#include <Network.h>
#include <GameUpdater.h>
#include <GameKeyboardActions.h>



class GameKeyboardInput
{
public:
	GameKeyboardInput();
	
	GAME_KEYBOARD_ACTIONS actionByButton[128];
	uint16_t buttonByAction[128];
		
	void handleKeyDown(SDL_Scancode scancode, Keyboard& keyboard, Network& network, GameUpdater& gameUpdater);
	void handleKeyUp(SDL_Scancode scancode, Keyboard& keyboard, Network& network, GameUpdater& gameUpdater);
};