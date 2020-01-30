#include <Game.h>
#include <InputTimeMessage.h>
#include <LoopBackNetwork.h>
#include <NetworkMessageFactory.h>
#include <SerializationStream.h>
#include <GreetingMessage.h>
#include <GenericRequestMessage.h>
#include <ConsecutiveTask.h>
#include <string>
#include <istream>
#include <sstream>
#include <GameInputBinding.h>
#include <AnonymousBinding.h>
#include <json.hpp>
#include <fstream>
#include <memory>
#include <GameState.h>
#include <PersistentStorage.h>
#include <InputImpulseMessage.h>
#include <GameStateMessage.h>
#include <GreetingMessage.h>
#include <DEBUG.h>

Game::Game()
{
	m_network = nullptr;
	m_atlas = nullptr;
}

Game::~Game()
{
	if (m_network != nullptr)
		delete m_network;
	if (m_atlas != nullptr)
		GPU_FreeImage(m_atlas);
}

void Game::init(GPU_Target* screen)
{
	loadConfig();
	loadPrototypes();
	//S::persistentStorage.clean();
	S::persistentStorage.init();
	
	if (S::config.loopBack)
		m_network = new LoopBackNetwork(m_gameMode.getGameUpdater());
	else
		m_network = new Network();		
	m_network->connect();
	
	m_window = SDL_GetWindowFromID(screen->context->windowID);
	SDL_SetWindowSize(m_window, m_prototypes.variables.fieldWidth, m_prototypes.variables.fieldHeight);
	GPU_SetWindowResolution(m_prototypes.variables.fieldWidth, m_prototypes.variables.fieldHeight);
	SDL_SetWindowPosition(m_window, S::config.window_X, S::config.window_Y);
	
	m_atlas = GPU_LoadImage("out/assets/atlas.png");
 	if (!m_atlas)
 		THROW_FATAL_ERROR("IMG IS NULL")
 	GPU_SetSnapMode(m_atlas, GPU_SNAP_NONE);
	GPU_SetBlending(m_atlas, 1);
	 
	m_renderer.init(screen, m_atlas);
	
	m_modes.push_back(&m_textureMode);
	m_textureMode.init(&m_renderer, &m_prototypes);
	
	m_gameMode.init(&m_renderer, &m_prototypes, m_network);
	m_modes.push_back(&m_gameMode);
	m_activeMode = 1;	
}

void Game::start()
{
	if (S::config.runBenchmark)
		runBenchmark();
	else
		startGame();
}

void Game::runBenchmark()
{
	GameUpdater gu;
	auto state = std::make_unique<GameState>(892422);
	S::log.add("State checksum: " + S::crc(*state));
	gu.load(std::move(state), &m_prototypes, true);
	
	auto joined = std::make_unique<InputPlayerJoinedMessage>();
	joined->login = 102;
	joined->serverStamp = 500;
	
	auto addAI1 = std::make_unique<InputImpulseMessage>();
	addAI1->impulse = INPUT_IMPULSE::ADD_AI;
	addAI1->login = 102;
	addAI1->serverStamp = 1000;
	
	auto addAI2 = std::make_unique<InputImpulseMessage>();
	addAI2->impulse = INPUT_IMPULSE::ADD_AI;
	addAI2->login = 102;
	addAI2->serverStamp = 1100;
	
	gu.addNewInput(std::move(joined));
	gu.addNewInput(std::move(addAI1));
	gu.addNewInput(std::move(addAI2));
	
	int32_t startTime = SDL_GetTicks();
	int32_t iters = 20;
	for(int32_t i = 0; i < iters; i++)
	{
		gu.update(i * 10000 + 100);
		S::log.add("State checksum at: " + std::to_string(i) + " = " + S::crc(*gu.state));
		S::log.add(std::to_string(i * 100 / iters) + "%");
	}
	S::log.add("Finished in " + std::to_string(SDL_GetTicks() - startTime));
	S::log.add("Performance buffer: " + std::to_string((float)iters * 10000 / (SDL_GetTicks() - startTime)));
	
	S::log.add("State checksum: " + S::crc(*gu.state));
	
	startGame();
}

