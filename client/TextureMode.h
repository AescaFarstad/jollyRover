#pragma once
#include <Mode.h>
#include <TextureView.h>


class TextureMode : public Mode
{
public:
	virtual ~TextureMode() = default;
	
	void init(Renderer* renderer, Prototypes* prototypes);

	void update(bool isActive) override;
	
	void onMouseDown(const SDL_MouseButtonEvent& event) override;
	void onMouseUp(const SDL_MouseButtonEvent& event) override;
	void onMouseMove(const SDL_MouseMotionEvent& event) override;
	
	void onKeyDown(SDL_Scancode scancode, const KeyboardInputContext& context) override;
	void onKeyUp(SDL_Scancode scancode, const KeyboardInputContext& context) override;

private:

	TextureView m_textureView;
	Prototypes* m_prototypes;
	std::vector<TextureDisplay> m_displays;
	
	void parse(TextureFolder& folder);
	void arrange();
	void sort();

};