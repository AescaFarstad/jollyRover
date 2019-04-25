#include <KeyboardInput.h>
#include <InputTimeMessage.h>
#include <LoadGameMessage.h>
#include <InputActionMessage.h>
#include <PersistentStorage.h>

namespace KeyboardInput
{
	void handleKeyDown(KEYBOARD_ACTIONS code, Keyboard& keyboard, Network& network, GameUpdater& gameUpdater)
	{
		switch (code)
		{
			case KEYBOARD_ACTIONS::TIME_STEP1 :
			{
				InputTimeMessage timeMsg;
				timeMsg.allowSteps = 1;	
				timeMsg.modifyAllowSteps = true;
				network.send(&timeMsg);
				break;
			};
			case KEYBOARD_ACTIONS::TIME_STEP5 :
			{
				InputTimeMessage timeMsg;
				timeMsg.allowSteps = 5;	
				timeMsg.modifyAllowSteps = true;
				network.send(&timeMsg);
				break;
			};
			case KEYBOARD_ACTIONS::TIME_STEP25 :
			{
				InputTimeMessage timeMsg;
				timeMsg.allowSteps = 25;	
				timeMsg.modifyAllowSteps = true;
				network.send(&timeMsg);
				break;
			};
			
			case KEYBOARD_ACTIONS::TIME_SCALE_T_03 :
			{
				InputTimeMessage timeMsg;
				timeMsg.forcedTimeScale = 0.3;
				timeMsg.forcedStepsAtOnce = 1;
				timeMsg.modifyForcedTimeScale = true;
				timeMsg.modifyForcedStepsAtOnce = true;
				network.send(&timeMsg);
				break;
			};
			case KEYBOARD_ACTIONS::TIME_SCALE_T_1 :
			{
				InputTimeMessage timeMsg;
				timeMsg.forcedTimeScale = 1;
				timeMsg.forcedStepsAtOnce = 1;
				timeMsg.modifyForcedTimeScale = true;
				timeMsg.modifyForcedStepsAtOnce = true;
				network.send(&timeMsg);
				break;
			};
			case KEYBOARD_ACTIONS::TIME_SCALE_T_5 :
			{
				InputTimeMessage timeMsg;
				timeMsg.forcedTimeScale = 3;
				timeMsg.forcedStepsAtOnce = 3;
				timeMsg.modifyForcedTimeScale = true;
				timeMsg.modifyForcedStepsAtOnce = true;
				network.send(&timeMsg);
				break;
			};
			
			case KEYBOARD_ACTIONS::TIME_SCALE03 :
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
			case KEYBOARD_ACTIONS::TIME_SCALE1 :
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
			case KEYBOARD_ACTIONS::TIME_SCALE5 :
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
			case KEYBOARD_ACTIONS::TIME_TOGGLE_PAUSE :
			{
				InputTimeMessage timeMsg;
				timeMsg.allowSteps = gameUpdater.state->time.allowedSteps > 0 ? -1 : 0;
				timeMsg.modifyAllowSteps = true;
				network.send(&timeMsg);
				break;
			};
			case KEYBOARD_ACTIONS::REVERT1 :
			{
				LoadGameMessage loadMsg;
				auto state = gameUpdater.getNewStateBySteps(gameUpdater.state->time.performedSteps - 1);
				auto stream = SerializationStream::createExp();
				state->serialize(*stream);
				loadMsg.stateLength = stream->getLength();
				loadMsg.state = stream->readAll();
				network.send(&loadMsg);
				break;
			};
			case KEYBOARD_ACTIONS::REVERT5 :
			{
				LoadGameMessage loadMsg;
				auto state = gameUpdater.getNewStateBySteps(gameUpdater.state->time.performedSteps - 5);
				auto stream = SerializationStream::createExp();
				state->serialize(*stream);
				loadMsg.stateLength = stream->getLength();
				loadMsg.state = stream->readAll();
				network.send(&loadMsg);
				break;
			};
			case KEYBOARD_ACTIONS::REVERT25 :
			{
				LoadGameMessage loadMsg;
				auto state = gameUpdater.getNewStateBySteps(gameUpdater.state->time.performedSteps - 25);
				auto stream = SerializationStream::createExp();
				state->serialize(*stream);
				loadMsg.stateLength = stream->getLength();
				loadMsg.state = stream->readAll();
				network.send(&loadMsg);
				break;
			};
			case KEYBOARD_ACTIONS::REVERT125 :
			{
				LoadGameMessage loadMsg;
				auto state = gameUpdater.getNewStateBySteps(gameUpdater.state->time.performedSteps - 125);
				auto stream = SerializationStream::createExp();
				state->serialize(*stream);
				loadMsg.stateLength = stream->getLength();
				loadMsg.state = stream->readAll();
				network.send(&loadMsg);
				break;
			};
			case KEYBOARD_ACTIONS::REVERT_FULL :
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
			case KEYBOARD_ACTIONS::RIGHT :
			case KEYBOARD_ACTIONS::LEFT :
			case KEYBOARD_ACTIONS::FORWARD :
			case KEYBOARD_ACTIONS::BACKWARD :
			{
				InputActionMessage im;

				im.downedButtons.push_back((int8_t)code);
				network.send(&im);
				break;
			}
			case KEYBOARD_ACTIONS::SAVE_GAME :
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
			case KEYBOARD_ACTIONS::LOAD_GAME :
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
			default:
			{
				S::log.add("action not handled: " + std::to_string((int)code), {LOG_TAGS::ERROR_});
				break;
			}	
		}
	}

	void handleKeyUp(KEYBOARD_ACTIONS code, Keyboard& keyboard, Network& network, GameUpdater& gameUpdater)
	{
		switch (code)
		{		
			case KEYBOARD_ACTIONS::TIME_SCALE_T_03 :
			case KEYBOARD_ACTIONS::TIME_SCALE_T_1 :
			case KEYBOARD_ACTIONS::TIME_SCALE_T_5 :
			{
				if (!keyboard.isDown[keyboard.buttonByAction[(int)KEYBOARD_ACTIONS::TIME_SCALE_T_03]] &&
					!keyboard.isDown[keyboard.buttonByAction[(int)KEYBOARD_ACTIONS::TIME_SCALE_T_1]] &&
					!keyboard.isDown[keyboard.buttonByAction[(int)KEYBOARD_ACTIONS::TIME_SCALE_T_5]]
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
			case KEYBOARD_ACTIONS::TIME_SCALE03 :
			case KEYBOARD_ACTIONS::TIME_SCALE1 :
			case KEYBOARD_ACTIONS::TIME_SCALE5 :
			case KEYBOARD_ACTIONS::TIME_STEP1 :
			case KEYBOARD_ACTIONS::TIME_STEP5 :
			case KEYBOARD_ACTIONS::TIME_STEP25 :
			case KEYBOARD_ACTIONS::TIME_TOGGLE_PAUSE :
			case KEYBOARD_ACTIONS::REVERT1 :
			case KEYBOARD_ACTIONS::REVERT5 :
			case KEYBOARD_ACTIONS::REVERT25 :
			case KEYBOARD_ACTIONS::REVERT125 :
			case KEYBOARD_ACTIONS::LOAD_GAME :
			case KEYBOARD_ACTIONS::SAVE_GAME :
			break;
			
			case KEYBOARD_ACTIONS::RIGHT :
			case KEYBOARD_ACTIONS::LEFT :
			case KEYBOARD_ACTIONS::FORWARD :
			case KEYBOARD_ACTIONS::BACKWARD :
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
}