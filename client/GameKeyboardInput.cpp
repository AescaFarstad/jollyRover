#include <GameKeyboardInput.h>
#include <InputTimeMessage.h>
#include <LoadGameMessage.h>
#include <InputActionMessage.h>
#include <InputImpulseMessage.h>
#include <PersistentStorage.h>
#include <GameKeyboardActions.h>

#ifdef __EMSCRIPTEN__
	#include <SDL_scancode.h>
#else 
	#include <SDL2/SDL_scancode.h>
#endif

GameKeyboardInput::GameKeyboardInput()
{
	for (size_t i = 0; i < 128; i++)
	{
		actionByButton[i] = GAME_KEYBOARD_ACTIONS::NONE;
	}
	
	actionByButton[SDL_SCANCODE_A] = GAME_KEYBOARD_ACTIONS::LEFT;
	actionByButton[SDL_SCANCODE_W] = GAME_KEYBOARD_ACTIONS::FORWARD;
	actionByButton[SDL_SCANCODE_D] = GAME_KEYBOARD_ACTIONS::RIGHT;
	actionByButton[SDL_SCANCODE_S] = GAME_KEYBOARD_ACTIONS::BACKWARD;
	
	actionByButton[SDL_SCANCODE_KP_3] = GAME_KEYBOARD_ACTIONS::TIME_STEP1;
	actionByButton[SDL_SCANCODE_KP_6] = GAME_KEYBOARD_ACTIONS::TIME_STEP5;
	actionByButton[SDL_SCANCODE_KP_9] = GAME_KEYBOARD_ACTIONS::TIME_STEP25;
	
	actionByButton[SDL_SCANCODE_KP_2] = GAME_KEYBOARD_ACTIONS::TIME_SCALE_T_03;
	actionByButton[SDL_SCANCODE_KP_5] = GAME_KEYBOARD_ACTIONS::TIME_SCALE_T_1;
	actionByButton[SDL_SCANCODE_KP_8] = GAME_KEYBOARD_ACTIONS::TIME_SCALE_T_5;
	
	actionByButton[SDL_SCANCODE_KP_1] = GAME_KEYBOARD_ACTIONS::TIME_SCALE03;
	actionByButton[SDL_SCANCODE_KP_4] = GAME_KEYBOARD_ACTIONS::TIME_SCALE1;
	actionByButton[SDL_SCANCODE_KP_7] = GAME_KEYBOARD_ACTIONS::TIME_SCALE5;
	
	actionByButton[SDL_SCANCODE_KP_0] = GAME_KEYBOARD_ACTIONS::TIME_TOGGLE_PAUSE;
	actionByButton[SDL_SCANCODE_SPACE] = GAME_KEYBOARD_ACTIONS::TIME_TOGGLE_PAUSE;
	
	actionByButton[SDL_SCANCODE_KP_DIVIDE] = GAME_KEYBOARD_ACTIONS::REVERT1;
	actionByButton[SDL_SCANCODE_KP_MULTIPLY] = GAME_KEYBOARD_ACTIONS::REVERT5;
	actionByButton[SDL_SCANCODE_KP_MINUS] = GAME_KEYBOARD_ACTIONS::REVERT25;
	actionByButton[SDL_SCANCODE_KP_PLUS] = GAME_KEYBOARD_ACTIONS::REVERT125;
	
	actionByButton[SDL_SCANCODE_KP_PERIOD] = GAME_KEYBOARD_ACTIONS::REVERT_FULL;
	
	actionByButton[SDL_SCANCODE_PAGEUP] = GAME_KEYBOARD_ACTIONS::SAVE_GAME;
	actionByButton[SDL_SCANCODE_PAGEDOWN] = GAME_KEYBOARD_ACTIONS::LOAD_GAME;
	
	actionByButton[SDL_SCANCODE_SEMICOLON] = GAME_KEYBOARD_ACTIONS::ADD_AI;
	actionByButton[SDL_SCANCODE_L] = GAME_KEYBOARD_ACTIONS::CLEAR_AI;
	actionByButton[SDL_SCANCODE_BACKSLASH] = GAME_KEYBOARD_ACTIONS::ACT_AI;
	actionByButton[SDL_SCANCODE_APOSTROPHE] = GAME_KEYBOARD_ACTIONS::TOGGLE_AI;
	
	for(uint16_t i = 0; i < 128; i++)
		buttonByAction[(int)actionByButton[i]] = i;
}

