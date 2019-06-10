#include <GameView.h>
#include <Creeps.h>
#include <ViewUtil.h>
#include <VisualDebug.h>
#include <FMath.h>
#include <std2.h>


GameView::GameView()
{
	m_loadCount = 0;
}

void GameView::init(Renderer* renderer, Prototypes* prototypes)
{
	m_screen = renderer->getScreen();
	m_prototypes = prototypes;
	m_renderer = renderer;
	
}
void GameView::render(GameState* state, RouteInput* routeInput)
{
	m_state = state;
	m_routeInput = routeInput;
	
	if (state->loadCount != m_loadCount)
	{
		m_loadCount = state->loadCount;
		m_projectileExplosions.clear();
		m_unitDeaths.clear();
	}
	
	SDL_Color white = colorFromHex(0xffffff);
	GPU_RectangleFilled(m_screen, 0, 0, m_prototypes->variables.fieldWidth, m_prototypes->variables.fieldHeight, white);

	drawPlayers();
	drawObstacles();
	drawInput();
	drawCars();
	drawUnitExplosion();	
	drawCreeps();
	drawProjectiles();
	//drawFormations();	
	//drawFormationConnections();	
	drawProjectileExplosion();	
	drawDebugGraphics();
	
	lastTime = state->time.time;
}

