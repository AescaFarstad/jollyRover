#include <Game.h>
#include <InputTimeMessage.h>
#include <LoopBackNetwork.h>
#include <NetworkMessageFactory.h>
#include <SerializationStream.h>
#include <GreetingMessage.h>
#include <GenericRequestMessage.h>
#include <ConsequtiveTask.h>
#include <string>
#include <istream>
#include <sstream>
#include <GameInputBinding.h>
#include <AnonymousBinding.h>
#include <json.hpp>
#include <fstream>
#include <memory>
#include <GameState.h>

Game::Game(SDL_Window* window, SDL_Renderer* renderer)
{
	loadPrototypes();

	gameView = new GameView(window, renderer, &prototypes);

	taskManager = new TaskManager();
	network = new LoopBackNetwork(&gameUpdater);
	network->connect();


	routeInput = new RouteInput();

	//addNetworkBindings();


	auto loadGameTask = std::make_unique<ConsequtiveTask>();
	//std::unique_ptr<ConsequtiveTask> loadGameTask(new ConsequtiveTask);

	loadGameTask->pushAsync([this](std::unique_ptr<Callback> callback) {
		auto binding = std::make_unique<AnonymousBinding>("wait for greetings prompt from the server and send it");

		auto handleRequest = std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this, cb = callback.release()](std::unique_ptr<NetworkMessage> message) {

			GreetingMessage gMsg;
			network->send(&gMsg);

			cb->execute();
			delete cb;
		});
		
		binding->
			bindByGenericType(RequestTypes::REQUEST_GREETING)->
			setCallOnce(true)->
			setHandler(std::move(handleRequest));
		
		network->genericRequestBinder.bind(std::move(binding));
		//binding->handle(std::make_unique<NetworkMessage>());
	}, "wait for greetings prompt from the server and send it");

	loadGameTask->pushAsync([this](std::unique_ptr<Callback> callback) {
		network->interceptOnce(MessageTypes::TYPE_GREETING_MSG, [this, cb = callback.release()](std::unique_ptr<NetworkMessage> message) {

			GreetingMessage* gMsg = dynamic_cast<GreetingMessage*>(message.release());
			login = gMsg->login;
			password = gMsg->password;
			delete gMsg;

			cb->execute();
			delete cb;
		});
		//network->binder.traceBindings("added #2");
	}, "wait for greetings response from the server");

	auto ping = [this](std::unique_ptr<Callback> callback) {
		network->interceptGenericRequestOnce(RequestTypes::REQUEST_PONG, [cb = callback.release()](std::unique_ptr<GenericRequestMessage> message) {
			
			cb->execute();
			delete cb;
		});

		GenericRequestMessage grMsg;
		grMsg.request = RequestTypes::REQUEST_PING;
		network->send(&grMsg);
	};

	loadGameTask->pushAsync(ping, "ping1");
	loadGameTask->pushAsync(ping, "ping2");
	loadGameTask->pushAsync(ping, "ping3");

	loadGameTask->pushSync([this]() {
		GenericRequestMessage grMsg;
		grMsg.request = RequestTypes::REQUEST_JOIN_GAME;
		network->send(&grMsg);
	}, "request to JOIN_GAME");	

	loadGameTask->pushAsync([this](std::unique_ptr<Callback> callback) {
		network->interceptOnce(MessageTypes::TYPE_GAME_STATE_MSG, [this, cb = callback.release()](std::unique_ptr<NetworkMessage> message) {

			GameStateMessage* gameStateMsg = dynamic_cast<GameStateMessage*>(message.release());
			clientToServerDelta = network->timeSync.localToServerUpperBound(0);
			S::log.add("server time delta: " + std::to_string(clientToServerDelta) + 
					" uncertainty: " + std::to_string(network->timeSync.getUncertainty()), { LOG_TAGS::NET });
			gameUpdater.load(std::unique_ptr<GameState>(gameStateMsg->state), &prototypes);
			gameStateMsg->ownsState = false;
			
			addNetworkBindings();

			cb->execute();
			delete cb;
		});
	}, "wait for game state from the server and load the game");

	loadGameTask->exec();


	taskManager->push(std::move(loadGameTask));
}

Game::~Game()
{
}