void Game::startGame()
{
	auto loadGameTask = std::make_unique<ConsecutiveTask>();	
	
	loadGameTask->pushAsync([this](std::unique_ptr<Callback> callback) {
		auto binding = std::make_unique<AnonymousBinding>("wait for greetings prompt from the server and send it");

		auto handleRequest = std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this, cb = callback.release()](std::unique_ptr<NetworkMessage> message) {

			m_network->send(GreetingMessage());

			cb->execute();
			delete cb;
		});
		
		binding->
			bindByGenericType(REQUEST_TYPE::REQUEST_GREETING)->
			setCallOnce(true)->
			setHandler(std::move(handleRequest));
		
		m_network->genericRequestBinder.bind(std::move(binding));
	}, "wait for greetings prompt from the server and send it");

	loadGameTask->pushAsync([this](std::unique_ptr<Callback> callback) {
		m_network->interceptOnce(MESSAGE_TYPE::TYPE_GREETING_MSG, [this, cb = callback.release()](std::unique_ptr<NetworkMessage> message) {

			GreetingMessage* gMsg = dynamic_cast<GreetingMessage*>(message.release());
			m_login = gMsg->login;
			m_password = gMsg->password;
			delete gMsg;

			cb->execute();
			delete cb;
		});
	}, "wait for greetings response from the server");

	auto ping = [this](std::unique_ptr<Callback> callback) {
		m_network->interceptGenericRequestOnce(REQUEST_TYPE::REQUEST_PONG, [cb = callback.release()](std::unique_ptr<GenericRequestMessage> message) {
			
			cb->execute();
			delete cb;
		});

		GenericRequestMessage grMsg;
		grMsg.request = REQUEST_TYPE::REQUEST_PING;
		m_network->send(grMsg);
	};

	loadGameTask->pushAsync(ping, "ping1");
	loadGameTask->pushAsync(ping, "ping2");
	loadGameTask->pushAsync(ping, "ping3");

	loadGameTask->pushSync([this]() {
		GenericRequestMessage grMsg;
		grMsg.request = REQUEST_TYPE::REQUEST_JOIN_GAME;
		m_network->send(grMsg);
	}, "request to JOIN_GAME");	

	loadGameTask->pushAsync([this](std::unique_ptr<Callback> callback) {
		m_network->interceptOnce(MESSAGE_TYPE::TYPE_GAME_STATE_MSG, [this, cb = callback.release()](std::unique_ptr<NetworkMessage> message) {

			GameStateMessage* gameStateMsg = dynamic_cast<GameStateMessage*>(message.release());
			int64_t clientToServerDelta = m_network->timeSync.localToServerUpperBound(0);
			S::log.add("server time delta: " + std::to_string(clientToServerDelta) + 
					" uncertainty: " + std::to_string(m_network->timeSync.getUncertainty()), { LOG_TAGS::NET });
			m_gameMode.loadGame(std::move(gameStateMsg->state), clientToServerDelta, m_login);
			
			addNetworkBindings();

			cb->execute();
			delete cb;
		});
	}, "wait for game state from the server and load the game");
	
	loadGameTask->exec();

	m_taskManager.push(std::move(loadGameTask));
}

void Game::update()
{
	m_taskManager.update();
	m_network->update();
	
	for(auto& mode : m_modes)
		mode->update(mode == m_modes[m_activeMode]);
}

void Game::handleEvent(const SDL_Event& event)
{
	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		m_modes[m_activeMode]->onMouseDown(event.button);
		return;
	}

	if (event.type == SDL_MOUSEBUTTONUP)
	{
		m_modes[m_activeMode]->onMouseUp(event.button);
		return;
	}

	if (event.type == SDL_MOUSEMOTION)
	{
		m_keyboardContext.mouseCoords.x = event.motion.x;
		m_keyboardContext.mouseCoords.y = event.motion.y;
		m_modes[m_activeMode]->onMouseMove(event.motion);
		return;
	}
	
	if (
		(event.type != SDL_KEYDOWN && 
		 event.type != SDL_KEYUP) || false
		)
		return;
	switch (event.type) 
	{
		case SDL_KEYDOWN:
			if (!m_keyboardContext.keyboard.isDown[event.key.keysym.scancode])
			{
				m_keyboardContext.keyboard.isDown[event.key.keysym.scancode] = true;
				m_modes[m_activeMode]->onKeyDown(event.key.keysym.scancode, m_keyboardContext);
			}
			break;

		case SDL_KEYUP:
			if (m_keyboardContext.keyboard.isDown[event.key.keysym.scancode])
			{
				m_keyboardContext.keyboard.isDown[event.key.keysym.scancode] = false;
				if (!handleGlobalKey(event.key.keysym.scancode))				
					m_modes[m_activeMode]->onKeyUp(event.key.keysym.scancode, m_keyboardContext);
			}
			break;

		default:
			break;
	}
		
}

bool Game::handleGlobalKey(SDL_Scancode scancode)
{
	switch (scancode) 
	{
		case SDL_SCANCODE_F1:
		{
			m_activeMode = (m_activeMode + 1) % m_modes.size();
			return true;			
		}
		default: return false;
	}
	
}

void Game::loadPrototypes()
{
	std::ifstream file("out/prototypes.json");
	nlohmann::json j = nlohmann::json::parse(file);
	file.close();
	m_prototypes.load(j);
}

void Game::loadConfig()
{
	std::ifstream file("out/config.json");
	nlohmann::json j = nlohmann::json::parse(file);
	file.close();
	S::config.load(j);	
}


void Game::handleGameInput(std::unique_ptr<NetworkMessage> message)
{
	InputMessage* t = dynamic_cast<InputMessage*>(message.release());
	m_gameMode.addNewInput(std::unique_ptr<InputMessage>(t));
}

