#pragma once
#include <Keyboard.h>
#include <Network.h>
#include <GameUpdater.h>

namespace KeyboardInput
{
	void handleKeyDown(KEYBOARD_ACTIONS code, Keyboard& keyboard, Network& network, GameUpdater& gameUpdater);
	void handleKeyUp(KEYBOARD_ACTIONS code, Keyboard& keyboard, Network& network, GameUpdater& gameUpdater);
}