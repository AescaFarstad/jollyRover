#include <Game.h>

Game::Game(SDL_Window* window, SDL_Renderer* renderer)
{
	loadPrototypes();

	gameView = new GameView(window, renderer, &prototypes);

	taskManager = new TaskManager();
	network = new Network();
	network->connect();


	routeInput = new RouteInput();

	//addNetworkBindings();


	std::unique_ptr<ConsequtiveTask> loadGameTask = std::make_unique<ConsequtiveTask>();

	loadGameTask->pushAsync([this](std::unique_ptr<Callback> callback) {
		std::unique_ptr<AnonymousBinding> binding = std::make_unique<AnonymousBinding>("wait for greetings prompt from the server and send it");

		auto handleRequest = std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this, cb = callback.release()](std::unique_ptr<NetworkMessage> message) {

			GreetingMessage gMsg;
			network->send(&gMsg);

			cb->execute();
			delete cb;
		});
		
		binding->
		bindByType(RequestTypes::REQUEST_GREETING)->
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
	//loadGameTask->pushAsync(ping, "ping2");
	//loadGameTask->pushAsync(ping, "ping3");

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
		routeInput->load(gameUpdater.state.get(), &prototypes, [this](std::unique_ptr<InputMessage> msg) {
			network->send(msg.get());
		});
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

	if (event->type == SDL_KEYDOWN && event->key.keysym.scancode == 53)
	{
		printf("\n\n\n\n\n\n\n\n\n\n\n\n");
	}

	if (
		(event->type != SDL_KEYDOWN && 
		 event->type != SDL_KEYUP) ||
		keyboard.actionByButton[event->key.keysym.scancode] == KEYBOARD_ACTIONS::NONE
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



void Game::loadPrototypes()
{
	std::ifstream file("prototypes.json");	
	json j = json::parse(file);
	file.close();
	prototypes.load(j);
}
int16_t idCounter = 0;
void Game::handleKeyDown(KEYBOARD_ACTIONS code)
{
	InputActionMessage im;

	im.downedButtons.push_back((int8_t)code);
	im.localId = idCounter++;
	network->send(&im);
}

void Game::handleKeyUp(KEYBOARD_ACTIONS code)
{
	InputActionMessage im;

	im.uppedButtons.push_back((int8_t)code);
	im.localId = idCounter++;
	network->send(&im);
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
	bindByType(MessageTypes::TYPE_INPUT_ACTION_MSG)->
	setCallOnce(false)->
	setHandler(std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this](std::unique_ptr<NetworkMessage> message){handleGameInput(std::move(message));}));
	network->binder.bind(std::move(binding));
	
	binding = std::make_unique<AnonymousBinding>("TYPE_INPUT_JOINED_MSG");
	binding->
	bindByType(MessageTypes::TYPE_INPUT_JOINED_MSG)->
	setCallOnce(false)->
	setHandler(std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this](std::unique_ptr<NetworkMessage> message){handleGameInput(std::move(message));}));
	network->binder.bind(std::move(binding));

	binding = std::make_unique<AnonymousBinding>("TYPE_INPUT_ROUTE_MSG");
	binding->
	bindByType(MessageTypes::TYPE_INPUT_ROUTE_MSG)->
	setCallOnce(false)->
	setHandler(std::make_unique<std::function<void(std::unique_ptr<NetworkMessage>)>>([this](std::unique_ptr<NetworkMessage> message){handleGameInput(std::move(message));}));
	network->binder.bind(std::move(binding));

	binding = std::make_unique<AnonymousBinding>("TYPE_INPUT_LEFT_MSG");
	binding->
	bindByType(MessageTypes::TYPE_INPUT_LEFT_MSG)->
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
	bindByType(MessageTypes::TYPE_GAME_STATE_MSG)->
	setCallOnce(false)->
	setHandler(std::move(handleGameState));
	network->binder.bind(std::move(binding));

	//-------------------------------------------------------------------------------

}

