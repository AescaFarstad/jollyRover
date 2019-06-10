#pragma once
#include <Mode.h>
#include <TextureView.h>


class TextureMode : public Mode
{
public:
	TextureMode() = default;
	virtual ~TextureMode() = default;
	
	void init(Renderer* renderer, Prototypes* prototypes);

	virtual void update(bool isActive) override;
	
	virtual void onMouseDown(SDL_MouseButtonEvent* event) override;
	virtual void onMouseUp(SDL_MouseButtonEvent* event) override;
	virtual void onMouseMove(SDL_MouseMotionEvent* event) override;
	
	virtual	void onKeyDown(SDL_Scancode scancode, Keyboard& keyboard) override;
	virtual	void onKeyUp(SDL_Scancode scancode, Keyboard& keyboard) override;

private:

	TextureView m_textureView;
	Prototypes* m_prototypes;
	std::vector<TextureDisplay> m_displays;
	
	void parse(TextureFolder& folder);
	void arrange();
	void sort();

};