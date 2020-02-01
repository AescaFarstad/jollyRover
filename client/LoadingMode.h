#pragma once
#include <Mode.h>
#include <Prototypes.h>
#include <Renderer.h>


class LoadingMode : public Mode
{
public:
	virtual ~LoadingMode() = default;
	
	void init(Renderer* renderer, Prototypes* prototypes, std::function<void()> onGoOffline);

	void update(bool isActive) override;
	
	void onMouseDown(const SDL_MouseButtonEvent& event) override;
	void onMouseUp(const SDL_MouseButtonEvent& event) override;
	void onMouseMove(const SDL_MouseMotionEvent& event) override;
	
	void onKeyDown(SDL_Scancode scancode, const KeyboardInputContext& context) override;
	void onKeyUp(SDL_Scancode scancode, const KeyboardInputContext& context) override;

private:
	Prototypes* m_prototypes;
	Renderer* m_renderer;
	std::function<void()> m_onGoOffline;
	int32_t m_ticksPassed = 0;
	const int32_t SHOW_OFFLINE_PROMPT_AT = 300;
	
};