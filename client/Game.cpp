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
#include <KeyboardInput.h>

Game::Game(GPU_Target* screen)
{
	loadConfig();
	loadPrototypes();

	m_gameView = new GameView(screen, &m_prototypes);

	m_taskManager = new TaskManager();
	if (S::config.loopBack)
		m_network = new LoopBackNetwork(&m_gameUpdater);
	else
		m_network = new Network();
		
	m_network->connect();


	m_routeInput = new RouteInput();


	auto loadGameTask = std::make_unique<ConsequtiveTask>();
	//std::unique_ptr<ConsequtiveTask> loadGameTask(new ConsequtiveTask);

	loadGameTask->pushAsync([this](std::unique_ptr<Callback> callback) {
		auto binding = std::make_unique<AnonymousBinding>("wait for greetings prompt from the server and send it");

		auto handleRequest = std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this, cb = callback.release()](std::unique_ptr<NetworkMessage> message) {

			GreetingMessage gMsg;
			m_network->send(&gMsg);

			cb->execute();
			delete cb;
		});
		
		binding->
			bindByGenericType(RequestTypes::REQUEST_GREETING)->
			setCallOnce(true)->
			setHandler(std::move(handleRequest));
		
		m_network->genericRequestBinder.bind(std::move(binding));
		//binding->handle(std::make_unique<NetworkMessage>());
	}, "wait for greetings prompt from the server and send it");

	loadGameTask->pushAsync([this](std::unique_ptr<Callback> callback) {
		m_network->interceptOnce(MessageTypes::TYPE_GREETING_MSG, [this, cb = callback.release()](std::unique_ptr<NetworkMessage> message) {

			GreetingMessage* gMsg = dynamic_cast<GreetingMessage*>(message.release());
			m_login = gMsg->login;
			m_password = gMsg->password;
			delete gMsg;

			cb->execute();
			delete cb;
		});
		//network->binder.traceBindings("added #2");
	}, "wait for greetings response from the server");

	auto ping = [this](std::unique_ptr<Callback> callback) {
		m_network->interceptGenericRequestOnce(RequestTypes::REQUEST_PONG, [cb = callback.release()](std::unique_ptr<GenericRequestMessage> message) {
			
			cb->execute();
			delete cb;
		});

		GenericRequestMessage grMsg;
		grMsg.request = RequestTypes::REQUEST_PING;
		m_network->send(&grMsg);
	};

	loadGameTask->pushAsync(ping, "ping1");
	loadGameTask->pushAsync(ping, "ping2");
	loadGameTask->pushAsync(ping, "ping3");

	loadGameTask->pushSync([this]() {
		GenericRequestMessage grMsg;
		grMsg.request = RequestTypes::REQUEST_JOIN_GAME;
		m_network->send(&grMsg);
	}, "request to JOIN_GAME");	

	loadGameTask->pushAsync([this](std::unique_ptr<Callback> callback) {
		m_network->interceptOnce(MessageTypes::TYPE_GAME_STATE_MSG, [this, cb = callback.release()](std::unique_ptr<NetworkMessage> message) {

			GameStateMessage* gameStateMsg = dynamic_cast<GameStateMessage*>(message.release());
			m_clientToServerDelta = m_network->timeSync.localToServerUpperBound(0);
			S::log.add("server time delta: " + std::to_string(m_clientToServerDelta) + 
					" uncertainty: " + std::to_string(m_network->timeSync.getUncertainty()), { LOG_TAGS::NET });
			m_gameUpdater.load(std::unique_ptr<GameState>(gameStateMsg->state), &m_prototypes, true);
			gameStateMsg->ownsState = false;
			
			addNetworkBindings();

			cb->execute();
			delete cb;
		});
	}, "wait for game state from the server and load the game");
	/*
	auto testPerformance = [this](std::unique_ptr<Callback> callback) {
			GameUpdater gu;
			auto state = std::make_unique<GameState>(892422);
			gu.load(std::move(state), &prototypes);
			
			int32_t startTime = SDL_GetTicks();
			for(int i = 0; i < 100; i++)
			{
				gu.update(i * 1000 + 100);
				S::log.add(std::to_string(i) + "%");
			}
			S::log.add("Finished in " + std::to_string(SDL_GetTicks() - startTime));
			S::log.add("Performance buffer: " + std::to_string((float)100 * 1000 / (SDL_GetTicks() - startTime)));
			
			callback->execute();
		};
	loadGameTask->pushAsync(testPerformance, "testPerformance");*/
	
	loadGameTask->exec();


	m_taskManager->push(std::move(loadGameTask));
}

Game::~Game()
{
}

void Game::update()
{
	m_taskManager->update();
	m_network->update();

	if (m_gameUpdater.isLoaded)
	{
		m_gameUpdater.update(SDL_GetTicks() + m_clientToServerDelta);
		if (!m_routeInput->isLoaded())
		{	
			m_routeInput->load(m_gameUpdater.state.get(), &m_prototypes);
		}
		if (m_routeInput->isCompletelyValid())
		{
			auto route = m_routeInput->claimRoute();
			InputRouteMessage nim(route);
			m_network->send(&nim);
		}
		m_gameView->render(m_gameUpdater.state.get(), m_routeInput);
	}
}