void GameView::onMouseMove(SDL_MouseMotionEvent* event)
{
	if (!m_state)
		return;
	auto nearestCreep = std2::minElement(m_state->creeps, [event](CreepState& creep){return Point(event->x, event->y).distanceTo(creep.unit.location);});
	if (Point(event->x, event->y).distanceTo(nearestCreep->unit.location) < 50)	
		VisualDebug::interestingId = nearestCreep->object.id;
	else
		VisualDebug::interestingId = -1;
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
#include <unordered_map>
std::unordered_map<int32_t, float> last; //TODO convert to proper CreepView

void GameView::drawCreeps()
{
	for(auto& creep : m_state->creeps)
	{
		//SDL_Color color = colorFromHex(0xff0000, 0x99);
		//SDL_Color color2 = colorFromHex(0x0000ff);
		if (creep.object.prototypeId == 1)
		{/*
			if (creep.unit.force == 0)
				GPU_Circle(m_renderer->getScreen(), creep.unit.location.x, creep.unit.location.y, creep.creepProto_->size, color);
			else
				GPU_CircleFilled(m_renderer->getScreen(), creep.unit.location.x, creep.unit.location.y, creep.creepProto_->size, color);
				*/
			
			float bodyAngle = creep.unit.voluntaryMovement.asAngle();
			float lastAngle = last[creep.object.id];
			float delta = FMath::angleDelta(bodyAngle, lastAngle);
			if (std::fabs(delta) > M_PI / (48.f + ((creep.object.id * 7)% 24)) )
			{
				if (delta > 0)
					bodyAngle = lastAngle + M_PI / (48.f + ((creep.object.id * 7) % 24));
				else
					bodyAngle = lastAngle - M_PI / (48.f + ((creep.object.id * 7) % 24));
			}
			last[creep.object.id] = bodyAngle;
			
			TextureDef& texture = creep.unit.force == 0? S::textures.td.soldier2 : S::textures.td.soldier1;
			m_renderer->blit(texture, creep.unit.location, bodyAngle + M_PI_2, 0.5);
			
			//VisualDebug::drawArrow(creep.unit.location, creep.unit.location + Point::fromAngle(bodyAngle, 30), 0x0000ff);
			//VisualDebug::drawArrow(creep.unit.location, creep.unit.location + Point::fromAngle(creep.unit.voluntaryMovement.asAngle(), 30), 0xff0000);
		}
		else if (creep.object.prototypeId == 0)
		{
			float barrelAngle;
			float bodyAngle;
			if (creep.creepProto_->moveType == MOVE_TYPE::TRACTOR)
				bodyAngle = creep.orientation;
			else
				bodyAngle = creep.unit.voluntaryMovement.asAngle();
			
			if (creep.targetLoc_.getLength() > 0)
				barrelAngle = (creep.targetLoc_ - creep.unit.location).asAngle();
			else
				barrelAngle = bodyAngle;
			
			m_renderer->blit(S::textures.tanks_2.tankBody_sand, creep.unit.location, bodyAngle, 1);
			m_renderer->blit(S::textures.tanks_2.tankSand_barrel1, creep.unit.location, barrelAngle, 1);
			/*
			auto scaledMovement = creep.unit.voluntaryMovement;
			if (scaledMovement.getLength() > 0)
			{
				scaledMovement.scaleTo(100);
				scaledMovement += creep.unit.location;
				
				GPU_Line(m_renderer->getScreen(), creep.unit.location.x, creep.unit.location.y, scaledMovement.x, scaledMovement.y, color);			
			}
			if (creep.targetLoc_.getLength() > 0)
				GPU_Line(m_renderer->getScreen(), creep.unit.location.x, creep.unit.location.y, creep.targetLoc_.x, creep.targetLoc_.y, color2);*/
		}		
		else if (creep.object.prototypeId == 2)
		{
			m_renderer->blit(S::textures.tanks_2.tankBody_huge, creep.unit.location, creep.orientation, 1);
		}
		//GPU_CircleFilled(m_screen, creep.unit.location.x, creep.unit.location.y, creep.creepProto_->size, color);
	}
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

void GameView::drawFormationConnections()
{
	SDL_Color color = colorFromHex(0xff00ff);
	
	for (auto& form : m_state->formations)
	{
		FormationProto& proto = m_prototypes->formations[form.object.prototypeId];
		
		for (size_t i = 0; i < form.slots.size(); i++)
		{
			auto creep = m_state->creepById_[form.slots[i]];
			if (creep && creep->unit.health > 0)
			{
				for(auto& neighbour : proto.slots[i].connections)
				{
					auto creep2 = m_state->creepById_[form.slots[neighbour.slot]];
					if (creep2 && creep2->unit.health > 0)
						GPU_Line(m_screen, creep->unit.location.x, creep->unit.location.y, creep2->unit.location.x, creep2->unit.location.y, color);
				}
			}
		}
	}
}

void GameView::drawFormations()
{
	SDL_Color color = colorFromHex(0xdddd00);
	
	for (auto& form : m_state->formations)
	{
		if (form.subObjective == SUB_OBJECTIVE::ASSAULT)
			continue;
			
		FormationProto& proto = m_prototypes->formations[form.object.prototypeId];
		
		Point AB(proto.AA.x, proto.BB.y);
		Point BA(proto.BB.x, proto.AA.y);
		
		//------------------------------------------------
		Point p1 = form.location + proto.AA.rotate(form.orientation);
		Point p2 = form.location + AB.rotate(form.orientation);
		Point p3 = form.location + proto.BB.rotate(form.orientation);
		Point p4 = form.location + BA.rotate(form.orientation);
		
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
		Point pi1 = form.location + (proto.AA + Point(padding, padding)).rotate(form.orientation);
		Point pi2 = form.location + (AB + Point(padding, -padding)).rotate(form.orientation);
		Point pi3 = form.location + (proto.BB + Point(-padding, -padding)).rotate(form.orientation);
		Point pi4 = form.location + (BA + Point(-padding, padding)).rotate(form.orientation);
		
		
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
		Point pt1 = form.targetLocation + proto.AA.rotate(form.targetOrientation);
		Point pt2 = form.targetLocation + AB.rotate(form.targetOrientation);
		Point pt3 = form.targetLocation + proto.BB.rotate(form.targetOrientation);
		Point pt4 = form.targetLocation + BA.rotate(form.targetOrientation);
		
		
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
					GPU_Circle(m_screen, creep->formationAttraction_.x, creep->formationAttraction_.y, 2, color);
					GPU_Circle(m_screen, creep->formationAttraction_.x, creep->formationAttraction_.y, 5, color);
					GPU_Line(
						m_screen, 
						creep->formationAttraction_.x, 
						creep->formationAttraction_.y, 
						creep->unit.location.x, 
						creep->unit.location.y, 
						color
					);
				}/**/
			}
		}
	}
}

