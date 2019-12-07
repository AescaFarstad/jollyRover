#include <GameView.h>
#include <Creeps.h>
#include <ViewUtil.h>
#include <VisualDebug.h>
#include <FMath.h>
#include <std2.h>


GameView::GameView()
{
	m_loadCount = 0;
	
	m_layer1Image = loadImage("out/assets/map layer 1.png");
	m_layer1Image->anchor_x = 0;
	m_layer1Image->anchor_y = 0;
	m_layer2Image = loadImage("out/assets/map layer 2.png");
	m_layer2Image->anchor_x = 0;
	m_layer2Image->anchor_y = 0;
	m_layer3Image = loadImage("out/assets/map layer 3.png"); 
	m_layer3Image->anchor_x = 0;
	m_layer3Image->anchor_y = 0;
	m_fontAmaticBold.load("out/assets/Amatic-Bold.ttf", 32);
	m_fontAmaticRegular.load("out/assets/AmaticSC-Regular.ttf", 32);
}

GameView::~GameView()
{
	if (m_layer1Image)
		delete m_layer1Image;
	if (m_layer2Image)
		delete m_layer2Image;
	if (m_layer3Image)
		delete m_layer3Image;
}


GPU_Image* GameView::loadImage(std::string path)
{
	GPU_Image* result = GPU_LoadImage(path.c_str());
	if (!result)
 		THROW_FATAL_ERROR("IMG IS NULL")
 	GPU_SetSnapMode(result, GPU_SNAP_NONE);
	GPU_SetBlending(result, 1);
	return result;
}

void GameView::init(Renderer* renderer, Prototypes* prototypes)
{
	m_screen = renderer->getScreen();
	m_prototypes = prototypes;
	m_renderer = renderer;
	
	resolveTextures(prototypes);
}

void GameView::resolveTextures(Prototypes* prototypes)
{
	for(auto& car: prototypes->cars)
	{
		car.opponentCarGunTexture = S::textures.getTextureByName(car.opponentCarGunTextureName);
		car.opponentCarHullTexture = S::textures.getTextureByName(car.opponentCarHullTextureName);
		car.playerCarGunTexture = S::textures.getTextureByName(car.playerCarGunTextureName);
		car.playerCarHullTexture = S::textures.getTextureByName(car.playerCarHullTextureName);
		
		assert(car.opponentCarGunTexture != nullptr || car.opponentCarGunTextureName == "");
		assert(car.opponentCarHullTexture != nullptr || car.opponentCarHullTextureName == "");
		assert(car.playerCarGunTexture != nullptr || car.playerCarGunTextureName == "");
		assert(car.playerCarHullTexture != nullptr || car.playerCarHullTextureName == "");
	}
	
	for(auto& creep: prototypes->creeps)
	{
		for(size_t i = 0; i < creep.gunTextureName.size(); i++)
		{
			creep.hullTexture.push_back(S::textures.getTextureByName(creep.hullTextureName[i]));
			creep.gunTexture.push_back(S::textures.getTextureByName(creep.gunTextureName[i]));
			
			assert(creep.hullTexture[i] != nullptr || creep.hullTextureName[i] == "");
			assert(creep.gunTexture[i] != nullptr || creep.gunTextureName[i] == "");
		}			
	}
	for(auto& weapon: prototypes->weapons)
	{
		for(size_t i = 0; i < weapon.projectileTextureName.size(); i++)
		{
			weapon.projectileTexture.push_back(S::textures.getTextureByName(weapon.projectileTextureName[i]));
			assert(weapon.projectileTexture[i] != nullptr || weapon.projectileTextureName[i] == "");	
		}
	}
}

void GameView::render(GameState* state, RouteInput* routeInput)
{
	m_state = state;
	m_routeInput = routeInput;
	
	if (state->loadCount != m_loadCount)
	{
		m_loadCount = state->loadCount;
		m_projectileExplosions.clear();
		m_creepDeaths.clear();
		m_carDeaths.clear();
	}
	
	SDL_Color white = ViewUtil::colorFromHex(0xffffff);
	GPU_RectangleFilled(m_screen, 0, 0, m_prototypes->variables.fieldWidth, m_prototypes->variables.fieldHeight, white);
	
	GPU_Rect layerRect{0, 0, (float)m_layer1Image->w, (float)m_layer1Image->h};
	Point location(0, 0);

	m_renderer->blit(m_layer1Image, layerRect, location);
	drawPlayers();
	drawUnitExplosion();
	m_renderer->blit(m_layer2Image, layerRect, location);	
	drawCreeps();
	drawCars();
	drawProjectiles();
	m_renderer->blit(m_layer3Image, layerRect, location);
	//drawFormations();	
	//drawFormationConnections();	
	drawProjectileExplosion();	
	drawDebugGraphics();
	//drawThreatMap();
	//drawObstacles();
	drawInput();
	
	drawHUD();
	
	lastTime = state->time.time;
}
void GameView::setLogin(int32_t login)
{
	m_login = login;
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
	//...
}

