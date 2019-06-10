#pragma once
#include <Mode.h>
#include <GameUpdater.h>
#include <GameView.h>
#include <RouteInput.h>
#include <Network.h>
#include <GameKeyboardInput.h>

class GameMode : public Mode
{
public:
	GameMode();
	virtual ~GameMode() = default;
	
	void init(GPU_Target* screen, Prototypes* prototypes, Network* network);
	void loadGame(std::unique_ptr<GameState> state, int64_t clientToServerDelta);
	
	void addNewInput(std::unique_ptr<InputMessage> input);

	virtual void update(bool isActive) override;
	
	virtual void onMouseDown(SDL_MouseButtonEvent* event) override;
	virtual void onMouseUp(SDL_MouseButtonEvent* event) override;
	virtual void onMouseMove(SDL_MouseMotionEvent* event) override;
	
	virtual	void handleKeyDown(SDL_Scancode scancode, Keyboard& keyboard) override;
	virtual	void handleKeyUp(SDL_Scancode scancode, Keyboard& keyboard) override;
	
	GameUpdater* getGameUpdater();

private:

	bool m_isLoaded;
	int64_t m_clientToServerDelta;
	
	Network* m_network;
	Prototypes* m_prototypes;
	
	GameUpdater m_gameUpdater;
	GameView m_gameView;
	RouteInput m_routeInput;
	GameKeyboardInput m_keyboardInput;

};