void GameKeyboardInput::onKeyDown(SDL_Scancode scancode, Keyboard& keyboard, Network& network, GameUpdater& gameUpdater)
{
	GAME_KEYBOARD_ACTIONS code = actionByButton[scancode];
	switch (code)
	{
		case GAME_KEYBOARD_ACTIONS::TIME_STEP1 :
		{
			InputTimeMessage timeMsg;
			timeMsg.allowSteps = 1;	
			timeMsg.modifyAllowSteps = true;
			network.send(&timeMsg);
			break;
		};
		case GAME_KEYBOARD_ACTIONS::TIME_STEP5 :
		{
			InputTimeMessage timeMsg;
			timeMsg.allowSteps = 5;	
			timeMsg.modifyAllowSteps = true;
			network.send(&timeMsg);
			break;
		};
		case GAME_KEYBOARD_ACTIONS::TIME_STEP25 :
		{
			InputTimeMessage timeMsg;
			timeMsg.allowSteps = 25;	
			timeMsg.modifyAllowSteps = true;
			network.send(&timeMsg);
			break;
		};
		
		case GAME_KEYBOARD_ACTIONS::TIME_SCALE_T_03 :
		{
			InputTimeMessage timeMsg;
			timeMsg.forcedTimeScale = 0.3;
			timeMsg.forcedStepsAtOnce = 1;
			timeMsg.modifyForcedTimeScale = true;
			timeMsg.modifyForcedStepsAtOnce = true;
			network.send(&timeMsg);
			break;
		};
		case GAME_KEYBOARD_ACTIONS::TIME_SCALE_T_1 :
		{
			InputTimeMessage timeMsg;
			timeMsg.forcedTimeScale = 1;
			timeMsg.forcedStepsAtOnce = 1;
			timeMsg.modifyForcedTimeScale = true;
			timeMsg.modifyForcedStepsAtOnce = true;
			network.send(&timeMsg);
			break;
		};
		case GAME_KEYBOARD_ACTIONS::TIME_SCALE_T_5 :
		{
			InputTimeMessage timeMsg;
			timeMsg.forcedTimeScale = 3;
			timeMsg.forcedStepsAtOnce = 3;
			timeMsg.modifyForcedTimeScale = true;
			timeMsg.modifyForcedStepsAtOnce = true;
			network.send(&timeMsg);
			break;
		};
		
		case GAME_KEYBOARD_ACTIONS::TIME_SCALE03 :
		{
			InputTimeMessage timeMsg;
			timeMsg.timeScale = 0.3;
			timeMsg.modifyTimeScale = true;
			timeMsg.stepsAtOnce = 1;
			timeMsg.modifyStepsAtOnce = true;
			timeMsg.allowSteps = -1;
			timeMsg.modifyAllowSteps = true;
			network.send(&timeMsg);
			break;
		};
		case GAME_KEYBOARD_ACTIONS::TIME_SCALE1 :
		{
			InputTimeMessage timeMsg;
			timeMsg.timeScale = 1;
			timeMsg.modifyTimeScale = true;
			timeMsg.stepsAtOnce = 1;
			timeMsg.modifyStepsAtOnce = true;
			timeMsg.allowSteps = -1;
			timeMsg.modifyAllowSteps = true;
			network.send(&timeMsg);
			break;
		};
		case GAME_KEYBOARD_ACTIONS::TIME_SCALE5 :
		{
			InputTimeMessage timeMsg;
			timeMsg.timeScale = 3;
			timeMsg.modifyTimeScale = true;
			timeMsg.stepsAtOnce = 3;
			timeMsg.modifyStepsAtOnce = true;
			timeMsg.allowSteps = -1;
			timeMsg.modifyAllowSteps = true;
			network.send(&timeMsg);
			break;
		};
		case GAME_KEYBOARD_ACTIONS::TIME_TOGGLE_PAUSE :
		{
			InputTimeMessage timeMsg;
			timeMsg.allowSteps = gameUpdater.state->time.allowedSteps > 0 ? -1 : 0;
			timeMsg.modifyAllowSteps = true;
			network.send(&timeMsg);
			break;
		};
		case GAME_KEYBOARD_ACTIONS::REVERT1 :
		{
			LoadGameMessage loadMsg;
			auto state = gameUpdater.getNewStateBySteps(std::max(0, gameUpdater.state->time.performedSteps - 1));
			if (!state)
				return;
			auto stream = SerializationStream::createExp();
			state->serialize(*stream);
			loadMsg.stateLength = stream->getLength();
			loadMsg.state = stream->readAll();
			network.send(&loadMsg);
			break;
		};
		case GAME_KEYBOARD_ACTIONS::REVERT5 :
		{
			LoadGameMessage loadMsg;
			auto state = gameUpdater.getNewStateBySteps(std::max(0, gameUpdater.state->time.performedSteps - 5));
			if (!state)
				return;
			auto stream = SerializationStream::createExp();
			state->serialize(*stream);
			loadMsg.stateLength = stream->getLength();
			loadMsg.state = stream->readAll();
			network.send(&loadMsg);
			break;
		};
		case GAME_KEYBOARD_ACTIONS::REVERT25 :
		{
			LoadGameMessage loadMsg;
			auto state = gameUpdater.getNewStateBySteps(std::max(0, gameUpdater.state->time.performedSteps - 25));
			if (!state)
				return;
			auto stream = SerializationStream::createExp();
			state->serialize(*stream);
			loadMsg.stateLength = stream->getLength();
			loadMsg.state = stream->readAll();
			network.send(&loadMsg);
			break;
		};
		case GAME_KEYBOARD_ACTIONS::REVERT125 :
		{
			LoadGameMessage loadMsg;
			auto state = gameUpdater.getNewStateBySteps(std::max(0, gameUpdater.state->time.performedSteps - 125));
			if (!state)
				return;
			auto stream = SerializationStream::createExp();
			state->serialize(*stream);
			loadMsg.stateLength = stream->getLength();
			loadMsg.state = stream->readAll();
			network.send(&loadMsg);
			break;
		};
		case GAME_KEYBOARD_ACTIONS::REVERT_FULL :
		{
			LoadGameMessage loadMsg;
			auto state = gameUpdater.getFirstState();
			auto stream = SerializationStream::createExp();
			state->serialize(*stream);
			loadMsg.stateLength = stream->getLength();
			loadMsg.state = stream->readAll();
			network.send(&loadMsg);
			break;
		};
		case GAME_KEYBOARD_ACTIONS::RIGHT :
		case GAME_KEYBOARD_ACTIONS::LEFT :
		case GAME_KEYBOARD_ACTIONS::FORWARD :
		case GAME_KEYBOARD_ACTIONS::BACKWARD :
		{
			InputActionMessage im;

			im.downedButtons.push_back((int8_t)code);
			network.send(&im);
			break;
		}
		case GAME_KEYBOARD_ACTIONS::SAVE_GAME :
		{
			if (!S::persistentStorage.isReady())
			{
				S::log.add("Can't save the game - persistent storage is not ready yet", {LOG_TAGS::ERROR_});
				return;
			}
				
			S::persistentStorage.savedState = Serializer::copyThroughSerialization(*gameUpdater.state.get());
			S::persistentStorage.commit();
			S::log.add("Saved", {LOG_TAGS::UNIQUE});
			break;
		}
		case GAME_KEYBOARD_ACTIONS::LOAD_GAME :
		{
			if (!S::persistentStorage.isReady())
			{
				S::log.add("Can't load the game - persistent storage is not ready yet", {LOG_TAGS::ERROR_});
				return;
			}
			
			if (!S::persistentStorage.savedState)
			{
				S::log.add("Nothing to load", {LOG_TAGS::ERROR_});
				return;
			}
			
			LoadGameMessage loadMsg;
			auto state = gameUpdater.getFirstState();
			auto stream = SerializationStream::createExp();
			S::persistentStorage.savedState->serialize(*stream);
			loadMsg.stateLength = stream->getLength();
			loadMsg.state = stream->readAll();
			network.send(&loadMsg);
			break;
		}
		case GAME_KEYBOARD_ACTIONS::ADD_AI :
		{			
			InputImpulseMessage msg;
			msg.impulse = INPUT_IMPULSE::ADD_AI;
			network.send(&msg);			
			break;
		}
		case GAME_KEYBOARD_ACTIONS::CLEAR_AI :
		{			
			InputImpulseMessage msg;
			msg.impulse = INPUT_IMPULSE::CLEAR_AI;
			network.send(&msg);			
			break;
		}
		case GAME_KEYBOARD_ACTIONS::TOGGLE_AI :
		{			
			InputImpulseMessage msg;
			msg.impulse = INPUT_IMPULSE::TOGGLE_AI;
			network.send(&msg);			
			break;
		}
		case GAME_KEYBOARD_ACTIONS::ACT_AI :
		{			
			InputImpulseMessage msg;
			msg.impulse = INPUT_IMPULSE::ACT_AI;
			network.send(&msg);			
			break;
		}
		default:
		{
			S::log.add("action not handled: " + std::to_string((int)code), {LOG_TAGS::ERROR_});
			break;
		}	
	}
}

