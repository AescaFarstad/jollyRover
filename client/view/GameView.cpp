#include <GameView.h>
#include <Creeps.h>
#include <ViewUtil.h>
#include <VisualDebug.h>
#include <FMath.h>
#include <std2.h>
#include <FPSMeter.h>
#include <DrawSettings.h>
#include <GameLogic.h>
#include <FPSMeter.h>


GameView::GameView()
{
	m_loadCount = 0;
}

GameView::~GameView()
{
	if (m_layer1Image)
		GPU_FreeImage(m_layer1Image);
	if (m_layer2Image)
		GPU_FreeImage(m_layer2Image);
	if (m_layer3Image)
		GPU_FreeImage(m_layer3Image);
}

void GameView::init(Renderer* renderer, Prototypes* prototypes)
{
	m_screen = renderer->getScreen();
	m_prototypes = prototypes;
	m_renderer = renderer;
	
	m_layer1Image = loadImage("out/assets/map layer 1.png");
	m_layer1Image->anchor_x = 0;
	m_layer1Image->anchor_y = 0;
	m_layer2Image = loadImage("out/assets/map layer 2.png");
	m_layer2Image->anchor_x = 0;
	m_layer2Image->anchor_y = 0;
	m_layer3Image = loadImage("out/assets/map layer 3.png"); 
	m_layer3Image->anchor_x = 0;
	m_layer3Image->anchor_y = 0;	
	
	resolveTextures(prototypes);
}

GPU_Image* GameView::loadImage(std::string path)
{
	auto result = GPU_LoadImage(path.c_str());
	if (!result)
 		THROW_FATAL_ERROR("IMG IS NULL")
 	GPU_SetSnapMode(result, GPU_SNAP_NONE);
	GPU_SetBlending(result, 1);
	return result;
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

	if (S::drawSettings.layer1)
		m_renderer->blit(m_layer1Image, layerRect, location);
	if (S::drawSettings.remnants)
		drawRemnants();
	if (S::drawSettings.layer2)
		m_renderer->blit(m_layer2Image, layerRect, location);
	if (S::drawSettings.cars)
		drawCars();	
	if (S::drawSettings.creeps)
		drawCreeps();
	if (S::drawSettings.projectiles)
		drawProjectiles();
	if (S::drawSettings.layer3)
		m_renderer->blit(m_layer3Image, layerRect, location);
	if (S::drawSettings.formations_D)
		drawFormations();	
	if (S::drawSettings.connections_D)
		drawFormationConnections();	
	if (S::drawSettings.formAgro_D)
		drawFormationAgro();
	if (S::drawSettings.explosions)
		drawProjectileExplosion();
	if (S::drawSettings.obstacles_D)
		drawObstacles();	
	if (S::drawSettings.threat_D)
		drawThreatMap();
	if (S::drawSettings.input)
		drawInput();	
	if (S::drawSettings.debug_D)
		drawDebugGraphics();
	if (S::drawSettings.hud)
		drawHUD();
	if (S::drawSettings.flyingMessage)		
		drawFlyingMessages();
	drawPhantomRoutes();
	
	m_lastTime = state->time.time;
}

void GameView::addMessage(std::string message, Point location, NFont::AlignEnum aligment)
{
	m_flyingMessages.add(message, location, m_state->time.time, aligment);
}

void GameView::addPhantomRoute(std::vector<RoutePoint> route)
{
	m_phantomRoutes.add(route, m_state->time.time);
}
	

void GameView::setLogin(int32_t login)
{
	m_login = login;
}

