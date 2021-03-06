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
	
	void init(Renderer* renderer, Prototypes* prototypes);
	void loadGame(const GameState& state, int64_t clientToServerDelta, int32_t login);
	
	void addNewInput(std::unique_ptr<InputMessage> input);
	GameUpdater* getGameUpdater();

	virtual void update(bool isActive) override;
	
	virtual void onMouseDown(const SDL_MouseButtonEvent& event) override;
	virtual void onMouseUp(const SDL_MouseButtonEvent& event) override;
	virtual void onMouseMove(const SDL_MouseMotionEvent& event) override;
	
	virtual	void onKeyDown(SDL_Scancode scancode, const KeyboardInputContext& context) override;
	virtual	void onKeyUp(SDL_Scancode scancode, const KeyboardInputContext& context) override;
	
	static Point normalizeMessageLocation(Point location, Prototypes* prototypes);	

private:

	bool m_isLoaded;
	int64_t m_clientToServerDelta;
	int32_t m_login;
	
	Prototypes* m_prototypes;
	
	GameUpdater m_gameUpdater;
	GameView m_gameView;
	RouteInput m_routeInput;
	GameKeyboardInput m_keyboardInput;
	
	uint32_t m_lastAutodrawNotificationStamp;
	uint32_t m_numRandomClicks;
	
	void handleRouteInput();

};