void Game::handleEvent(SDL_Event* event)
{
	if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		m_routeInput->onMouseDown(&event->button);
		return;
	}

	if (event->type == SDL_MOUSEBUTTONUP)
	{
		m_routeInput->onMouseUp(&event->button);
		return;
	}

	if (event->type == SDL_MOUSEMOTION)
	{
		m_routeInput->onMouseMove(&event->motion);
		m_gameView->onMouseMove(&event->motion);
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
			if (!m_keyboard.isDown[event->key.keysym.scancode])
			{
				//printf("Key press detected\n");
				m_keyboard.isDown[event->key.keysym.scancode] = true;
				KeyboardInput::handleKeyDown(m_keyboard.actionByButton[event->key.keysym.scancode], m_keyboard, *m_network, m_gameUpdater);
			}
			break;

		case SDL_KEYUP:
			if (m_keyboard.isDown[event->key.keysym.scancode])
			{
				//printf("Key release detected\n");
				m_keyboard.isDown[event->key.keysym.scancode] = false;
				KeyboardInput::handleKeyUp(m_keyboard.actionByButton[event->key.keysym.scancode], m_keyboard, *m_network, m_gameUpdater);
			}
			break;

		default:
			break;
	}
		
}

void Game::loadPrototypes()
{
	std::ifstream file("out/prototypes.json");
	json j = json::parse(file);
	file.close();
	m_prototypes.load(j);
	
	//std::ifstream file2("out/assets/sheet_tanks.png");
}

void Game::loadConfig()
{
	std::ifstream file("out/config.json");
	json j = json::parse(file);
	file.close();
	S::config.load(j);	
}



void Game::handleGameInput(std::unique_ptr<NetworkMessage> message)
{
	InputMessage* t = dynamic_cast<InputMessage*>(message.release());
	m_gameUpdater.addNewInput(std::unique_ptr<InputMessage>(t));
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
	m_network->binder.bind(std::move(binding));
	
	binding = std::make_unique<AnonymousBinding>("TYPE_INPUT_JOINED_MSG");
	binding->
	bindByMsgType(MessageTypes::TYPE_INPUT_JOINED_MSG)->
	setCallOnce(false)->
	setHandler(std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this](std::unique_ptr<NetworkMessage> message){handleGameInput(std::move(message));}));
	m_network->binder.bind(std::move(binding));

	binding = std::make_unique<AnonymousBinding>("TYPE_INPUT_ROUTE_MSG");
	binding->
	bindByMsgType(MessageTypes::TYPE_INPUT_ROUTE_MSG)->
	setCallOnce(false)->
	setHandler(std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this](std::unique_ptr<NetworkMessage> message){handleGameInput(std::move(message));}));
	m_network->binder.bind(std::move(binding));

	binding = std::make_unique<AnonymousBinding>("TYPE_INPUT_LEFT_MSG");
	binding->
	bindByMsgType(MessageTypes::TYPE_INPUT_LEFT_MSG)->
	setCallOnce(false)->
	setHandler(std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this](std::unique_ptr<NetworkMessage> message){handleGameInput(std::move(message));}));
	m_network->binder.bind(std::move(binding));

	binding = std::make_unique<AnonymousBinding>("TYPE_INPUT_TIME_MSG");
	binding->
	bindByMsgType(MessageTypes::TYPE_INPUT_TIME_MSG)->
	setCallOnce(false)->
	setHandler(std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this](std::unique_ptr<NetworkMessage> message){handleGameInput(std::move(message));}));
	m_network->binder.bind(std::move(binding));

	binding = std::make_unique<AnonymousBinding>("TYPE_LOAD_GAME_MSG");
	binding->
	bindByMsgType(MessageTypes::TYPE_LOAD_GAME_MSG)->
	setCallOnce(false)->
	setHandler(std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this](std::unique_ptr<NetworkMessage> message){handleGameInput(std::move(message));}));
	m_network->binder.bind(std::move(binding));

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

		auto myState = m_gameUpdater.getNewStateByStamp(gameStateMsg->state->timeStamp);
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
		for (size_t i = 0; i < m_gameUpdater.state->players.size(); i++)
		{
			logStr += "[";
			logStr += std::to_string(m_gameUpdater.state->players[i].x);
			logStr += ":";
			logStr += std::to_string(m_gameUpdater.state->players[i].y);
			logStr += "] ";
		}
		S::log.add(logStr + "\n\n", { LOG_TAGS::UNIQUE });
	});

	binding = std::make_unique<AnonymousBinding>("TYPE_GAME_STATE_MSG");
	binding->
	bindByMsgType(MessageTypes::TYPE_GAME_STATE_MSG)->
	setCallOnce(false)->
	setHandler(std::move(handleGameState));
	m_network->binder.bind(std::move(binding));

	//-------------------------------------------------------------------------------

}

