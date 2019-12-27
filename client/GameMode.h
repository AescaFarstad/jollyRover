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
	
	void init(Renderer* renderer, Prototypes* prototypes, Network* network);
	void loadGame(std::unique_ptr<GameState> state, int64_t clientToServerDelta, int32_t login);
	
	void addNewInput(std::unique_ptr<InputMessage> input);
	GameUpdater* getGameUpdater();

	virtual void update(bool isActive) override;
	
	virtual void onMouseDown(SDL_MouseButtonEvent* event) override;
	virtual void onMouseUp(SDL_MouseButtonEvent* event) override;
	virtual void onMouseMove(SDL_MouseMotionEvent* event) override;
	
	virtual	void onKeyDown(SDL_Scancode scancode, Keyboard& keyboard) override;
	virtual	void onKeyUp(SDL_Scancode scancode, Keyboard& keyboard) override;
	

private:

	bool m_isLoaded;
	int64_t m_clientToServerDelta;
	int32_t m_login;
	
	Network* m_network;
	Prototypes* m_prototypes;
	
	GameUpdater m_gameUpdater;
	GameView m_gameView;
	RouteInput m_routeInput;
	GameKeyboardInput m_keyboardInput;

};