void GameView::drawObstacles()
{
	SDL_Color obstacleColor = ViewUtil::colorFromHex(0x004477);

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
	SDL_Color validColor = ViewUtil::colorFromHex(0x00ff00);
	SDL_Color invalidColor = ViewUtil::colorFromHex(0xff0000);
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
	SDL_Color myColor = ViewUtil::colorFromHex(0x0000ff);
	SDL_Color theirColor = ViewUtil::colorFromHex(0x555555);
	int rectSize = 4;

	for (PlayerState &player : m_state->players)
	{
		for (CarState &car : player.activeCars)
		{
			if ((size_t)car.routeIndex >= car.route.size() - 1)
				continue;
			for (size_t i = car.routeIndex + 1; i < car.route.size(); i++)
			{
				GPU_Line(
					m_screen,
					car.route[i - 1].x,
					car.route[i - 1].y,
					car.route[i].x,
					car.route[i].y,
					player.login == m_login ? myColor : theirColor
				);
				GPU_Rectangle(
					m_screen,
					car.route[i].x - rectSize / 2,
					car.route[i].y - rectSize / 2,
					car.route[i].x + rectSize / 2,
					car.route[i].y + rectSize / 2,
					player.login == m_login ? myColor : theirColor
				);
			}
			auto& proto = m_prototypes->cars[car.object.prototypeId];			
			auto& carTexture = player.login == m_login ? 
				proto.playerCarHullTexture : 
				proto.opponentCarHullTexture;
			auto& carGunTexture = player.login == m_login ? 
				proto.playerCarGunTexture : 
				proto.opponentCarGunTexture;
				
			size_t fromIndex;
			size_t toIndex;
			float ratio;
			if (car.progress > 0.5)
			{
				fromIndex = car.routeIndex;
				toIndex = car.routeIndex + 1;
				if (toIndex == car.route.size())
					toIndex = fromIndex;
				ratio = car.progress - 0.5;
			}
			else
			{
				fromIndex = car.routeIndex - 1;
				toIndex = car.routeIndex;
				if (fromIndex < 0)
					fromIndex = 0;
				ratio = car.progress + 0.5;
			}
			auto vec1 = car.route[fromIndex + 1] - car.route[fromIndex];
			auto vec2 = car.route[toIndex + 1] - car.route[toIndex];
			float angle =  (vec1 * (1 - ratio) + vec2 * ratio).asAngle();
			
			m_renderer->blit(*carTexture, car.unit.location, angle, 0.8);
			auto gunLocation = Point::fromAngle(angle, 9);
			gunLocation += car.unit.location;
			m_renderer->blit(*carGunTexture, gunLocation, angle, 0.8);
			/*
			GPU_RectangleFilled(
					m_screen,
					car.unit.location.x - carSize / 2,
					car.unit.location.y - carSize / 2,
					car.unit.location.x + carSize / 2,
					car.unit.location.y + carSize / 2,
					player.login == m_login ? myColor : theirColor
				);*/
		}
	}
}
#include <unordered_map>
std::unordered_map<int32_t, float> last; //TODO convert to proper CreepView

void GameView::drawCreeps()
{
	for(auto& creep : m_state->creeps)
	{
		//SDL_Color color = ViewUtil::colorFromHex(0xff0000, 0x99);
		//SDL_Color color2 = ViewUtil::colorFromHex(0x0000ff);
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
			m_renderer->blit(*creep.creepProto_->hullTexture[creep.unit.force], creep.unit.location, bodyAngle + M_PI_2, 0.5);
			
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
						
			m_renderer->blit(*creep.creepProto_->hullTexture[creep.unit.force], creep.unit.location, bodyAngle, 1);
			m_renderer->blit(*creep.creepProto_->gunTexture[creep.unit.force], creep.unit.location, barrelAngle, 1);
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
		//GPU_CircleFilled(m_screen, creep.unit.location.x, creep.unit.location.y, creep.creepProto_->size, color);
	}
}


