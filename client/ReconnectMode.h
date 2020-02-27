#pragma once
#include <Mode.h>
#include <Prototypes.h>
#include <Renderer.h>
#include <Network.h>


class ReconnectMode : public Mode
{
public:
	virtual ~ReconnectMode() = default;
	
	void init(Renderer* renderer, Prototypes* prototypes);

	void update(bool isActive) override;
	
	void onMouseDown(const SDL_MouseButtonEvent& event) override;
	void onMouseUp(const SDL_MouseButtonEvent& event) override;
	void onMouseMove(const SDL_MouseMotionEvent& event) override;
	
	void onKeyDown(SDL_Scancode scancode, const KeyboardInputContext& context) override;
	void onKeyUp(SDL_Scancode scancode, const KeyboardInputContext& context) override;

private:
	Prototypes* m_prototypes;
	Renderer* m_renderer;
	int32_t m_ticksPassed = 0;
	
};