void GameView::onMouseMove(const SDL_MouseMotionEvent& event)
{
	if (!m_state)
		return;
	auto nearestCreep = std2::minElement(m_state->creeps, [event](CreepState& creep){return Point(event.x, event.y).distanceTo(creep.unit.location);});
	if (Point(event.x, event.y).distanceTo(nearestCreep->unit.location) < 50)	
		VisualDebug::interestingId = nearestCreep->unit.id;
	else
		VisualDebug::interestingId = -1;
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

void GameView::renderPathStep(Point from, Point to, SequenceDef& dash, SequenceDef& dot, uint32_t rndSeed, float ratio)
{
	SeededRandom rnd(rndSeed);
	{
		auto& texture = rnd.getFromVector(dash.frames);
		auto loc = (from + to) / 2;
		m_renderer->blit(texture, loc, (to - from).asAngle(), 0.5, FMath::lerp_ui8(0, 0xaa, 0.5, 0, ratio));
	}
	{
		auto& texture = rnd.getFromVector(dot.frames);
		m_renderer->blit(texture, to, rnd.getAngle(), 0.5, FMath::lerp_ui8(0.5, 0xaa, 1, 0, ratio));
	}
}

void GameView::drawInput()
{
	
	auto& route = m_routeInput->getRoutePoints();

	for (size_t i = 1; i < route.size(); i++)
	{
		auto& dashSeq = route[i].isValid_ ? S::sequences.greenPathDash : S::sequences.redPathDash;
		auto& dotSeq = route[i].isValid_ ? S::sequences.greenPathDot : S::sequences.redPathDot;
		renderPathStep(route[i - 1].location, route[i].location, dashSeq, dotSeq, m_login + i, 0);
	}
}

void GameView::drawCars()
{
	for (PlayerState &player : m_state->players)
	{
		for (CarState &car : player.activeCars)
		{
			if ((size_t)car.routeIndex >= car.route.size() - 1)
				continue;
				
			//Dotted line
			size_t upperLimit = FMath::lerp(0, 0, 1000, 80, m_state->time.time - car.startStamp);
			if (player.login == m_login)
				upperLimit = car.route.size();
			upperLimit = std::min(car.route.size(), upperLimit);
			for (size_t i = car.routeIndex + 2; i < upperLimit; i++)
			{
				auto ratio = (i == (size_t)car.routeIndex + 2) ? car.progress : 0;
				auto& dashSeq = player.login == m_login ? S::sequences.greenPathDash : S::sequences.grayPathDash;
				auto& dotSeq = player.login == m_login ? S::sequences.greenPathDot : S::sequences.grayPathDot;
				renderPathStep(car.route[i - 1], car.route[i], dashSeq, dotSeq, player.login + i, ratio);
			}
			
			//Car
			auto& proto = m_prototypes->cars[car.unit.prototypeId];			
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
				if (toIndex == car.route.size() - 1)
					toIndex = fromIndex;
				ratio = car.progress - 0.5;
			}
			else
			{
				
				fromIndex = car.routeIndex > 0 ? car.routeIndex - 1 : 0;
				toIndex = car.routeIndex;
				ratio = car.progress + 0.5;
			}
			auto vec1 = car.route[fromIndex + 1] - car.route[fromIndex];
			auto vec2 = car.route[toIndex + 1] - car.route[toIndex];
			float angle =  (vec1 * (1 - ratio) + vec2 * ratio).asAngle();
			
			m_renderer->blit(*carTexture, car.unit.location, angle, 0.8);
			auto gunLocation = Point::fromAngle(angle, 9);
			gunLocation += car.unit.location;
			m_renderer->blit(*carGunTexture, gunLocation, angle, 0.8);
			
			//fire
			if (car.unit.health < m_prototypes->cars[car.unit.prototypeId].maxHealth * 0.6)
			{
				auto blendMode = m_renderer->getBlendMode();
				m_renderer->setBlendMode(GPU_BlendPresetEnum::GPU_BLEND_ADD);
				Point offset;
				SeededRandom rnd(FMath::q_sdbm(car.unit.id));
				offset.x = rnd.get(-0.2f, 0.2f) * carTexture->rect.w;
				offset.y = rnd.get(-0.2f, 0.2f) * carTexture->rect.h;
				offset.rotate(angle, offset);
				drawFire(car.unit.location + offset, angle, rnd, m_state->time.time);
				
				if (car.unit.health < m_prototypes->cars[car.unit.prototypeId].maxHealth * 0.3)
				{
					offset.x = rnd.get(-0.2f, 0.2f) * carTexture->rect.w;
					offset.y = rnd.get(-0.2f, 0.2f) * carTexture->rect.h;
					offset.rotate(angle, offset);
					drawFire(car.unit.location + offset, angle, rnd, m_state->time.time);
				}
				m_renderer->setBlendMode(blendMode);
			}
		}
	}
}