void GameView::drawProjectileExplosion()
{
	if (!m_state->isEventLoggerEnabled)
		return;
	m_projectileExplosions.render(
			m_renderer, 
			m_state->eventLogger.projectileExplosions.begin(), 
			m_state->eventLogger.projectileExplosions.end(),
			m_state, 
			m_prototypes
		);
	/*
	size_t projectilesTotal = std::min(m_state->eventLogger.projectileExplosions.size, m_state->eventLogger.projectileExplosions.total);
	int32_t minTime = m_state->time.time - m_prototypes->variables.maxEventAnimationTime;
	
	for(size_t i = 0; i < projectilesTotal; i++)
	{
		ProjectileExplosionEvent& event = m_state->eventLogger.projectileExplosions.array[i];
		if (event.stamp > minTime && event.stamp < m_state->time.time)
		{
			int32_t seed = FMath::q_sdbm(id);
			if (m_seed != seed)
				init(seed, state, prototypes);
			m_renderer->blit(S::textures.tanks_1.Smoke.smokeGrey2, event.location, 0, 0.2, 0x55);
		}
	}*/
}

void GameView::drawUnitExplosion()
{
	if (!m_state->isEventLoggerEnabled)
		return;
	m_unitDeaths.render(
			m_renderer, 
			m_state->eventLogger.unitDeaths.begin(), 
			m_state->eventLogger.unitDeaths.end(),
			m_state, 
			m_prototypes
		);	
		/*
	size_t unitsTotal = std::min(m_state->eventLogger.unitDeaths.size, m_state->eventLogger.unitDeaths.total);
	int32_t minTime = m_state->time.time - m_prototypes->variables.maxEventAnimationTime;
	
	for(size_t i = 0; i < unitsTotal; i++)
	{
		UnitDeathEvent& event = m_state->eventLogger.unitDeaths.array[i];
		if (event.stamp > minTime && event.stamp < m_state->time.time)
			m_renderer->blit(S::textures.tanks_1.Smoke.smokeOrange0, event.location, 0, 0.2, 0x55);
	}*/
}

void GameView::drawDebugGraphics()
{
	for(auto& line : S::visualDebug.lines)
	{
		SDL_Color color = colorFromHex(line.color, 0xff);
		GPU_Line(m_screen, line.from.x, line.from.y, line.to.x, line.to.y, color);
	}
	
	for(auto& arrow : S::visualDebug.arrows)
	{
		SDL_Color color = colorFromHex(arrow.color, 0xff);
		GPU_Line(m_screen, arrow.from.x, arrow.from.y, arrow.to.x, arrow.to.y, color);
		
		Point vec = arrow.to - arrow.from;
		
		float angle = M_PI / 5;
		float headSize = FMath::lerp(0.f, 0.f, 100.f, 8.f, vec.getLength());
		headSize = std::min(headSize, 10.f);
		headSize++;
		
		vec = vec.rotate(angle);
		vec.scaleTo(headSize);
		
		Point branch = arrow.to - vec;
		
		GPU_Line(m_screen, arrow.to.x, arrow.to.y, branch.x, branch.y, color);
		
		vec = vec.rotate(-angle*2);
		branch = arrow.to - vec;
		
		GPU_Line(m_screen, arrow.to.x, arrow.to.y, branch.x, branch.y, color);
	}
	
	for(auto& rect : S::visualDebug.rects)
	{
		SDL_Color color = colorFromHex(rect.color, rect.alpha);
		if (rect.fill)
			GPU_RectangleFilled2(m_screen, rect.rect, color);
		else
			GPU_Rectangle2(m_screen, rect.rect, color);
	}
	
	for(auto& circle : S::visualDebug.circles)
	{
		SDL_Color color = colorFromHex(circle.color, circle.alpha);
		if (circle.fill)
			GPU_CircleFilled(m_screen, circle.origin.x, circle.origin.y, circle.radius, color);
		else
			GPU_Circle(m_screen, circle.origin.x, circle.origin.y, circle.radius, color);
	}
}