void Game::addNetworkBindings()
{
	//-------------------------------------------------------------------------------
	std::unique_ptr<AnonymousBinding> binding;
	
	binding = std::make_unique<AnonymousBinding>("TYPE_INPUT_ACTION_MSG");
	binding->
	bindByMsgType(MESSAGE_TYPE::TYPE_INPUT_ACTION_MSG)->
	setCallOnce(false)->
	setHandler(std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this](std::unique_ptr<NetworkMessage> message){handleGameInput(std::move(message));}));
	m_network->binder.bind(std::move(binding));
	
	binding = std::make_unique<AnonymousBinding>("TYPE_INPUT_JOINED_MSG");
	binding->
	bindByMsgType(MESSAGE_TYPE::TYPE_INPUT_JOINED_MSG)->
	setCallOnce(false)->
	setHandler(std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this](std::unique_ptr<NetworkMessage> message){handleGameInput(std::move(message));}));
	m_network->binder.bind(std::move(binding));

	binding = std::make_unique<AnonymousBinding>("TYPE_INPUT_ROUTE_MSG");
	binding->
	bindByMsgType(MESSAGE_TYPE::TYPE_INPUT_ROUTE_MSG)->
	setCallOnce(false)->
	setHandler(std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this](std::unique_ptr<NetworkMessage> message){handleGameInput(std::move(message));}));
	m_network->binder.bind(std::move(binding));

	binding = std::make_unique<AnonymousBinding>("TYPE_INPUT_LEFT_MSG");
	binding->
	bindByMsgType(MESSAGE_TYPE::TYPE_INPUT_LEFT_MSG)->
	setCallOnce(false)->
	setHandler(std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this](std::unique_ptr<NetworkMessage> message){handleGameInput(std::move(message));}));
	m_network->binder.bind(std::move(binding));

	binding = std::make_unique<AnonymousBinding>("TYPE_INPUT_TIME_MSG");
	binding->
	bindByMsgType(MESSAGE_TYPE::TYPE_INPUT_TIME_MSG)->
	setCallOnce(false)->
	setHandler(std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this](std::unique_ptr<NetworkMessage> message){handleGameInput(std::move(message));}));
	m_network->binder.bind(std::move(binding));
	
	binding = std::make_unique<AnonymousBinding>("TYPE_INPUT_IMPULSE_MSG");
	binding->
	bindByMsgType(MESSAGE_TYPE::TYPE_INPUT_IMPULSE_MSG)->
	setCallOnce(false)->
	setHandler(std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this](std::unique_ptr<NetworkMessage> message){handleGameInput(std::move(message));}));
	m_network->binder.bind(std::move(binding));
	
	binding = std::make_unique<AnonymousBinding>("TYPE_INPUT_DEBUG_MSG");
	binding->
	bindByMsgType(MESSAGE_TYPE::TYPE_INPUT_DEBUG_MSG)->
	setCallOnce(false)->
	setHandler(std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this](std::unique_ptr<NetworkMessage> message){handleGameInput(std::move(message));}));
	m_network->binder.bind(std::move(binding));

	binding = std::make_unique<AnonymousBinding>("TYPE_LOAD_GAME_MSG");
	binding->
	bindByMsgType(MESSAGE_TYPE::TYPE_LOAD_GAME_MSG)->
	setCallOnce(false)->
	setHandler(std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this](std::unique_ptr<NetworkMessage> message){handleGameInput(std::move(message));}));
	m_network->binder.bind(std::move(binding));

	//-------------------------------------------------------------------------------

	

	//-------------------------------------------------------------------------------


	auto handleGameState = std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this](std::unique_ptr<NetworkMessage> message) {

		GameStateMessage* gameStateMsg = dynamic_cast<GameStateMessage*>(message.get());

		SerializationStream stream1 = SerializationStream::createExp();
		SerializationStream stream2 = SerializationStream::createExp();
		SerializationStream stream3 = SerializationStream::createExp();
		Serializer::write(*gameStateMsg->state, stream1);
		S::log.add("SERVER STATE (" + std::to_string(gameStateMsg->state->timeStamp) + ")\n\t" +
			Serializer::toHex(stream1.readAll(), stream1.getLength()), { LOG_TAGS::UNIQUE });
		
		GameUpdater* gameUpdater = m_gameMode.getGameUpdater();
		
		auto myState = gameUpdater->getNewStateByStamp(gameStateMsg->state->timeStamp);
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
	});

	binding = std::make_unique<AnonymousBinding>("TYPE_GAME_STATE_MSG");
	binding->
	bindByMsgType(MESSAGE_TYPE::TYPE_GAME_STATE_MSG)->
	setCallOnce(false)->
	setHandler(std::move(handleGameState));
	m_network->binder.bind(std::move(binding));

	//-------------------------------------------------------------------------------

}