void Game::update()
{
	taskManager->update();
	network->update();

	if (gameUpdater.isLoaded)
	{
		gameUpdater.update(SDL_GetTicks() + clientToServerDelta);
		if (!routeInput->isLoaded)
		{	
			routeInput->load(gameUpdater.state.get(), &prototypes);
		}
		if (routeInput->isCompletelyValid)
		{
			auto route = routeInput->claimRoute();
			InputRouteMessage nim(route);
			network->send(&nim);
		}
		gameView->render(gameUpdater.state.get(), routeInput);
	}
}

void Game::handleEvent(SDL_Event* event)
{
	if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		routeInput->onMouseDown(&event->button);
		return;
	}

	if (event->type == SDL_MOUSEBUTTONUP)
	{
		routeInput->onMouseUp(&event->button);
		return;
	}

	if (event->type == SDL_MOUSEMOTION)
	{
		routeInput->onMouseMove(&event->motion);
		return;
	}
	
	if (
		(event->type != SDL_KEYDOWN && 
		 event->type != SDL_KEYUP) || false
		//keyboard.actionByButton[event->key.keysym.scancode] == KEYBOARD_ACTIONS::NONE
		)
		return;
	switch (event->type) 
	{
		case SDL_KEYDOWN:
			if (!keyboard.isDown[event->key.keysym.scancode])
			{
				//printf("Key press detected\n");
				keyboard.isDown[event->key.keysym.scancode] = true;
				handleKeyDown(keyboard.actionByButton[event->key.keysym.scancode]);
			}
			break;

		case SDL_KEYUP:
			if (keyboard.isDown[event->key.keysym.scancode])
			{
				//printf("Key release detected\n");
				keyboard.isDown[event->key.keysym.scancode] = false;
				handleKeyUp(keyboard.actionByButton[event->key.keysym.scancode]);
			}
			break;

		default:
			break;
	}
		
}


#ifdef __EMSCRIPTEN__
	#include <SDL_image.h>
#else 
	#include <SDL2/SDL_image.h>
#endif


void Game::loadPrototypes()
{
	std::ifstream file("out/prototypes.json");
	json j = json::parse(file);
	file.close();
	prototypes.load(j);
	
	//std::ifstream file2("out/assets/sheet_tanks.png");
}
int16_t idCounter = 0;
void Game::handleKeyDown(KEYBOARD_ACTIONS code)
{
	switch (code)
	{
		case KEYBOARD_ACTIONS::TIME_STEP1 :
		{
			InputTimeMessage timeMsg;
			if (gameUpdater.state->time.allowedSteps > 0)
				timeMsg.allowSteps = gameUpdater.state->time.allowedSteps;
			else
				timeMsg.allowSteps = gameUpdater.state->time.performedSteps;
			timeMsg.allowSteps += 1;	
			timeMsg.modifyAllowSteps = true;
			network->send(&timeMsg);
			break;
		};
		case KEYBOARD_ACTIONS::TIME_STEP5 :
		{
			InputTimeMessage timeMsg;
			if (gameUpdater.state->time.allowedSteps > 0)
				timeMsg.allowSteps = gameUpdater.state->time.allowedSteps;
			else
				timeMsg.allowSteps = gameUpdater.state->time.performedSteps;
			timeMsg.allowSteps += 5;	
			timeMsg.modifyAllowSteps = true;
			network->send(&timeMsg);
			break;
		};
		case KEYBOARD_ACTIONS::TIME_STEP25 :
		{
			InputTimeMessage timeMsg;
			if (gameUpdater.state->time.allowedSteps > 0)
				timeMsg.allowSteps = gameUpdater.state->time.allowedSteps;
			else
				timeMsg.allowSteps = gameUpdater.state->time.performedSteps;
			timeMsg.allowSteps += 25;	
			timeMsg.modifyAllowSteps = true;
			network->send(&timeMsg);
			break;
		};
		
		case KEYBOARD_ACTIONS::TIME_SCALE_T_03 :
		{
			InputTimeMessage timeMsg;
			timeMsg.forcedTimeScale = 0.3;
			timeMsg.modifyForcedTimeScale = true;
			network->send(&timeMsg);
			break;
		};
		case KEYBOARD_ACTIONS::TIME_SCALE_T_1 :
		{
			InputTimeMessage timeMsg;
			timeMsg.forcedTimeScale = 1;
			timeMsg.modifyForcedTimeScale = true;
			network->send(&timeMsg);
			break;
		};
		case KEYBOARD_ACTIONS::TIME_SCALE_T_3 :
		{
			InputTimeMessage timeMsg;
			timeMsg.forcedTimeScale = 3;
			timeMsg.modifyForcedTimeScale = true;
			network->send(&timeMsg);
			break;
		};
		
		case KEYBOARD_ACTIONS::TIME_SCALE03 :
		{
			InputTimeMessage timeMsg;
			timeMsg.timeScale = 0.3;
			timeMsg.modifyTimeScale = true;
			timeMsg.allowSteps = -1;
			timeMsg.modifyAllowSteps = true;
			network->send(&timeMsg);
			break;
		};
		case KEYBOARD_ACTIONS::TIME_SCALE1 :
		{
			InputTimeMessage timeMsg;
			timeMsg.timeScale = 1;
			timeMsg.modifyTimeScale = true;
			timeMsg.allowSteps = -1;
			timeMsg.modifyAllowSteps = true;
			network->send(&timeMsg);
			break;
		};
		case KEYBOARD_ACTIONS::TIME_SCALE3 :
		{
			InputTimeMessage timeMsg;
			timeMsg.timeScale = 3;
			timeMsg.modifyTimeScale = true;
			timeMsg.allowSteps = -1;
			timeMsg.modifyAllowSteps = true;
			network->send(&timeMsg);
			break;
		};
		case KEYBOARD_ACTIONS::RIGHT :
		case KEYBOARD_ACTIONS::LEFT :
		case KEYBOARD_ACTIONS::FORWARD :
		case KEYBOARD_ACTIONS::BACKWARD :
		{
			InputActionMessage im;

			im.downedButtons.push_back((int8_t)code);
			im.localId = idCounter++;
			network->send(&im);
			break;
		}
		default:
		{
			S::log.add("action not handled: " + std::to_string((int)code), {LOG_TAGS::ERROR_});
			break;
		}	
	}
}

