#include <GameView.h>
#include <Creeps.h>
#include <ViewUtil.h>


GameView::GameView(GPU_Target* screen, Prototypes* prototypes)
{
	this->m_screen = screen;
	this->m_prototypes = prototypes;

	m_isInitialized = false;
	
}

GameView::~GameView()
{
	if (m_image != nullptr)
		GPU_FreeImage(m_image);
}

void GameView::render(GameState* state, RouteInput* routeInput)
{
	if (!m_isInitialized)
		init();

	this->m_state = state;
	this->m_routeInput = routeInput;
	
	SDL_Color white = colorFromHex(0xffffff);
	GPU_RectangleFilled(m_screen, 0, 0, m_prototypes->variables.fieldWidth, m_prototypes->variables.fieldHeight, white);

	drawPlayers();
	drawObstacles();
	drawInput();
	drawCars();
	drawCreeps();
	drawProjectiles();
	drawFormations();	
}

void GameView::init()
{
	m_window = SDL_GetWindowFromID(m_screen->context->windowID);
	SDL_SetWindowSize(m_window, m_prototypes->variables.fieldWidth, m_prototypes->variables.fieldHeight);
	GPU_SetWindowResolution(m_prototypes->variables.fieldWidth, m_prototypes->variables.fieldHeight);
	SDL_SetWindowPosition(m_window, S::config.window_X, S::config.window_Y);
	
	m_image = GPU_LoadImage("out/assets/atlas.png");
 	if (!m_image)
 		THROW_FATAL_ERROR("IMG IS NULL")
 	GPU_SetSnapMode(m_image, GPU_SNAP_NONE);
	 
	m_renderer.init(m_screen, m_image);
	m_isInitialized = true;
	
}

void GameView::drawPlayers()
{
	SDL_Color colors[10] = {
		colorFromHex(0x000000),
		colorFromHex(0xff0000),
		colorFromHex(0x00ff00),
		colorFromHex(0x0000ff),
		colorFromHex(0xffff00),
		colorFromHex(0x00ffff),
		colorFromHex(0xff00ff),
		colorFromHex(0x777777),
		colorFromHex(0x0077ff),
		colorFromHex(0xff7700)
	};

	for (size_t i = 0; i < m_state->players.size(); i++)
	{
		GPU_RectangleFilled(
			m_screen, 
			m_state->players[i].x + 200,
			m_state->players[i].y + 200,
			m_state->players[i].x + 205,
			m_state->players[i].y + 205, 
			colors[i]);
	}
}

void GameView::drawObstacles()
{
	SDL_Color obstacleColor = colorFromHex(0x004477);

	for (auto &obstacle : m_prototypes->obstacles)
	{
		size_t count = obstacle.vertices.size();
		float* points = new float[(count * 2)];
		for (size_t i = 0; i < count * 2; i+=2)
		{
			points[i] = obstacle.vertices[i / 2].x;
			points[i + 1] = obstacle.vertices[i / 2].y;
		}
		
		GPU_Polygon(m_screen, count, points, obstacleColor);
		delete[] points;
		
		int32_t centroidSize = 4;
		GPU_Rectangle(m_screen, 
			obstacle.centroid.x - centroidSize/2, 
			obstacle.centroid.y - centroidSize/2,
			obstacle.centroid.x + centroidSize/2,
			obstacle.centroid.y + centroidSize/2,
			obstacleColor);
	}	
}

void GameView::drawInput()
{
	SDL_Color validColor = colorFromHex(0x00ff00);
	SDL_Color invalidColor = colorFromHex(0xff0000);
	int rectSize = 6;

	for (size_t i = 1; i < m_routeInput->route.size(); i++)
	{
		bool isValidEdge = m_routeInput->route[i].isValid_;
		SDL_Color& newColor = isValidEdge ? validColor : invalidColor;
		GPU_Line(m_screen,
			m_routeInput->route[i - 1].location.x,
			m_routeInput->route[i - 1].location.y,
			m_routeInput->route[i].location.x,
			m_routeInput->route[i].location.y,
			newColor
		);

		//S::log.add(std::to_string(i) + " draw " + routeInput->route[i - 1].toString() + " -> " + routeInput->route[i].toString());

		SDL_Color& newColor2 = m_routeInput->route[i].isValid_ ? validColor : invalidColor;

		GPU_Rectangle(
			m_screen, 
			m_routeInput->route[i].location.x - rectSize / 2,
			m_routeInput->route[i].location.y - rectSize / 2,
			m_routeInput->route[i].location.x + rectSize / 2,
			m_routeInput->route[i].location.y + rectSize / 2,
			newColor2
			);
	}
}

