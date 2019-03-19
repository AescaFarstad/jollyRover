#include <GameView.h>


GameView::GameView(SDL_Window* window, SDL_Renderer* renderer, Prototypes* prototypes)
{
	this->window = window;
	this->renderer = renderer;
	this->prototypes = prototypes;

	isInitialized = false;
	
}


GameView::~GameView()
{
}
/*
void rendery(SDL_Renderer *renderer, SDL_Surface *surf, SDL_Texture *texture)
	{
		SDL_Rect srcR;
		srcR.x = 573;
		srcR.y = 275;
		srcR.w = 83;
		srcR.h = 78;
		
		SDL_Rect srcR2;
		srcR2.x = 0;
		srcR2.y = 384;
		srcR2.w = 100;
		srcR2.h = 97;
			//x="573" y="275" width="83" height="78" x="0" y="384" width="100" height="97"
		
		SDL_Rect dst;
		SDL_Rect dst2;
		for(size_t i = 0; i < 15000; i++)
		{
			dst.x = i % 1280;
			dst.y = i / 1280;
			dst.w = 20;
			dst.h = 20;
			SDL_RenderCopyEx(renderer, texture, &srcR, &dst, 0, NULL, SDL_FLIP_NONE);
			
			int h = i + 100000;
			
			dst2.x = h % 1280;
			dst2.y = h / 1280;
			dst2.w = 40;
			dst2.h = 40;
			SDL_RenderCopyEx(renderer, texture, &srcR2, &dst2, 0, NULL, SDL_FLIP_NONE);
		}
		
	}*/
void GameView::render(GameState* state, RouteInput* routeInput)
{
	if (!isInitialized)
		init();

	this->state = state;
	this->routeInput = routeInput;

	drawPlayers();
	drawObstacles();
	drawInput();
	drawCars();
	drawCreeps();
	drawProjectiles();
	//rendery(renderer, loadedSurface, ltexture);
}


void GameView::init()
{
	SDL_SetWindowSize(window, prototypes->variables.fieldWidth, prototypes->variables.fieldHeight);
	isInitialized = true;
	loadedSurface = IMG_Load("out/assets/sheet_tanks.png");
	if(!loadedSurface) {
    	printf("IMG_Load: %s\n", IMG_GetError());}
	ltexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
}

