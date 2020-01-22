#pragma once
#include <GameState.h>
#include <Prototypes.h>
#include <RouteInput.h>
#include <memory>
#include <SDL_gpu.h>
#include <Renderer.h>
#include <SerialAnimationView.h>
#include <CreepView.h>
#include <ViewController.h>
#include <FlyingMessage.h>
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
	void addMessage(std::string message, Point location, NFont::AlignEnum aligment = NFont::AlignEnum::LEFT);
	
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
	ViewController<CreepView> m_creeps;
	int32_t m_lastTime;	
	CircularContainer<FlyingMessage, 5> m_flyingMessages;
	
	GPU_Image* m_layer1Image;
	GPU_Image* m_layer2Image;
	GPU_Image* m_layer3Image;
	NFont m_fontAmaticBold;
	NFont m_fontAmaticBoldBig;
	NFont m_fontAmaticRegular;
	NFont m_fontDebug;
	NFont m_fontDebug_m;
	
	uint32_t m_loadCount;

	void drawPlayers();
	void drawObstacles();
	void drawInput();
	void drawCars();
	void drawCreeps();
	void drawProjectiles();
	void drawFormations();
	void drawFormationAgro();
	void drawFormationConnections();
	void drawProjectileExplosion();
	void drawRemnants();
	void drawDebugGraphics();
	void drawThreatMap();
	void drawHUD();
	void drawFlyingMessages();
	
	void renderPathStep(Point from, Point to, SequenceDef& dash, SequenceDef& dot, uint32_t rndSeed, float ratio);
	
	GPU_Image* loadImage(std::string path);
	void resolveTextures(Prototypes* prototypes);
	
	
};

