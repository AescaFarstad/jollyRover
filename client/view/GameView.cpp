#include <GameView.h>
#include <Creeps.h>


GameView::GameView(SDL_Window* window, SDL_Renderer* renderer, Prototypes* prototypes)
{
	this->m_window = window;
	this->m_renderer = renderer;
	this->m_prototypes = prototypes;

	m_isInitialized = false;
	
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
	if (!m_isInitialized)
		init();

	this->m_state = state;
	this->m_routeInput = routeInput;

	drawPlayers();
	drawObstacles();
	drawInput();
	drawCars();
	drawCreeps();
	drawProjectiles();
	drawFormations();
	//rendery(renderer, loadedSurface, ltexture);
	
}


void GameView::init()
{
	SDL_SetWindowSize(m_window, m_prototypes->variables.fieldWidth, m_prototypes->variables.fieldHeight);
	m_isInitialized = true;
	//loadedSurface = IMG_Load("out/assets/sheet_tanks.png");
	//if(!loadedSurface) {
    //	printf("IMG_Load: %s\n", IMG_GetError());}
	//ltexture = SDL_CreateTextureFromSurface(m_renderer, loadedSurface);
}

void GameView::setColor(uint32_t color)
{
	SDL_SetRenderDrawColor(m_renderer, (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff, 0xFF);
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

	for (size_t i = 0; i < m_state->players.size(); i++)
	{
		setColor(colors[i]);

		SDL_Rect rect;
		rect.x = m_state->players[i].x + 200;
		rect.y = m_state->players[i].y + 200;
		rect.w = 5;
		rect.h = 5;
		SDL_RenderFillRect(m_renderer, &rect);
	}
}

void GameView::drawObstacles()
{
	uint32_t obstacleColor = 0x004477;
	int centroidSize = 4;

	setColor(obstacleColor);

	for (auto &obstacle : m_prototypes->obstacles)
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
		SDL_RenderDrawLines(m_renderer, points, count);
		delete[] points;
		
		SDL_Rect rect;
		rect.x = obstacle.centroid.x - centroidSize/2;
		rect.y = obstacle.centroid.y - centroidSize/2;
		rect.w = centroidSize;
		rect.h = centroidSize;
		SDL_RenderDrawRect(m_renderer, &rect);
	}
}

void GameView::drawInput()
{
	uint32_t validColor = 0x00ff00;
	uint32_t invalidColor = 0xff0000;
	uint32_t lastColor = 0;
	int rectSize = 6;

	for (size_t i = 1; i < m_routeInput->route.size(); i++)
	{
		bool isValidEdge = m_routeInput->route[i].isValid_;
		uint32_t newColor = isValidEdge ? validColor : invalidColor;
		if (lastColor != newColor)
		{
			setColor(newColor);
			lastColor = newColor;
		}
		SDL_RenderDrawLine(m_renderer,
			(int)m_routeInput->route[i - 1].location.x,
			(int)m_routeInput->route[i - 1].location.y,
			(int)m_routeInput->route[i].location.x,
			(int)m_routeInput->route[i].location.y
		);

		//S::log.add(std::to_string(i) + " draw " + routeInput->route[i - 1].toString() + " -> " + routeInput->route[i].toString());

		newColor = m_routeInput->route[i].isValid_ ? validColor : invalidColor;
		if (lastColor != newColor)
		{
			setColor(newColor);
			lastColor = newColor;
		}

		SDL_Rect rect;
		rect.x = m_routeInput->route[i].location.x - rectSize / 2;
		rect.y = m_routeInput->route[i].location.y - rectSize / 2;
		rect.w = rectSize;
		rect.h = rectSize;
		SDL_RenderDrawRect(m_renderer, &rect);
	}
}