void GameView::drawFire(const Point& location, float rotation, SeededRandom& rnd, int32_t time)
{
	float miniFlamesIntensity = 0;
	for(size_t i = 0; i < 5; i++)
	{
		SeededRandom rndFlame(rnd.getSeed() * (i + 100));
		float intensity = FMath::fluctuation({3000.f + rndFlame.get(0, 1000), 1700, 1300}, time) - 0.5;
		if (intensity > 0)
		{
			miniFlamesIntensity += intensity;
			Point offset;
			offset.x = rndFlame.get(-1.f, 1.f) * 6;
			offset.y = rndFlame.get(-1.f, 1.f) * 6;
			offset.rotate(rotation, offset);
			auto flameTint = ViewUtil::colorFromHex(0xffffff, FMath::lerp_ui8(0, 0, 0.5, 0xbb, intensity));
			auto flameScale = FMath::lerp(0, 0.2, 0.5, 0.4, intensity);
			m_renderer->blit(S::textures.td.fire1, location + offset, -M_PI_2, flameScale, flameTint);
		}
	}
	
	miniFlamesIntensity = std::min(1.f, miniFlamesIntensity);
	
	{
		const int32_t fps = 15;
		auto& fireFrames = S::sequences.fire.frames;
		auto frameIndex = (time * fps / 1000 + rnd.get(0, fireFrames.size())) % fireFrames.size();
		auto& frame = fireFrames[frameIndex];
		
		auto alpha = FMath::lerp_ui8(0, 0xff, 1, 0x55, FMath::fluctuation({5000, 1500, 580}, time));
		alpha = FMath::lerp_ui8(0, alpha, 1, alpha / 2, miniFlamesIntensity);
		auto scale = FMath::lerp(0, 0.4, 1, 0.6, FMath::fluctuation({7000, 1900}, time));
		
		auto tint = ViewUtil::colorFromHex(0xffffff, alpha);
	
		m_renderer->blit(frame, location, -M_PI_2, scale, tint);
	}
}

void GameView::drawCreeps()
{
	m_creeps.render(
			m_state->creeps.begin(), 
			m_state->creeps.end(),
			m_state->timeStamp, 
			[this](CreepState& creep, CreepView& view){
				view.render(m_renderer, creep, m_state, m_prototypes, m_login);
			}
		);
}


void GameView::drawProjectiles()
{
	SDL_Color color = ViewUtil::colorFromHex(0x333333);
	int32_t rectSize = 2;
	
	for (ProjectileState &projectile : m_state->projectiles)
	{
		TextureDef* texture = m_prototypes->weapons[projectile.weapon].projectileTexture[projectile.force];
		if (!texture)
		{
			GPU_RectangleFilled(
				m_screen,
				projectile.location.x - rectSize/2,
				projectile.location.y - rectSize/2,
				projectile.location.x + rectSize/2,
				projectile.location.y + rectSize/2,
				color
			);
		}
		else
		{
			auto barrel = m_prototypes->weapons[projectile.prototypeId].barrelSize;
			if (m_state->time.time - projectile.spawnedAt < barrel / projectile.speed * 1000)
				continue; //Shell is inside the barreltexture
			m_renderer->blit(*texture, projectile.location, (projectile.target - projectile.location).asAngle(), 0.7);
		}
	}
}