void GameKeyboardInput::onKeyUp(SDL_Scancode scancode, Keyboard& keyboard, Network& network, GameUpdater& gameUpdater)
{
	GAME_KEYBOARD_ACTIONS code = actionByButton[scancode];
	switch (code)
	{		
		case GAME_KEYBOARD_ACTIONS::TIME_SCALE_T_03 :
		case GAME_KEYBOARD_ACTIONS::TIME_SCALE_T_1 :
		case GAME_KEYBOARD_ACTIONS::TIME_SCALE_T_5 :
		{
			if (!keyboard.isDown[buttonByAction[(int)GAME_KEYBOARD_ACTIONS::TIME_SCALE_T_03]] &&
				!keyboard.isDown[buttonByAction[(int)GAME_KEYBOARD_ACTIONS::TIME_SCALE_T_1]] &&
				!keyboard.isDown[buttonByAction[(int)GAME_KEYBOARD_ACTIONS::TIME_SCALE_T_5]]
			)
			{
			InputTimeMessage timeMsg;
			timeMsg.forcedTimeScale = -1;
			timeMsg.forcedStepsAtOnce = -1;
			timeMsg.modifyForcedTimeScale = true;
			timeMsg.modifyForcedStepsAtOnce = true;
			network.send(&timeMsg);
			}
			break;
			
		};
		case GAME_KEYBOARD_ACTIONS::TIME_SCALE03 :
		case GAME_KEYBOARD_ACTIONS::TIME_SCALE1 :
		case GAME_KEYBOARD_ACTIONS::TIME_SCALE5 :
		case GAME_KEYBOARD_ACTIONS::TIME_STEP1 :
		case GAME_KEYBOARD_ACTIONS::TIME_STEP5 :
		case GAME_KEYBOARD_ACTIONS::TIME_STEP25 :
		case GAME_KEYBOARD_ACTIONS::TIME_TOGGLE_PAUSE :
		case GAME_KEYBOARD_ACTIONS::REVERT1 :
		case GAME_KEYBOARD_ACTIONS::REVERT5 :
		case GAME_KEYBOARD_ACTIONS::REVERT25 :
		case GAME_KEYBOARD_ACTIONS::REVERT125 :
		case GAME_KEYBOARD_ACTIONS::LOAD_GAME :
		case GAME_KEYBOARD_ACTIONS::SAVE_GAME :
		case GAME_KEYBOARD_ACTIONS::ADD_AI :
		case GAME_KEYBOARD_ACTIONS::CLEAR_AI :
		case GAME_KEYBOARD_ACTIONS::ACT_AI :
		case GAME_KEYBOARD_ACTIONS::TOGGLE_AI :
		break;
		
		case GAME_KEYBOARD_ACTIONS::RIGHT :
		case GAME_KEYBOARD_ACTIONS::LEFT :
		case GAME_KEYBOARD_ACTIONS::FORWARD :
		case GAME_KEYBOARD_ACTIONS::BACKWARD :
		{
			InputActionMessage im;

			im.uppedButtons.push_back((int8_t)code);
			network.send(&im);
			break;
		}
		default:
		{
			S::log.add("action not handled: " + std::to_string((int)code), {LOG_TAGS::ERROR_});
			break;
		}		
		
	}
}