void Game::handleKeyUp(KEYBOARD_ACTIONS code)
{
	switch (code)
	{		
		case KEYBOARD_ACTIONS::TIME_SCALE_T_03 :
		case KEYBOARD_ACTIONS::TIME_SCALE_T_1 :
		case KEYBOARD_ACTIONS::TIME_SCALE_T_3 :
		{
			if (!keyboard.isDown[keyboard.buttonByAction[(int)KEYBOARD_ACTIONS::TIME_SCALE_T_03]] &&
				!keyboard.isDown[keyboard.buttonByAction[(int)KEYBOARD_ACTIONS::TIME_SCALE_T_1]] &&
				!keyboard.isDown[keyboard.buttonByAction[(int)KEYBOARD_ACTIONS::TIME_SCALE_T_3]]
			)
			{
			InputTimeMessage timeMsg;
			timeMsg.forcedTimeScale = -1;
			timeMsg.modifyForcedTimeScale = true;
			network->send(&timeMsg);
			}
			break;
			
		};
		case KEYBOARD_ACTIONS::TIME_SCALE03 :
		case KEYBOARD_ACTIONS::TIME_SCALE1 :
		case KEYBOARD_ACTIONS::TIME_SCALE3 :
		case KEYBOARD_ACTIONS::TIME_STEP1 :
		case KEYBOARD_ACTIONS::TIME_STEP5 :
		case KEYBOARD_ACTIONS::TIME_STEP25 :
		break;
		
		case KEYBOARD_ACTIONS::RIGHT :
		case KEYBOARD_ACTIONS::LEFT :
		case KEYBOARD_ACTIONS::FORWARD :
		case KEYBOARD_ACTIONS::BACKWARD :
		{
			InputActionMessage im;

			im.uppedButtons.push_back((int8_t)code);
			im.localId = idCounter++;
			network->send(&im);
			break;
		}
		default:
		{
			S::log.add("action not handled: " + std::to_string((int)code), {LOG_TAGS::ERROR_});
			break;
		}		
		
	}
}

void Game::handleGameInput(std::unique_ptr<NetworkMessage> message)
{
	InputMessage* t = dynamic_cast<InputMessage*>(message.release());
	gameUpdater.addNewInput(std::unique_ptr<InputMessage>(t));
}