void GameView::drawFormationConnections()
{
	SDL_Color color = ViewUtil::colorFromHex(0xff00ff);
	
	for (auto& form : m_state->formations)
	{
		FormationProto& proto = m_prototypes->formations[form.prototypeId];
		
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


void GameView::drawFormationAgro()
{
	for (auto& form : m_state->formations)
	{
		Point location;
		int32_t totalCreeps = 0;
		float hostileThreat = 0;
		int32_t mapIndex = form.force == 0 ? 1 : 0;
		
		for (size_t i = 0; i < form.slots.size(); i++)
		{
			auto& slot = form.slots[i];
			
			if (slot > 0)
			{
				auto creep = std::find_if(m_state->creeps.begin(), m_state->creeps.end(), [slot](CreepState& creep){ return creep.unit.id == slot; });
				if (creep != m_state->creeps.end())
				{
					location+=creep->unit.location;
					hostileThreat += m_state->threatMap_[mapIndex].getThreatAt(creep->unit.location);
					totalCreeps++;
				}
			}
		}
		
		location /= totalCreeps;
		hostileThreat /= totalCreeps;
		
		int32_t percent = 100 * form.carAgro / m_prototypes->variables.carAgroThresholdPerSlot / totalCreeps;
		std::string state = "";
		switch (form.subObjective)
		{
			case SUB_OBJECTIVE::MOVE: {state = "M"; break;}
			case SUB_OBJECTIVE::PURSUE: {state = "P"; break;}
			case SUB_OBJECTIVE::NONE: {state = "N"; break;}
			case SUB_OBJECTIVE::ASSAULT: {state = "A"; break;}
			case SUB_OBJECTIVE::RETREAT: {state = "R"; if (form.targetFormationId != -1) state += " merging"; break;}
			default: {state = "?"; break;}
		}
		S::fonts.fontDebug.draw(m_screen, location.x, location.y, "%s agro: %d, (%d%%)", state.c_str(), form.carAgro, (int32_t)percent);
		
		percent = 100 * hostileThreat / form.agroAt;
		S::fonts.fontDebug_m.draw(m_screen, location.x, location.y + 16, "threat: %d, (%d%%)", (int32_t)hostileThreat, (int32_t)percent);
		S::fonts.fontDebug_m.draw(m_screen, location.x, location.y + 28, "count: %d", totalCreeps);
		S::fonts.fontDebug_m.draw(m_screen, location.x, location.y + 40, "H: %.1f F: %.1f /: %.1f / %.1f", form.hostile_, form.friendly_, form.hostile_ / form.friendly_, form.bravery);
	}
}

void GameView::drawFormations()
{
	SDL_Color color = ViewUtil::colorFromHex(0xdddd00);
	
	for (auto& form : m_state->formations)
	{
		if (form.subObjective != SUB_OBJECTIVE::MOVE && form.subObjective != SUB_OBJECTIVE::RETREAT)
			continue;
			
		FormationProto& proto = m_prototypes->formations[form.prototypeId];
		
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
		int32_t padding = 2;
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
		
		Point location;
		int32_t totalCreeps = 0;
		
		for (size_t i = 0; i < form.slots.size(); i++)
		{
			int32_t slotSize = 4;
			
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
				
				auto creep = std::find_if(m_state->creeps.begin(), m_state->creeps.end(), [slot](CreepState& creep){ return creep.unit.id == slot; });
				if (creep != m_state->creeps.end())
				{
					location+=creep->unit.location;
					totalCreeps++;
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
		
	auto init = [this](ProjectileExplosionEvent& event, uint32_t seed, SerialAnimationView& view){
		view.init(seed, event, m_state, m_prototypes, m_login);
	};
	auto blendMode = m_renderer->getBlendMode();
	m_renderer->setBlendMode(GPU_BlendPresetEnum::GPU_BLEND_ADD);
	
	m_projectileExplosions.render(
			m_state->eventLogger.projectileExplosions.begin(), 
			m_state->eventLogger.projectileExplosions.end(),
			m_state->timeStamp,
			[this, &init](ProjectileExplosionEvent& event, SerialAnimationView& view){
				view.render<ProjectileExplosionEvent>(m_renderer, event, m_state->time.time, init);
			}
		);
	m_renderer->setBlendMode(blendMode);
}

void GameView::drawRemnants()
{
	if (!m_state->isEventLoggerEnabled)
		return;		
		
	auto initCreep = [this](CreepDeathEvent& event, uint32_t seed, SerialAnimationView& view){
		view.init(seed, event, m_state, m_prototypes, m_login, m_creeps.getView(event.unitDeath.unitId));
	};
	
	auto initCar = [this](CarDeathEvent& event, uint32_t seed, SerialAnimationView& view){
		view.init(seed, event, m_state, m_prototypes, m_login);
	};
	
	m_creepDeaths.render(
			m_state->eventLogger.creepDeaths.begin(), 
			m_state->eventLogger.creepDeaths.end(),
			m_state->timeStamp,
			[this, &initCreep](CreepDeathEvent& event, SerialAnimationView& view){
				view.render<CreepDeathEvent>(m_renderer, event, m_state->time.time, initCreep);
			}
			//, m_creeps.getView(event.unitDeath.unitId)
		);	
	m_carDeaths.render(
			m_state->eventLogger.carDeaths.begin(), 
			m_state->eventLogger.carDeaths.end(),
			m_state->timeStamp,
			[this, &initCar](CarDeathEvent& event, SerialAnimationView& view){
				view.render<CarDeathEvent>(m_renderer, event, m_state->time.time, initCar);
			}
		);
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
		ViewUtil::drawArrow(m_screen, arrow.from, arrow.to, arrow.color);
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
	
	for(auto& text : S::visualDebug.texts)
	{
		S::fonts.fontDebug.draw(m_screen, text.origin.x, text.origin.y, "%s", text.text.c_str());
	}
	
	auto selectedCreep = m_state->creepById_[VisualDebug::interestingId];
	if (selectedCreep)
	{
		SDL_Color color = ViewUtil::colorFromHex(0x0000ff, 0xff);
		SDL_Color color2 = ViewUtil::colorFromHex(0xffff00, 0xff);
		auto loc = selectedCreep->unit.location;
		GPU_Line(m_screen, loc.x - 5, loc.y - 5, loc.x + 5, loc.y + 5, color);
		GPU_Line(m_screen, loc.x + 5, loc.y - 5, loc.x - 5, loc.y + 5, color);
		GPU_Circle(m_screen, loc.x, loc.y, 3, color2);
		
		//m_fontDebug.draw(m_screen, loc.x, loc.y- 20, "mv: %.2f, sp: %.2f / %.2f", selectedCreep->movement_.getLength(), selectedCreep->velocity.getLength(), selectedCreep->creepProto_->speed);
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
				color.a = (int8_t)FMath::lerpClipped(0, 0, 1000, 120, (*maps[c])[i][j]);
				if (color.a > 0)
					color.a += 10;
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
	if (S::drawSettings.scores)
	{	
		std::vector<PlayerState*> players;
		players.resize(m_state->players.size());
		std::transform(m_state->players.begin(), m_state->players.end(), players.begin(), [](PlayerState& player){ return &player; });
		
		std::sort(players.begin(), players.end(), [](PlayerState* a, PlayerState* b){ return a->score > b->score;});
		int32_t i = 0;
		for(auto p : players)
		{
			if (p->wentOfflineAt > 0)
				continue;
			
			std::string text = "";
			if (!p->isHeadless)
			{
				if (p->login == m_login)
					text += "YOU";
				else
					text += "player " + std::to_string(p->login);
				if (p->isAI)
					text += " (AI)";
			}
			else
			{
				text += "AI " + std::to_string(-p->login);
			}
			text += ":  " +  std::to_string(p->score);
			
			for(auto& car : p->activeCars)
			{
				if (car.score > 0)
					text += "  +  " +  std::to_string(car.score);
			}
			
			S::fonts.fontAmaticBold.draw(m_screen, 5.f, 20.f + 30 * i, "%s", text.c_str());
			
			i++;
		}
	}
	
	auto& vars = m_prototypes->variables;
	
	if (S::drawSettings.fps_D)
		S::fonts.fontAmaticBold.draw(m_screen, 5.f, vars.fieldHeight - 50, "fps: %.1f", S::fpsMeter.getfps(500));
		
	const auto player = GameLogic::playerByLogin(m_state, m_login);
	if (player)
	{
		if (player->repairsLeft > 0)
		{
			int32_t repairs = (player->repairsTotal - player->repairsLeft) * 100 / player->repairsTotal;
			S::fonts.fontAmaticBoldBig.draw(m_screen, vars.fieldWidth / 2, 30, NFont::AlignEnum::CENTER, "Hull repairs: %d%%", repairs);
		}
		else if (player->refuelLeft > 0)
		{
			int32_t refuels = (player->refuelTotal - player->refuelLeft) * 100 / player->refuelTotal;
			S::fonts.fontAmaticBoldBig.draw(m_screen, vars.fieldWidth / 2, 30, NFont::AlignEnum::CENTER, "Refueling: %d%%", refuels);
		}		
	}
}

void GameView::drawFlyingMessages()
{
	for(auto& msg : m_flyingMessages)
		msg.render(m_state->time.time, S::fonts.fontAmaticBoldBig, m_screen);
}

void GameView::drawPhantomRoutes()
{
	auto stamp = m_state->time.time;
	for(auto& r : m_phantomRoutes)
	{
		if (stamp > r.stamp + PhantomRoute::LIFETIME)
			continue;
			
		
		float alpha = 1;
		float fadeRatio = (float)(stamp - r.stamp) / PhantomRoute::LIFETIME;
		if (fadeRatio > PhantomRoute::FADE_POINT)
			alpha = FMath::lerp(PhantomRoute::FADE_POINT, 1.f, 1.f, 0.f, fadeRatio);
		alpha = FMath::lerp(1.f, 0.f, 0.f, 0.5f, alpha);
		for (size_t i = 1; i < r.route.size(); i++)
			renderPathStep(r.route[i - 1].location, r.route[i].location, S::sequences.grayPathDash, S::sequences.grayPathDot, r.stamp, alpha);
	}
}