void GameView::drawProjectiles()
{
	SDL_Color color = ViewUtil::colorFromHex(0xff00ff);
	int rectSize = 2;
	
	for (Projectile &projectile : m_state->projectiles)
	{
		
		if (projectile.weapon == 0)
		{
			if (m_state->time.time - projectile.spawnedAt < 20 / projectile.speed * 1000)
				continue; //Shell is inside the barrel
			TextureDef* texture = m_prototypes->weapons[projectile.weapon].projectileTexture[projectile.force];
			m_renderer->blit(*texture, projectile.location, (projectile.target - projectile.location).asAngle(), 0.7);
		}
		else
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
}

void GameView::drawFormationConnections()
{
	SDL_Color color = ViewUtil::colorFromHex(0xff00ff);
	
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
	SDL_Color color = ViewUtil::colorFromHex(0xdddd00);
	
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
		
		int32_t radius = FMath::lerpClipped(0, 2, form.agroAt, 10, form.agro_);
		GPU_CircleFilled(m_screen, form.actualLocation_.x, form.actualLocation_.y, radius, color);
		
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
			m_prototypes,
			m_login
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
	m_creepDeaths.render(
			m_renderer, 
			m_state->eventLogger.creepDeaths.begin(), 
			m_state->eventLogger.creepDeaths.end(),
			m_state, 
			m_prototypes,
			m_login
		);	
	m_carDeaths.render(
			m_renderer, 
			m_state->eventLogger.carDeaths.begin(), 
			m_state->eventLogger.carDeaths.end(),
			m_state, 
			m_prototypes,
			m_login
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
		SDL_Color color = ViewUtil::colorFromHex(line.color, 0xff);
		GPU_Line(m_screen, line.from.x, line.from.y, line.to.x, line.to.y, color);
	}
	
	for(auto& arrow : S::visualDebug.arrows)
	{
		SDL_Color color = ViewUtil::colorFromHex(arrow.color, 0xff);
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
		SDL_Color color = ViewUtil::colorFromHex(rect.color, rect.alpha);
		if (rect.fill)
			GPU_RectangleFilled2(m_screen, rect.rect, color);
		else
			GPU_Rectangle2(m_screen, rect.rect, color);
	}
	
	for(auto& circle : S::visualDebug.circles)
	{
		SDL_Color color = ViewUtil::colorFromHex(circle.color, circle.alpha);
		if (circle.fill)
			GPU_CircleFilled(m_screen, circle.origin.x, circle.origin.y, circle.radius, color);
		else
			GPU_Circle(m_screen, circle.origin.x, circle.origin.y, circle.radius, color);
	}
}

void GameView::drawThreatMap()
{
	const std::vector<std::vector<int32_t>>* maps[2];
	maps[0] = m_state->threatMap_[0].getRawData();
	maps[1] = m_state->threatMap_[1].getRawData();
	const Point AA = m_state->threatMap_[0].getAA();
	int32_t cellSize = m_state->threatMap_[0].getCellSize();
	
	SDL_Color colors[2] = {ViewUtil::colorFromHex(0xff0000), ViewUtil::colorFromHex(0x0000ff)};
	
	for(size_t c = 0; c < 2; c++)
	{
		for(size_t i = 1; i < maps[c]->size() - 1; i++)
		{
			for(size_t j = 1; j < (*maps[c])[i].size() - 1; j++)
			{
				SDL_Color color = colors[c];
				color.a = (int8_t)FMath::lerpClipped(0, 0, 1000, 150, (*maps[c])[i][j]);
				GPU_RectangleFilled(
					m_screen, 
					AA.x + cellSize * (i - 1),
					AA.y + cellSize * (j - 1),
					AA.x + cellSize * (i),
					AA.y + cellSize * (j),
					color
				);					
			}
		}
	}
}

void GameView::drawHUD()
{
	//m_fontAmaticBold.draw(m_screen, 20.f, 20.f, "Score: %d", m_state->timeStamp);
	
	std::vector<PlayerState> players = m_state->players;
	std::sort(players.begin(), players.end(), [](PlayerState& a, PlayerState& b){ return a.score > b.score;});
	int32_t i = 0;
	for(auto& p : players)
	{
		std::string text = "";
		if (!p.isHeadless)
		{
			if (p.login == m_login)
				text += "YOU";
			else
				text += "player " + std::to_string(p.login);
			if (p.isAI)
				text += " (AI)";
		}
		else
		{
			text += "AI " + std::to_string(-p.login);
		}
		text += ":  " +  std::to_string(p.score);
		
		for(auto& car : p.activeCars)
		{
			if (car.score > 0)
				text += "  +  " +  std::to_string(car.score);
		}
		
		m_fontAmaticBold.draw(m_screen, 5.f, 20.f + 30 * i, "%s", text.c_str());
		
		i++;
	}
}