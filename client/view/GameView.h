#pragma once
#include <GameState.h>
#include <Prototypes.h>
#include <RouteInput.h>
#include <memory>
#include <SDL_gpu.h>
#include <Renderer.h>
#include <SerialAnimationView.h>
#include <ViewController.h>
#include <NFont.h>

#ifdef __EMSCRIPTEN__
	#include <SDL_image.h>
	#include <SDL.h>
#else 
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
#endif

class GameView
{
public:
	GameView();
	~GameView();

	void init(Renderer* renderer, Prototypes* prototypes);
	void render(GameState* state, RouteInput* routeInput);
	
	void onMouseMove(SDL_MouseMotionEvent* event);
	
	void setLogin(int32_t login);
	
private:

	GPU_Target* m_screen; 
	Renderer* m_renderer;
	Prototypes* m_prototypes;
	GameState* m_state;
	RouteInput* m_routeInput;
	int32_t m_login;
	ViewController<SerialAnimationView> m_projectileExplosions;	
	ViewController<SerialAnimationView> m_creepDeaths;
	ViewController<SerialAnimationView> m_carDeaths;
	int32_t lastTime;	
	
	GPU_Image* m_layer1Image;
	GPU_Image* m_layer2Image;
	GPU_Image* m_layer3Image;
	NFont m_fontAmaticBold;
	NFont m_fontAmaticRegular;
	
	uint32_t m_loadCount;

	void drawPlayers();
	void drawObstacles();
	void drawInput();
	void drawCars();
	void drawCreeps();
	void drawProjectiles();
	void drawFormations();
	void drawFormationConnections();
	void drawProjectileExplosion();
	void drawUnitExplosion();
	void drawDebugGraphics();
	void drawThreatMap();
	void drawHUD();
	
	GPU_Image* loadImage(std::string path);
	void resolveTextures(Prototypes* prototypes);
	
	
};