void GameView::drawCars()
{
	SDL_Color color = colorFromHex(0x0000ff);
	int rectSize = 4;
	int carSize = 8;

	for (PlayerTest &player : m_state->players)
	{
		for (CarRide &ride : player.activeCars)
		{
			for (size_t i = ride.routeIndex + 1; i < ride.route.size(); i++)
			{
				GPU_Line(
					m_screen,
					ride.route[i - 1].x,
					ride.route[i - 1].y,
					ride.route[i].x,
					ride.route[i].y,
					color
				);
				GPU_Rectangle(
					m_screen,
					ride.route[i].x - rectSize / 2,
					ride.route[i].y - rectSize / 2,
					ride.route[i].x + rectSize / 2,
					ride.route[i].y + rectSize / 2,
					color
				);
			}

			Point carLocation = ride.route[ride.routeIndex + 1] - ride.route[ride.routeIndex];
			carLocation.scaleTo(ride.progress * carLocation.getLength());
			carLocation += ride.route[ride.routeIndex];

			GPU_RectangleFilled(
					m_screen,
					carLocation.x - carSize / 2,
					carLocation.y - carSize / 2,
					carLocation.x + carSize / 2,
					carLocation.y + carSize / 2,
					color
				);
		}
	}
}


void GameView::drawCreeps()
{	
	m_creepViews.render(&m_renderer, m_state->creeps, m_state, m_prototypes);
}


void GameView::drawProjectiles()
{
	SDL_Color color = colorFromHex(0xff00ff);
	int rectSize = 2;
	
	for (Projectile &projectile : m_state->projectiles)
	{
		GPU_Rectangle(
			m_screen,
			projectile.location.x - rectSize/2,
			projectile.location.y - rectSize/2,
			projectile.location.x + rectSize/2,
			projectile.location.y + rectSize/2,
			color
		);	
	}
}

void GameView::drawFormations()
{
	SDL_Color color = colorFromHex(0xdddd00);
	
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
		
		float points[10];
		
		points[0] = p1.x;
		points[1] = p1.y;		
		points[2] = p2.x;
		points[3] = p2.y;
		points[4] = p3.x;
		points[5] = p3.y;
		points[6] = p4.x;
		points[7] = p4.y;
		points[8] = p1.x;
		points[9] = p1.y;
		
		GPU_Polygon(m_screen, 5, points, color);
		
		//--------------------------------------------------------
		int padding = 2;
		Point pi1 = form.location + (proto.AA + Point(padding, padding)).rotate(form.orientation + M_PI / 2);
		Point pi2 = form.location + (AB + Point(padding, -padding)).rotate(form.orientation + M_PI / 2);
		Point pi3 = form.location + (proto.BB + Point(-padding, -padding)).rotate(form.orientation + M_PI / 2);
		Point pi4 = form.location + (BA + Point(-padding, padding)).rotate(form.orientation + M_PI / 2);
		
		
		points[0] = pi1.x;
		points[1] = pi1.y;		
		points[2] = pi2.x;
		points[3] = pi2.y;
		points[4] = pi3.x;
		points[5] = pi3.y;
		points[6] = pi4.x;
		points[7] = pi4.y;
		points[8] = pi1.x;
		points[9] = pi1.y;
		
		GPU_Polygon(m_screen, 5, points, color);
		
		//-------------------------------------------------------------------
		Point pt1 = form.targetLocation + proto.AA.rotate(form.targetOrientation + M_PI / 2);
		Point pt2 = form.targetLocation + AB.rotate(form.targetOrientation + M_PI / 2);
		Point pt3 = form.targetLocation + proto.BB.rotate(form.targetOrientation + M_PI / 2);
		Point pt4 = form.targetLocation + BA.rotate(form.targetOrientation + M_PI / 2);
		
		
		GPU_Line(m_screen, p1.x, p1.y, pt1.x, pt1.y, color);
		GPU_Line(m_screen, p4.x, p4.y, pt4.x, pt4.y, color);
		
		points[0] = pt1.x;
		points[1] = pt1.y;		
		points[2] = pt2.x;
		points[3] = pt2.y;
		points[4] = pt3.x;
		points[5] = pt3.y;
		points[6] = pt4.x;
		points[7] = pt4.y;
		points[8] = pt1.x;
		points[9] = pt1.y;
		
		GPU_Polygon(m_screen, 5, points, color);
		
		for (size_t i = 0; i < form.slots.size(); i++)
		{
			int slotSize = 4;
			
			auto& slot = form.slots[i];
			Point slotLocation = Creeps::getCurrentSlotLocation(form, i);
				
			
			GPU_Rectangle(
				m_screen,
				slotLocation.x - slotSize/2,
				slotLocation.y - slotSize/2,
				slotLocation.x + slotSize/2,
				slotLocation.y + slotSize/2,
				color
			);
			
			if (slot > 0)
			{
				slotSize /=2;
				GPU_Rectangle(
					m_screen,
					slotLocation.x - slotSize/2,
					slotLocation.y - slotSize/2,
					slotLocation.x + slotSize/2,
					slotLocation.y + slotSize/2,
					color
				);
				
				auto creep = std::find_if(m_state->creeps.begin(), m_state->creeps.end(), [slot](CreepState& creep){ return creep.object.id == slot; });
				if (creep != m_state->creeps.end())
				{
					GPU_Line(
						m_screen, 
						slotLocation.x, 
						slotLocation.y, 
						creep->unit.location.x, 
						creep->unit.location.y, 
						color
					);
				}
			}
		}
	}
}