void Game::addNetworkBindings()
{
	//-------------------------------------------------------------------------------
	std::unique_ptr<AnonymousBinding> binding;
	
	binding = std::make_unique<AnonymousBinding>("TYPE_INPUT_ACTION_MSG");
	binding->
	bindByMsgType(MessageTypes::TYPE_INPUT_ACTION_MSG)->
	setCallOnce(false)->
	setHandler(std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this](std::unique_ptr<NetworkMessage> message){handleGameInput(std::move(message));}));
	network->binder.bind(std::move(binding));
	
	binding = std::make_unique<AnonymousBinding>("TYPE_INPUT_JOINED_MSG");
	binding->
	bindByMsgType(MessageTypes::TYPE_INPUT_JOINED_MSG)->
	setCallOnce(false)->
	setHandler(std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this](std::unique_ptr<NetworkMessage> message){handleGameInput(std::move(message));}));
	network->binder.bind(std::move(binding));

	binding = std::make_unique<AnonymousBinding>("TYPE_INPUT_ROUTE_MSG");
	binding->
	bindByMsgType(MessageTypes::TYPE_INPUT_ROUTE_MSG)->
	setCallOnce(false)->
	setHandler(std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this](std::unique_ptr<NetworkMessage> message){handleGameInput(std::move(message));}));
	network->binder.bind(std::move(binding));

	binding = std::make_unique<AnonymousBinding>("TYPE_INPUT_LEFT_MSG");
	binding->
	bindByMsgType(MessageTypes::TYPE_INPUT_LEFT_MSG)->
	setCallOnce(false)->
	setHandler(std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this](std::unique_ptr<NetworkMessage> message){handleGameInput(std::move(message));}));
	network->binder.bind(std::move(binding));

	binding = std::make_unique<AnonymousBinding>("TYPE_INPUT_TIME_MSG");
	binding->
	bindByMsgType(MessageTypes::TYPE_INPUT_TIME_MSG)->
	setCallOnce(false)->
	setHandler(std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this](std::unique_ptr<NetworkMessage> message){handleGameInput(std::move(message));}));
	network->binder.bind(std::move(binding));

	//-------------------------------------------------------------------------------

	

	//-------------------------------------------------------------------------------


	auto handleGameState = std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this](std::unique_ptr<NetworkMessage> message) {

		GameStateMessage* gameStateMsg = dynamic_cast<GameStateMessage*>(message.get());

		SerializationStream stream1(new StreamGrowerExp(128, 2)),
			stream2(new StreamGrowerExp(128, 2)),
			stream3(new StreamGrowerExp(128, 2));
		Serializer::write(*gameStateMsg->state, stream1);
		S::log.add("SERVER STATE (" + std::to_string(gameStateMsg->state->timeStamp) + ")\n\t" +
			Serializer::toHex(stream1.readAll(), stream1.getLength()), { LOG_TAGS::UNIQUE });

		auto myState = gameUpdater.getNewStateByStamp(gameStateMsg->state->timeStamp);
		Serializer::write(*gameStateMsg->state, stream2);

		if (stream1.getLength() != stream2.getLength() ||
			memcmp(stream1.readAll(), stream2.readAll(), stream1.getLength()) != 0)
		{
			S::log.add("BUT MY STATE (" + std::to_string(myState->timeStamp) + ")\n\t" +
				Serializer::toHex(stream2.readAll(), stream2.getLength()), { LOG_TAGS::UNIQUE });
		}
		/*
		Serializer::write(gameUpdater->state.get(), stream3);
		S::log.add("CURRENT STATE (" + std::to_string(gameUpdater->state->timeStamp) + ")\n\t" +
		Serializer::toHex(stream3.readAll(), stream3.getLength()), { LOG_TAGS::UNIQUE });*/
		std::string logStr = "PLAYER LOCATIONS: \n\t";
		for (size_t i = 0; i < gameUpdater.state->players.size(); i++)
		{
			logStr += "[";
			logStr += std::to_string(gameUpdater.state->players[i].x);
			logStr += ":";
			logStr += std::to_string(gameUpdater.state->players[i].y);
			logStr += "] ";
		}
		S::log.add(logStr + "\n\n", { LOG_TAGS::UNIQUE });
	});

	binding = std::make_unique<AnonymousBinding>("TYPE_GAME_STATE_MSG");
	binding->
	bindByMsgType(MessageTypes::TYPE_GAME_STATE_MSG)->
	setCallOnce(false)->
	setHandler(std::move(handleGameState));
	network->binder.bind(std::move(binding));

	//-------------------------------------------------------------------------------

}