void GameView::drawCars()
{
	uint32_t color = 0x0000ff;
	int rectSize = 4;
	int carSize = 8;
	setColor(color);

	for (PlayerTest &player : m_state->players)
	{
		for (CarRide &ride : player.activeCars)
		{
			for (size_t i = ride.routeIndex + 1; i < ride.route.size(); i++)
			{
				SDL_RenderDrawLine(m_renderer,
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
				SDL_RenderDrawRect(m_renderer, &rect);
			}

			Point carLocation = ride.route[ride.routeIndex + 1] - ride.route[ride.routeIndex];
			carLocation.scaleTo(ride.progress * carLocation.getLength());
			carLocation += ride.route[ride.routeIndex];

			SDL_Rect carRect;
			carRect.x = carLocation.x - carSize / 2;
			carRect.y = carLocation.y - carSize / 2;
			carRect.w = carSize;
			carRect.h = carSize;
			SDL_RenderFillRect(m_renderer, &carRect);
		}
	}
}


void GameView::drawCreeps()
{	
	m_creepViews.render(m_renderer, m_state->creeps, m_state, m_prototypes);
	return;
	uint32_t color = 0xff0000;
	int rectSize1 = 10;
	int rectSize2 = 6;
	setColor(color);
	
	for (CreepState &creep : m_state->creeps)
	{
		int rectSize = creep.weapon.prototypeId == 1 ? rectSize1 : rectSize2;
		SDL_Rect rect;
		rect.x = creep.unit.location.x - rectSize/2;
		rect.y = creep.unit.location.y - rectSize/2;
		rect.w = rectSize;
		rect.h = rectSize;
		SDL_RenderDrawRect(m_renderer, &rect);
		
		if (creep.unit.force == 1)
		{
			rectSize -= 2;		
			
			rect.x = creep.unit.location.x - rectSize/2;
			rect.y = creep.unit.location.y - rectSize/2;
			rect.w = rectSize;
			rect.h = rectSize;
			SDL_RenderDrawRect(m_renderer, &rect);
			
			rectSize -= 2;		
			
			rect.x = creep.unit.location.x - rectSize/2;
			rect.y = creep.unit.location.y - rectSize/2;
			rect.w = rectSize;
			rect.h = rectSize;
			SDL_RenderDrawRect(m_renderer, &rect);
			
		}
	}
}


void GameView::drawProjectiles()
{
	
	uint32_t color = 0xff00ff;
	int rectSize = 2;
	setColor(color);
	
	for (Projectile &projectile : m_state->projectiles)
	{
		if (projectile.object.prototypeId != 3)
		{
			SDL_Rect rect;
			rect.x = projectile.location.x - rectSize/2;
			rect.y = projectile.location.y - rectSize/2;
			rect.w = rectSize;
			rect.h = rectSize;
			SDL_RenderDrawRect(m_renderer, &rect);			
		}
	}
	
	color = 0x0;
	rectSize = 3;
	setColor(color);
	
	for (Projectile &projectile : m_state->projectiles)
	{
		if (projectile.object.prototypeId == 3)
		{
			SDL_Rect rect;
			rect.x = projectile.location.x - rectSize/2;
			rect.y = projectile.location.y - rectSize/2;
			rect.w = rectSize;
			rect.h = rectSize;
			SDL_RenderDrawRect(m_renderer, &rect);			
		}
	}
}

void GameView::drawFormations()
{
	uint32_t color = 0xdddd00;
	setColor(color);
	
	for (auto& form : m_state->formations)
	{
		FormationProto& proto = m_prototypes->formations[form.object.prototypeId];
		
		Point AB(proto.AA.x, proto.BB.y);
		Point BA(proto.BB.x, proto.AA.y);
		
		//------------------------------------------------
		Point p1 = form.location + proto.AA.rotate(form.orientation + M_PI / 2);
		Point p2 = form.location + AB.rotate(form.orientation + M_PI / 2);
		Point p3 = form.location + proto.BB.rotate(form.orientation + M_PI / 2);
		Point p4 = form.location + BA.rotate(form.orientation + M_PI / 2);
		
		SDL_Point points[5];
		
		points[0].x = p1.x;
		points[0].y = p1.y;		
		points[1].x = p2.x;
		points[1].y = p2.y;
		points[2].x = p3.x;
		points[2].y = p3.y;
		points[3].x = p4.x;
		points[3].y = p4.y;
		points[4].x = p1.x;
		points[4].y = p1.y;
		
		SDL_RenderDrawLines(m_renderer, points, 5);
		
		//--------------------------------------------------------
		int padding = 2;
		Point pi1 = form.location + (proto.AA + Point(padding, padding)).rotate(form.orientation + M_PI / 2);
		Point pi2 = form.location + (AB + Point(padding, -padding)).rotate(form.orientation + M_PI / 2);
		Point pi3 = form.location + (proto.BB + Point(-padding, -padding)).rotate(form.orientation + M_PI / 2);
		Point pi4 = form.location + (BA + Point(-padding, padding)).rotate(form.orientation + M_PI / 2);
		
		
		points[0].x = pi1.x;
		points[0].y = pi1.y;		
		points[1].x = pi2.x;
		points[1].y = pi2.y;
		points[2].x = pi3.x;
		points[2].y = pi3.y;
		points[3].x = pi4.x;
		points[3].y = pi4.y;
		points[4].x = pi1.x;
		points[4].y = pi1.y;
		
		SDL_RenderDrawLines(m_renderer, points, 5);
		
		//-------------------------------------------------------------------
		Point pt1 = form.targetLocation + proto.AA.rotate(form.targetOrientation + M_PI / 2);
		Point pt2 = form.targetLocation + AB.rotate(form.targetOrientation + M_PI / 2);
		Point pt3 = form.targetLocation + proto.BB.rotate(form.targetOrientation + M_PI / 2);
		Point pt4 = form.targetLocation + BA.rotate(form.targetOrientation + M_PI / 2);
		
		SDL_RenderDrawLine(m_renderer, p1.x, p1.y, pt1.x, pt1.y);
		SDL_RenderDrawLine(m_renderer, p4.x, p4.y, pt4.x, pt4.y);
		
		points[0].x = pt1.x;
		points[0].y = pt1.y;		
		points[1].x = pt2.x;
		points[1].y = pt2.y;
		points[2].x = pt3.x;
		points[2].y = pt3.y;
		points[3].x = pt4.x;
		points[3].y = pt4.y;
		points[4].x = pt1.x;
		points[4].y = pt1.y;
		
		SDL_RenderDrawLines(m_renderer, points, 5);
		
		for (size_t i = 0; i < form.slots.size(); i++)
		{
			int slotSize = 4;
			
			auto& slot = form.slots[i];
			Point slotLocation = Creeps::getCurrentSlotLocation(form, i);
				
			SDL_Rect rect;
			rect.x = slotLocation.x - slotSize/2;
			rect.y = slotLocation.y - slotSize/2;
			rect.w = slotSize;
			rect.h = slotSize;
			SDL_RenderDrawRect(m_renderer, &rect);			
			
			if (slot > 0)
			{
				slotSize /=2;
				rect.x = slotLocation.x - slotSize/2;
				rect.y = slotLocation.y - slotSize/2;
				rect.w = slotSize;
				rect.h = slotSize;
				SDL_RenderDrawRect(m_renderer, &rect);
				
				auto creep = std::find_if(m_state->creeps.begin(), m_state->creeps.end(), [slot](CreepState& creep){ return creep.object.id == slot; });
				if (creep != m_state->creeps.end())
					SDL_RenderDrawLine(m_renderer, slotLocation.x, slotLocation.y, creep->unit.location.x, creep->unit.location.y);
			}
		}
	}
}