#pragma once
#include <Mode.h>
#include <GameUpdater.h>
#include <GameView.h>
#include <RouteInput.h>

class DemoMode : public Mode
{
public:
	DemoMode();
	virtual ~DemoMode() = default;
	
	void init(Renderer* renderer, Prototypes* prototypes);
	void loadGame(const std::vector<char>& demoData);
	

	virtual void update(bool isActive) override;
	
	virtual void onMouseDown(const SDL_MouseButtonEvent& event) override;
	virtual void onMouseUp(const SDL_MouseButtonEvent& event) override;
	virtual void onMouseMove(const SDL_MouseMotionEvent& event) override;
	
	virtual	void onKeyDown(SDL_Scancode scancode, const KeyboardInputContext& context) override;
	virtual	void onKeyUp(SDL_Scancode scancode, const KeyboardInputContext& context) override;
	
	
	void visualizeInputs(std::vector<InputMessage*> inputs);
	

private:

	bool m_isLoaded;
	uint32_t m_startTime;
	uint32_t m_startStateStamp;
	
	Prototypes* m_prototypes;
	
	GameUpdater m_gameUpdater;
	GameView m_gameView;
	RouteInput m_routeInput;

};