void GameView::setColor(uint32_t color)
{
	SDL_SetRenderDrawColor(renderer, (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, 0xFF);
}

void GameView::drawPlayers()
{
	uint32_t colors[10] = {
		0x000000,
		0xff0000,
		0x00ff00,
		0x0000ff,
		0xffff00,
		0x00ffff,
		0xff00ff,
		0x777777,
		0x0077ff,
		0xff7700
	};

	for (size_t i = 0; i < state->players.size(); i++)
	{
		setColor(colors[i]);

		SDL_Rect rect;
		rect.x = state->players[i].x + 200;
		rect.y = state->players[i].y + 200;
		rect.w = 5;
		rect.h = 5;
		SDL_RenderFillRect(renderer, &rect);
	}
}

void GameView::drawObstacles()
{
	uint32_t obstacleColor = 0x004477;

	setColor(obstacleColor);

	for (auto &obstacle : prototypes->obstacles)
	{
		size_t count = obstacle.vertices.size() + 1;
		SDL_Point* points = new SDL_Point[count];
		for (size_t i = 0; i < count - 1; i++)
		{
			points[i].x = obstacle.vertices[i].x;
			points[i].y = obstacle.vertices[i].y;
		}
		points[count - 1].x = obstacle.vertices[0].x;
		points[count - 1].y = obstacle.vertices[0].y;
		SDL_RenderDrawLines(renderer, points, count);
		delete[] points;
	}
}

void GameView::drawInput()
{
	uint32_t validColor = 0x00ff00;
	uint32_t invalidColor = 0xff0000;
	uint32_t lastColor = 0;
	int rectSize = 6;

	for (size_t i = 1; i < routeInput->route.size(); i++)
	{
		bool isValidEdge = routeInput->route[i].isValid;
		uint32_t newColor = isValidEdge ? validColor : invalidColor;
		if (lastColor != newColor)
		{
			setColor(newColor);
			lastColor = newColor;
		}
		SDL_RenderDrawLine(renderer,
			(int)routeInput->route[i - 1].x,
			(int)routeInput->route[i - 1].y,
			(int)routeInput->route[i].x,
			(int)routeInput->route[i].y
		);

		//S::log.add(std::to_string(i) + " draw " + routeInput->route[i - 1].toString() + " -> " + routeInput->route[i].toString());

		newColor = routeInput->route[i].isValid ? validColor : invalidColor;
		if (lastColor != newColor)
		{
			setColor(newColor);
			lastColor = newColor;
		}

		SDL_Rect rect;
		rect.x = routeInput->route[i].x - rectSize / 2;
		rect.y = routeInput->route[i].y - rectSize / 2;
		rect.w = rectSize;
		rect.h = rectSize;
		SDL_RenderDrawRect(renderer, &rect);
	}
}

void GameView::drawCars()
{
	uint32_t color = 0x0000ff;
	int rectSize = 4;
	int carSize = 8;
	setColor(color);

	for (PlayerTest &player : state->players)
	{
		for (CarRide &ride : player.activeCars)
		{
			for (size_t i = ride.routeIndex + 1; i < ride.route.size(); i++)
			{
				SDL_RenderDrawLine(renderer,
					(int)ride.route[i - 1].x,
					(int)ride.route[i - 1].y,
					(int)ride.route[i].x,
					(int)ride.route[i].y
				);

				SDL_Rect rect;
				rect.x = ride.route[i].x - rectSize / 2;
				rect.y = ride.route[i].y - rectSize / 2;
				rect.w = rectSize;
				rect.h = rectSize;
				SDL_RenderDrawRect(renderer, &rect);
			}

			Point carLocation = ride.route[ride.routeIndex + 1] - ride.route[ride.routeIndex];
			carLocation.scaleTo(ride.progress * carLocation.getLength());
			carLocation += ride.route[ride.routeIndex];

			SDL_Rect carRect;
			carRect.x = carLocation.x - carSize / 2;
			carRect.y = carLocation.y - carSize / 2;
			carRect.w = carSize;
			carRect.h = carSize;
			SDL_RenderFillRect(renderer, &carRect);
		}
	}
}


void GameView::drawCreeps()
{	
	creepViews.render(renderer, state->creeps, state, prototypes);
	return;
	uint32_t color = 0xff0000;
	int rectSize1 = 10;
	int rectSize2 = 6;
	setColor(color);
	
	for (CreepState &creep : state->creeps)
	{
		int rectSize = creep.weapon.prototypeId == 1 ? rectSize1 : rectSize2;
		SDL_Rect rect;
		rect.x = creep.unit.location.x - rectSize/2;
		rect.y = creep.unit.location.y - rectSize/2;
		rect.w = rectSize;
		rect.h = rectSize;
		SDL_RenderDrawRect(renderer, &rect);
		
		if (creep.unit.force == 1)
		{
			rectSize -= 2;		
			
			rect.x = creep.unit.location.x - rectSize/2;
			rect.y = creep.unit.location.y - rectSize/2;
			rect.w = rectSize;
			rect.h = rectSize;
			SDL_RenderDrawRect(renderer, &rect);
			
			rectSize -= 2;		
			
			rect.x = creep.unit.location.x - rectSize/2;
			rect.y = creep.unit.location.y - rectSize/2;
			rect.w = rectSize;
			rect.h = rectSize;
			SDL_RenderDrawRect(renderer, &rect);
			
		}
	}
}


void GameView::drawProjectiles()
{
	
	uint32_t color = 0xff00ff;
	int rectSize = 2;
	setColor(color);
	
	for (Projectile &projectile : state->projectiles)
	{
		if (projectile.object.prototypeId != 3)
		{
			SDL_Rect rect;
			rect.x = projectile.location.x - rectSize/2;
			rect.y = projectile.location.y - rectSize/2;
			rect.w = rectSize;
			rect.h = rectSize;
			SDL_RenderDrawRect(renderer, &rect);			
		}
	}
	
	color = 0x0;
	rectSize = 3;
	setColor(color);
	
	for (Projectile &projectile : state->projectiles)
	{
		if (projectile.object.prototypeId == 3)
		{
			SDL_Rect rect;
			rect.x = projectile.location.x - rectSize/2;
			rect.y = projectile.location.y - rectSize/2;
			rect.w = rectSize;
			rect.h = rectSize;
			SDL_RenderDrawRect(renderer, &rect);			
		}
	}
}