#include <Creeps.h>
#include <FMath.h>
#include <VisualDebug.h>
#include <std2.h>
#include <Field.h>
#include <GameUtil.h>

namespace Creeps
{
	using namespace CreepsInternal;
	
	void handleCreepSpawn(GameState* state, Prototypes* prototypes, int32_t timePassed)
	{
		preprocessCreeps(state, prototypes);
		runDebugAsserts(state, prototypes);
		spawnFormations(state, prototypes);
		updateCache(state, prototypes);		
	}
	
	void handleCreepUpdate(GameState* state, Prototypes* prototypes, int32_t timePassed)
	{
		processFormations(state, prototypes, timePassed);
		
		processProjectiles(state, prototypes, timePassed);		
		
		processCreeps(state, prototypes, timePassed);
		pushCreepsOutOfObstacles(state, prototypes, timePassed);
		preventCreepObstacleCollision(state, prototypes);
		
		applyCreepMovement(state);		
	}
	
	void handleCreepDeath(GameState* state, Prototypes* prototypes, int32_t timePassed)
	{
		removeDeadProjectiles(state);
		removeDeadCreeps(state);
		removeDeadFormations(state);
	}
		
	Point getCurrentSlotLocation(FormationState& formation, int32_t slot)
	{
		return formation.location + formation.formationPrototype_->slots[slot].offset.rotate(formation.orientation);			
	}
	
	Point getTargetSlotLocation(FormationState& formation, int32_t slot)
	{
		return formation.targetLocation + formation.formationPrototype_->slots[slot].offset.rotate(formation.targetOrientation);			
	}
	
	
	namespace CreepsInternal
	{
		
		void preprocessCreeps(GameState* state, Prototypes* prototypes)
		{
			state->forceStrength_.clear();
			state->forceStrength_.resize(prototypes->forces.size());
				
			for(CreepState& creep : state->creeps)
			{
				if (creep.creepProto_ == nullptr)
				{
					THROW_FATAL_ERROR("Prototypes are not resolved");
				}
				state->forceStrength_[creep.unit.force] += creep.creepProto_->strength;
			}
		}
	
		void runDebugAsserts(GameState* state, Prototypes* prototypes)
		{
			for(auto& creep : state->creeps)
			{
				auto& mapCreep = state->creepById_[creep.unit.id];
				if (mapCreep != &creep)
					THROW_FATAL_ERROR("creep pointer mismatch");
				if (mapCreep->unit.id != creep.unit.id)
					THROW_FATAL_ERROR("creep pointer mismatch");
			}
			
			for(auto& creep : state->creeps)
				if (creep.creepProto_ != &prototypes->creeps[creep.unit.prototypeId])
					THROW_FATAL_ERROR("frefer");
					
			for(auto& form : state->formations)
			{	
				for(size_t i = 0; i < form.slots.size(); i++)
				{
					for(size_t j = i + 1; j < form.slots.size(); j++)
					{
						if (form.slots[i] == form.slots[j] && form.slots[i] != -1)
							THROW_FATAL_ERROR("same creeps in different slots");
					}
				}
			}
		}
		
		void spawnFormations(GameState* state, Prototypes* prototypes)
		{
			for(auto& force : prototypes->forces)
			{
				if (state->formations.size() == FormationState::MAX_FORMATION_COUNT)
					break;
				if (!force.alwaysOn)
					continue;			
				if (state->forceStrength_[force.id] < prototypes->variables.intensity)
				{
					FormationProto formation;
					do 
						formation = state->random.getFromVector(prototypes->formations);	
					while(!formation.enabled || std::find(formation.forces.begin(), formation.forces.end(), force.id) == formation.forces.end());
					if (state->creeps.size() + formation.slots.size() < GameState::MAX_CREEPS)
					{
						spawnFormation(state, prototypes, force, prototypes->formations[formation.id]);
						state->forceStrength_[force.id] += formation.strength;
					}
				}
			}
		}
		
		void updateCache(GameState* state, Prototypes* prototypes)
		{
			state->threatMap_[0].reset();
			state->threatMap_[1].reset();
			
			for(auto& creep : state->creeps)
				creep.movement_.scaleTo(0);
			
			if (!state->creepMap_.isValid())
			{
				Point BB(prototypes->variables.fieldWidth + 100, prototypes->variables.fieldHeight + 100);
				state->creepMap_ = SpatialMap<CreepState>(50, true, Point(-100, -100), BB);
			}
			state->creepMap_.reset(state->creeps.size() * 2);
			state->creepMap_.addUnique(state->creeps);
			
			if (!state->threatMap_[0].isValid())
			{
				Point BB(prototypes->variables.fieldWidth, prototypes->variables.fieldHeight);
				state->threatMap_[0] = ThreatMap(120, Point(), BB);
				state->threatMap_[1] = ThreatMap(120, Point(), BB);
			}
			
			for(auto& creep : state->creeps)
			{
				state->threatMap_[creep.unit.force].addThreat(creep.unit.location, creep.creepProto_->strength);
			}
			state->threatMap_[0].blur(0.8);
			state->threatMap_[0].blur(0.7);
			state->threatMap_[0].blur(0.6);
			
			state->threatMap_[1].blur(0.8);
			state->threatMap_[1].blur(0.7);
			state->threatMap_[1].blur(0.6);
		}
		
		void spawnFormation(GameState* state, Prototypes* prototypes, ForceProto& forceProto, FormationProto& formationProto)
		{
			Point formationCenter(
				state->random.get(forceProto.spawnAA.x, forceProto.spawnBB.x),
				state->random.get(forceProto.spawnAA.y, forceProto.spawnBB.y)
			);
			
			auto& var = prototypes->variables;
			
			float angle = var.fieldCenter.subtract(formationCenter).asAngle();
			state->formations.emplace_back();
			FormationState& formation = state->formations.back();
			formation.objectiveID = -1;
			formation.id = state->idCounter++;
			formation.prototypeId = formationProto.id;
			formation.force = forceProto.id;
			formation.formationPrototype_ = &formationProto;
			formation.isDisposed_ = false;
			formation.orientation = angle;
			formation.location = formationCenter;
			formation.speed = FMath::F_MAX;
			formation.angularSpeed = 0;
			formation.spawnedAt = state->time.time;
			formation.agroAt = state->random.get(0.7f, 1.4f) * formationProto.relativeAgroability * var.baseAgroLevel;
			formation.bravery = state->random.get(var.formationMinBravery, var.formationMaxBravery);
			formation.carAgro = 0;
			formation.subObjective = SUB_OBJECTIVE::NONE;
			
			for(auto& slot : formationProto.slots)
			{
				if (!slot.optional)
				{
					
					CreepState& creep = spawnCreep(slot.creepType, getCurrentSlotLocation(formation, formation.slots.size()), state, prototypes);
					creep.unit.force = forceProto.id;
					creep.formationId = formation.id;
					creep.formationsSlot = formation.slots.size();
					creep.orientation = formation.orientation;
					formation.slots.push_back(creep.unit.id);
					formation.speed = std::min(formation.speed, creep.creepProto_->speed);
				}
				else
				{
					formation.slots.push_back(-1);
				}				
			}
		}		
		
		CreepState& spawnCreep(int16_t type, const Point& location, GameState* state, Prototypes* prototypes)
		{
			state->creeps.emplace_back();
			CreepState& creep = state->creeps.back();
			creep.creepProto_ = &(prototypes->creeps[type]);
			creep.weaponProto_ = &(prototypes->weapons[creep.creepProto_->weapon]);
			
			creep.numWhiskers = creep.creepProto_->whiskers + 1;
			for(int8_t i = 0; i < creep.numWhiskers; i++)
			{
				creep.whiskers[i] = state->random.get((i + 1) * 0.7f, (i + 1) * 1.3f) / creep.numWhiskers * creep.creepProto_->maxWhiskerLength;
				if (i > 0 && creep.whiskers[i] - creep.whiskers[i - 1] < 10)
					creep.whiskers[i] = creep.whiskers[i - 1] + 10;
			}
			for(int8_t i = creep.numWhiskers; i < CreepState::MAX_WHISKER_COUNT; i++)
			{
				creep.whiskers[i] = -1;
			}
			
			creep.unit.prototypeId = type;
			creep.unit.id = state->idCounter++;
			state->creepById_[creep.unit.id] = &creep;
			
			creep.unit.health = creep.creepProto_->maxHealth;
			creep.unit.location = location;
			
			creep.weapon.prototypeId = creep.creepProto_->weapon;
			creep.weapon.attackCooldown = 0;
			creep.weapon.target.id = -1;
			
			creep.formationId = -1;
			creep.formationsSlot = -1;
			
			creep.sensedAnObstacle = false;
			
			return creep;
		}
		
		void processFormations(GameState* state, Prototypes* prototypes, int32_t timePassed)
		{			
			for(auto& formation : state->formations)
			{
				switch (formation.subObjective)
				{
					case SUB_OBJECTIVE::NONE :
					{
						findObjective(formation, state, prototypes);
						break;
					}
					case SUB_OBJECTIVE::MOVE :
					{
						moveFormation(formation, state, prototypes, timePassed);
						break;
					}
					case SUB_OBJECTIVE::ASSAULT :
					{
						performAssault(formation, state, prototypes, timePassed);
						break;
					}					
					case SUB_OBJECTIVE::PURSUE :
					{
						performPursue(formation, state, prototypes, timePassed);
						break;
					}					
					case SUB_OBJECTIVE::RETREAT :
					{
						moveFormation(formation, state, prototypes, timePassed);
						break;
					}
					default:
					{
						THROW_FATAL_ERROR("subObjective not handled.");
						break;
					}
				}
			}
		}
		
		void findObjective(FormationState& formation, GameState* state, Prototypes* prototypes)
		{
			if (formation.formationPrototype_->type == FORMATION_TYPE::ASSAULT)
			{
				if (formation.objectiveID == -1)
				{
					if (formation.subObjective == SUB_OBJECTIVE::NONE)
						formation.subObjective = SUB_OBJECTIVE::MOVE;
					setObjective(formation, formation.subObjective, state, prototypes);
				}
				else
				{
					formation.subObjective = SUB_OBJECTIVE::ASSAULT;
					// or retreat / regroup if no enemies ...
				}				
			}
			
			formation.bravery = state->random.get(prototypes->variables.formationMinBravery, prototypes->variables.formationMaxBravery);
		}
		
		void setObjective(FormationState& formation, SUB_OBJECTIVE type, GameState* state, Prototypes* prototypes)
		{
			std::vector<ObjectiveProto>* protos;
			switch (type)
			{
				case SUB_OBJECTIVE::MOVE: { protos = &prototypes->moveObjectives[formation.force]; break; }
				case SUB_OBJECTIVE::RETREAT: { protos = &prototypes->retreatObjectives[formation.force]; break; }				
				default:
					THROW_FATAL_ERROR("unhandled objective type");
			}
			
			ObjectiveProto& objective = state->random.getFromVector(*protos);
			
			formation.objectiveID = objective.id;
			formation.targetLocation = objective.location;
			formation.targetOrientation = prototypes->variables.fieldCenter.subtract(objective.location).asAngle();
			
			formation.subObjective = type;
			compactFormation(formation, state);
		}
		
		void moveFormation(FormationState& formation, GameState* state, Prototypes* prototypes, int32_t timePassed)
		{
			tryOptimiseFormation(formation, state, timePassed);
			
			bool formationIsInFinalPhase = formation.location.distanceTo(formation.targetLocation) < 20;
				
			float speedModifier = formation.subObjective == SUB_OBJECTIVE::RETREAT ? prototypes->variables.retreatSpeedModifier : 1;
			
			float avgMoveBalance = 0;
			float maxMoveImbalance = 0;
			int32_t creepMoveBalanceCount = 1;
			
			int32_t targetInObstacleCount = 0;
			
			float tractorMoveBalanceFactor = 0;
			int32_t tractorCreepsCount = 0;
			float avgOrientationImbalance = 0;
			float totalStrength = 0;
			float hostileThreat = 0;
			int32_t totalCreeps = 0;
			int32_t mapIndex = formation.force == 0 ? 1 : 0;
			
			formation.actualLocation_ = Point();
			
			
			for(auto& id : formation.slots)
			{
				if (id < 0)
					continue;
					
				CreepState* creepP = state->creepById_[id];
				CreepState& creep = *creepP;
				if (!creepP || creepP->unit.health <= 0)
					continue;
				
				creep.weapon.target.id = -1;
					
				formation.actualLocation_ += creep.unit.location * creep.creepProto_->strength;
				totalStrength += creep.creepProto_->strength;
				totalCreeps++;
				hostileThreat += state->threatMap_[mapIndex].getThreatAt(creep.unit.location);
					
				Point formationSlotLocation = getCurrentSlotLocation(formation, creep.formationsSlot);
				Point targetSlotLocation = getTargetSlotLocation(formation, creep.formationsSlot);
				Point creep2Target = targetSlotLocation - creep.unit.location;
				Point creep2Slot = formationSlotLocation - creep.unit.location;
				
				Obstacle* obstacle = Field::findObstacle(formationSlotLocation, prototypes);
				
				Point* target;
				if (!obstacle)
				{
					creepMoveBalanceCount++;
					float creep2TargetL = creep2Target.getLength();
					float creep2SlotL = creep2Slot.getLength();
					
					if (creep2TargetL < creep2SlotL)
					{
						avgMoveBalance += creep2SlotL - creep2TargetL;
						target = &targetSlotLocation;
					}
					else
					{
						avgMoveBalance -= creep2SlotL;
						if (creep.creepProto_->moveType == MOVE_TYPE::TRACTOR)
						{
							tractorMoveBalanceFactor += creep.creepProto_->size / 5;
							tractorCreepsCount++;
							
							avgOrientationImbalance += std::fabs(FMath::angleDelta(creep.orientation, formation.targetOrientation));
							if (creep2TargetL < 1 && creep.velocity.getLength() == 0)
								changeTractorOrientation(creep, formation.targetOrientation, timePassed);
						}
						maxMoveImbalance = std::max(maxMoveImbalance, creep2SlotL);
						target = &formationSlotLocation;
					}							
				}
				else
				{
					target = &targetSlotLocation;
					targetInObstacleCount++;
				}
				
				int32_t partnerCount = 0;
				float targetPriority = creep.sensedAnObstacle || creep.creepProto_->moveType == MOVE_TYPE::TRACTOR ? 15 : 2;
				target->scaleBy(targetPriority);
				if (!formationIsInFinalPhase)
				{
					for(auto& connection : formation.formationPrototype_->slots[creep.formationsSlot].connections)
					{
						CreepState* partner = state->creepById_[formation.slots[connection.slot]];
						if (partner && partner->unit.health > 0)
						{
							partnerCount++;
							*target += partner->unit.location + connection.offset.rotate(formation.orientation);
						}
					}
				}
				target->scaleBy(1.f/(targetPriority + partnerCount));
				creep.formationAttraction_ = *target;
				moveCreepTowardsPoint(creep, *target, prototypes, timePassed, speedModifier);
				
				if (formation.subObjective != SUB_OBJECTIVE::RETREAT)
				{
					if (creep.weapon.attackCooldown <= 0)
					{
						auto cars = state->carMap_.getInRadius(creep.unit.location, creep.weaponProto_->range * 1.5);
						if (cars.size() > 0)
						{
							auto interceptLoc = FMath::intercept(
									creep.unit.location, 
									cars[0]->unit.location, 
									cars[0]->unit.voluntaryMovement / timePassed, 
									creep.weaponProto_->bulletSpeed / 1000.f
								);
							Point nearestInterception;
							if (!interceptLoc[0].isNaN())
							{
								if (interceptLoc[1].isNaN())
									nearestInterception = interceptLoc[0];
								else if ((creep.unit.location - interceptLoc[0]).getLength() < (creep.unit.location - interceptLoc[2]).getLength())
									nearestInterception = interceptLoc[0];
								else
									nearestInterception = interceptLoc[1];
							}
							else
							{
								nearestInterception = interceptLoc[1];
							}
							
							if (!nearestInterception.isNaN() && (nearestInterception - creep.unit.location).getLength() < creep.weaponProto_->range)		
								performCreepAttack(creep, cars[0]->unit, state, nearestInterception - cars[0]->unit.location);
						}
					}					
				}
			}
			
			if (creepMoveBalanceCount > 0)
				avgMoveBalance /= creepMoveBalanceCount;
			if (tractorCreepsCount > 0)
			{
				tractorMoveBalanceFactor /= tractorCreepsCount;
				avgOrientationImbalance /= tractorCreepsCount;
			}
			formation.actualLocation_ /= totalStrength;
			
			
			Point formationDirection;
			formationDirection.setFromAngle(formation.orientation);
			auto cells = prototypes->obstacleMap.getCellsInRadius(formation.location, formation.formationPrototype_->width * 0.35, 0.2);
			int32_t obstaclesAhead = 0;
			for(auto& cell : cells)
			{
				if (cell.second->size() > 0 && ((cell.first - formation.location) * formationDirection) > 0 )
				{
					obstaclesAhead++;
				}
			}
			
			float speedMulti = FMath::lerp(0, 0.8f + obstaclesAhead, -20 - obstaclesAhead, 0.7f, avgMoveBalance);
			speedMulti = std::max(0.1f, speedMulti);
			speedMulti *= targetInObstacleCount + 1;
			speedMulti = std::min(8.f, speedMulti);
			
			Point formation2Target = formation.targetLocation - formation.location;
			float desiredOrientation;
			if (formation2Target.getLength() > 30)
				desiredOrientation = formation2Target.asAngle();
			else
				desiredOrientation = formation.targetOrientation;
			float aDelta = FMath::angleDelta(formation.orientation, desiredOrientation);
			float speedRatio = std::fabs(aDelta) > 0.05 ? 0.5 : 1;
			
			float stepSize = timePassed * formation.speed * speedMulti * speedRatio * speedModifier * speedModifier;			
			
			bool formationMovementComplete = true;
			if (formation2Target.getLength() > stepSize)
			{
				formation2Target.scaleTo(stepSize);
				formation.location += formation2Target;
				formationMovementComplete = false;
			}
			else
			{
				speedRatio = 0;
				formation.location = formation.targetLocation;
			}
			
			float turnSize = timePassed * formation.formationPrototype_->maxAngularSpeed * (1 - speedRatio + 0.1)  * speedModifier * speedModifier;
			
			if (std::fabs(aDelta) > turnSize)
			{
				float turnDirection = aDelta > 0 ? 1 : -1;
				formation.orientation += turnDirection * turnSize;
				formationMovementComplete = false;
			}
			else
			{
				formation.orientation = desiredOrientation;
			}
			
			if (formationMovementComplete && avgMoveBalance > -1 - tractorMoveBalanceFactor && avgOrientationImbalance < 0.1)
				formation.subObjective = SUB_OBJECTIVE::NONE;
			
			hostileThreat /= totalCreeps;
			if (hostileThreat > formation.agroAt && formation.subObjective != SUB_OBJECTIVE::RETREAT)
				formation.subObjective = SUB_OBJECTIVE::ASSAULT;
			formation.agro_ = hostileThreat;
			
			if (formation.carAgro > 0)
			{
				formation.carAgro -= timePassed;
				if (formation.carAgro > prototypes->variables.carAgroThresholdPerSlot * totalCreeps)
					formation.subObjective = SUB_OBJECTIVE::PURSUE;
			}
		}
		
		void tryOptimiseFormation(FormationState& formation, GameState* state, int32_t timePassed)
		{
			size_t i = ((state->time.time - formation.spawnedAt) / timePassed) % formation.slots.size();
			
			CreepState* creep1 = state->creepById_[formation.slots[i]];
			if (!creep1 || creep1->unit.health <= 0)
				return;
				
			float creep1ToSlotDistance = creep1->unit.location.distanceTo(getCurrentSlotLocation(formation, i));
			for(size_t j = 0; j < formation.slots.size(); j++)
			{
				if (i == j)
					continue;
				CreepState* creep2 = state->creepById_[formation.slots[j]];
				if (!creep2 || creep2->unit.health <= 0 || creep1->unit.prototypeId != creep2->unit.prototypeId)
					continue;
				if (creep1ToSlotDistance > creep1->unit.location.distanceTo(getCurrentSlotLocation(formation, j)) &&
					creep1ToSlotDistance > creep2->unit.location.distanceTo(getCurrentSlotLocation(formation, i))
				)
				{
					creep1->formationsSlot = j;
					creep2->formationsSlot = i;
					formation.slots[i] = creep2->unit.id;
					formation.slots[j] = creep1->unit.id;
					return;
				}
			}
		}		
		
		void compactFormation(FormationState& formation, GameState* state)
		{
			auto& proto = *formation.formationPrototype_;
			for(size_t i = 0; i < formation.slots.size(); i++)			
			{
				CreepState* creep1 = state->creepById_[formation.slots[i]];
				
				if (!creep1)
					continue;
					
				for(size_t j = 0; j < formation.slots.size(); j++)
				{
					if (
							i == j || 
							(!proto.slots[i].optional && proto.slots[j].optional) ||
							proto.slots[i].creepType != proto.slots[j].creepType
						)
					{
						continue;
					}
					
					if (proto.slots[j].priority > proto.slots[i].priority && !state->creepById_[formation.slots[j]])
					{					
						creep1->formationsSlot = j;
						formation.slots[i] = -1;
						formation.slots[j] = creep1->unit.id;
						break;
					}
				}
			}
		}
		
		void performPursue(FormationState& formation, GameState* state, Prototypes* prototypes, int32_t timePassed)
		{
			float avgDistance = 0;
			float minDistance = std::numeric_limits<float>::max();
			int32_t totalCreeps = 1;
			
			for(auto& id : formation.slots)
			{
				if (id < 0)
					continue;
					
				CreepState* creepP = state->creepById_[id];
				CreepState& creep = *creepP;
				
				if (!creepP || creep.unit.health <= 0)
					return;
				
				totalCreeps++;
				
				auto target = chaseAndDestroy(creep, state, prototypes, timePassed, [](CreepState& creep, GameState* state, Prototypes* prototypes){
					
					CarState* bestCar;
					float bestValue = std::numeric_limits<float>::max();
					
					for(auto& player : state->players)
					{
						for(auto& car : player.activeCars)
						{
							float distance = creep.unit.location.distanceTo(car.unit.location);
							float value = distance / (1 + car.agro);
							if (value < bestValue)
							{
								bestValue = value;
								bestCar = &car;
							}
						}
					}
					
					return bestCar ? &bestCar->unit : nullptr;
				});
				
				if (target)
				{
					float distance = creep.unit.location.distanceTo(target->location);
					avgDistance += distance;
					minDistance = std::min(minDistance, distance);
				}
			}
			avgDistance /= totalCreeps;
			
			if (avgDistance + minDistance > prototypes->variables.carChaseBreakoutDistance)
			{
				formation.carAgro /= 2;
				formation.objectiveID = -1;
				formation.subObjective = SUB_OBJECTIVE::NONE;				
			}
		}
		
		Unit* chaseAndDestroy(CreepState& creep, GameState* state, Prototypes* prototypes, int32_t timePassed, 
				std::function<Unit*(CreepState&, GameState*, Prototypes*)> getTargetForCreep)
		{
			Unit* target = nullptr;
			if (creep.weapon.target.id > 0)
				target = GameUtil::resolveTarget(creep.weapon.target, state);
			if (target == nullptr || target->health <= 0)
				target = getTargetForCreep(creep, state, prototypes);
			if (target == nullptr)
			{
				creep.weapon.target.id = -1;
				return target;
			}
			else
			{
				creep.weapon.target.id = target->id;
				creep.weapon.target.type = target->host_.type;
			}			
			
			Obstacle* obstacle = Field::findObstacle(creep.unit.location, prototypes);
			if (obstacle)
			{
				moveUnitOutOfObstacle(creep, obstacle, timePassed);				
			}
			else
			{
				creep.targetLoc_ = target->location;
				//TODO take targets size into account (UnitPrototype?)
				if (target->location.distanceTo(creep.unit.location) < creep.weaponProto_->range)
				{
					performCreepAttack(creep, *target, state, Point());
				}
				else
				{
					moveCreepTowardsPoint(creep, target->location, prototypes, timePassed);				
				}
			}
			return target;
		}
		
		void performAssault(FormationState& formation, GameState* state, Prototypes* prototypes, int32_t timePassed)
		{
			float friendlyNearbyForces = 0;
			float hostileNearbyForces = 0;
			float totalCreeps = 0;
			
			int32_t resetTarget = ((state->time.time - formation.spawnedAt) / timePassed) % (100 + formation.formationPrototype_->slots.size() * 2);
			if (resetTarget % 2 != 0)
				resetTarget = -1;
			else
				resetTarget /= 2;
			
			formation.actualLocation_ = Point();
			
			for(auto& id : formation.slots)
			{
				if (id < 0)
					continue;
					
				CreepState* creepP = state->creepById_[id];
				CreepState& creep = *creepP;
				
				if (!creepP || creep.unit.health <= 0)
					return;
				//TODO optimise	threatMap_[mapIndex]
				int32_t mapIndex = creep.unit.force == 0 ? 1 : 0;
				hostileNearbyForces += state->threatMap_[mapIndex].getThreatAt(creep.unit.location);
				friendlyNearbyForces += state->threatMap_[1 - mapIndex].getThreatAt(creep.unit.location);
				totalCreeps++;
				
				formation.actualLocation_ += creep.unit.location;
				
				if (totalCreeps == resetTarget)
					creep.weapon.target.id = -1;
				
				chaseAndDestroy(creep, state, prototypes, timePassed, [](CreepState& creep, GameState* state, Prototypes* prototypes){
					auto result = getBestAssaultTargetForCreep(creep, state->creeps);
					return result ? &result->unit : nullptr;
					});
			}
			
			hostileNearbyForces /= totalCreeps;
			friendlyNearbyForces /= totalCreeps;
			formation.actualLocation_ /= totalCreeps;
			
			formation.hostile_ = hostileNearbyForces;
			formation.friendly_ = friendlyNearbyForces;
			
			if (totalCreeps >= prototypes->variables.minCreepsToRetreat && hostileNearbyForces / friendlyNearbyForces > formation.bravery)
			{
				setObjective(formation, SUB_OBJECTIVE::RETREAT, state, prototypes);
				auto nearbyCreeps = state->creepMap_.getInRadius(formation.actualLocation_, 100);
				std::vector<int32_t> nearByFormations;
				for(auto& ncreeep : nearbyCreeps)
				{
					if (
						ncreeep->unit.force == formation.force && 
						ncreeep->formationId != formation.id && 
						!std2::containsValue(nearByFormations, ncreeep->formationId)
						)
					{
						nearByFormations.push_back(ncreeep->formationId);
					}
				}
				for(auto& fid : nearByFormations)
				{
					auto nf = std::find_if(state->formations.begin(), state->formations.end(), [fid](FormationState& fs){return fs.id == fid;});
					nf->bravery--;
				}
				return;
			}
			
			if (hostileNearbyForces < formation.agroAt / 2)
			{
				Point center;
				
				for(auto& id : formation.slots)
				{
					if (id < 0)
						continue;
						
					CreepState* creepP = state->creepById_[id];
					CreepState& creep = *creepP;
					
					if (!creepP || creep.unit.health <= 0)
						continue;
					center += creep.unit.location;
				}
				center.scaleBy(1.f/totalCreeps);
				auto nearbyCreeps = state->creepMap_.getInRadius(center, 300);
				bool hasEnemies = false;
				for(auto& creep : nearbyCreeps)
				{
					if 	(creep->unit.force != formation.force)
					{
						hasEnemies = true;
						break;
					}
				}
				if (!hasEnemies)
				{
					formation.objectiveID = -1;
					formation.subObjective = SUB_OBJECTIVE::NONE;
				}
			}
		}
		
		void performCreepAttack(CreepState& creep, Unit& target, GameState* state, const Point& leading)
		{
			if (creep.weapon.attackCooldown <= 0)
			{
				creep.weapon.attackCooldown = 1000000 / creep.weaponProto_->attackSpeed;
				spawnProjectile(creep.unit.location, target.location + leading, creep.weaponProto_, creep.unit.force, state);
			}
		}
		
		void moveCreepTowardsPoint(CreepState& creep, Point& target, Prototypes* prototypes, int32_t timePassed, float speedModifier)
		{
			if (creep.creepProto_->moveType == MOVE_TYPE::TRACTOR)
				moveTractorTowardsPoint(creep, target, prototypes, timePassed, speedModifier);
			else if (creep.creepProto_->moveType == MOVE_TYPE::WALKER)
				moveWalkerTowardsPoint(creep, target, prototypes, timePassed, speedModifier);
		}
		
		void moveTractorTowardsPoint(CreepState& creep, Point& target, Prototypes* prototypes, int32_t timePassed, float speedModifier)
		{
			auto& proto = creep.creepProto_;
			float breakAccel = -creep.creepProto_->breaksStrength * speedModifier;
			float speed = creep.velocity.getLength();
			float omniSpeed = creep.creepProto_->omniDirectionalSpeed;
			
			Point creep2Target = target - creep.unit.location;
			float testStep = creep.velocity.projectOnto(creep2Target) + omniSpeed;
			Point nextTarget = avoidObstacles(creep, std::min(testStep, creep2Target.getLength()), target, prototypes);
			creep2Target = nextTarget - creep.unit.location;
			
			float timeToLoseSpeed = -speed / breakAccel;
			float breakDistance = speed * timeToLoseSpeed / 2 + omniSpeed * timeToLoseSpeed; 
			if (nextTarget == target && creep2Target.getLength() < breakDistance * 1.05 + 1)
			{
				Point omniDirectionalStep = creep2Target;
				if (omniDirectionalStep.getLength() > 0)
					omniDirectionalStep.scaleTo(std::min(creep2Target.getLength(), omniSpeed * timePassed));
					
				accelerateTractor(creep, breakAccel * timePassed);
				creep.movement_ = creep.velocity + omniDirectionalStep;
				creep.unit.voluntaryMovement = creep.velocity;
				return;
			}
			
			// Move
			bool targetIsUnreachableAtCurrentSpeed;			
			float angularSpeed = creep.creepProto_->angularSpeed;
			
				
			if (speed == 0)
			{
				targetIsUnreachableAtCurrentSpeed = false;				
			}
			else
			{
				float turnRadiusLength = speed / angularSpeed;
				//turnRadiusLength -= omniSpeed * nextTarget.distanceTo(creep.unit.location);
				Point turnRadius = Point::fromAngle(creep.orientation, turnRadiusLength);
				Point unreachableCenter1 = creep.unit.location + turnRadius.rotate(M_PI_2);
				Point unreachableCenter2 = creep.unit.location + turnRadius.rotate(-M_PI_2);
				
				float distance1 = unreachableCenter1.distanceTo(nextTarget);
				float distance2 = unreachableCenter2.distanceTo(nextTarget);
				targetIsUnreachableAtCurrentSpeed = distance1 < turnRadiusLength || distance2 < turnRadiusLength;
			}
			
			float maxValidTurnRadius = creep2Target.getLength() / 2 / creep2Target.cos(creep.velocity.rotate(M_PI_2)); //TODO / 0			
			maxValidTurnRadius = std::fabs(maxValidTurnRadius);
			
			float maxSpeed = angularSpeed * maxValidTurnRadius;
			
			float desiredDirection = creep2Target.asAngle();
			changeTractorOrientation(creep, desiredDirection, timePassed);				
			
			if (targetIsUnreachableAtCurrentSpeed)
			{
				accelerateTractor(creep, breakAccel * timePassed);
			}
			else
			{
				float accel = timePassed * FMath::lerpClipped(0, proto->acceleration.x, proto->speed * speedModifier, proto->acceleration.y, speed);
				
				if (accel * 1.05 > maxSpeed - speed)
					accel = (maxSpeed - speed) / 1.05;
				
				accelerateTractor(creep, accel);
			}
			
			Point omniDirectionalStep = creep2Target;
			if (creep2Target.getLength() > 0)
				omniDirectionalStep.scaleTo(std::min(creep2Target.getLength(), omniSpeed * timePassed));
				
			creep.movement_ = creep.velocity + omniDirectionalStep;
			creep.unit.voluntaryMovement = creep.velocity;
		}
		
		void accelerateTractor(CreepState& creep, float amount)
		{
			if (amount > 0)
			{
				creep.velocity += Point(amount, 0).rotate(creep.orientation);
			}
			else
			{
				Point breakForce = Point(amount, 0).rotate(creep.orientation);
				
				if (breakForce.getLength() > creep.velocity.getLength())
				{
					creep.velocity.x = 0;
					creep.velocity.y = 0;
				}
				else
				{
					creep.velocity += breakForce;
				}
				
			}
		}
		
		
		void moveWalkerTowardsPoint(CreepState& creep, Point& target, Prototypes* prototypes, int32_t timePassed, float speedModifier)
		{
			float stepSize = creep.creepProto_->speed * timePassed * speedModifier;
			Point nextTarget = avoidObstacles(creep, stepSize, target, prototypes);
			creep.sensedAnObstacle = nextTarget != target;
			
			Point finalStep = nextTarget - creep.unit.location;
			if (finalStep.getLength() > 0)
				finalStep.scaleTo(std::min(stepSize, finalStep.getLength()));	
				
			creep.movement_ = finalStep;
			creep.unit.voluntaryMovement = finalStep;
		}
		
		Point avoidObstacles(CreepState& creep, float stepSize, Point& target, Prototypes* prototypes)
		{
			Point step = target - creep.unit.location;
			if (step.getLength() == 0)
				return target;
						
			for(int32_t i = -2; i < (int32_t)creep.whiskers.size(); i++)
			{
				float whisker;
				if (i == -2)
					whisker = stepSize;
				else if (i == -1)
					whisker = stepSize * 1.1;
				else if (creep.whiskers[i] > 0)
					whisker = creep.whiskers[i];
					
				if (whisker <= 0 || whisker > (target - creep.unit.location).getLength())
					break;
					
				Point whiskerStep = step;
					whiskerStep.scaleTo(whisker);
				Point nextLoc = creep.unit.location + whiskerStep;
				Obstacle* obstacle = Field::findObstacle(nextLoc, prototypes);
				if (obstacle)
				{
					Edge* bestEdge = Field::nearestIntersectedEdge(creep.unit.location, target, obstacle->edges, obstacle->extendedEdges);
					
					if (!bestEdge)
						return target; // Can happen because of rounding errors
					
					Point creepToTarget = target - creep.unit.location;
					Point creepToCentroid = obstacle->centroid - creep.unit.location;
					Point creepToP1 = *bestEdge->p1 - creep.unit.location;
					
					float creepSide = creepToCentroid.crossProduct(creepToTarget);
					float p1Side = creepToCentroid.crossProduct(creepToP1);
					
					if ((creepSide > 0) == (p1Side > 0))
						return *bestEdge->p1;
					else
						return *bestEdge->p2;
				}
			}
			return target;
		}
		
		void moveUnitOutOfObstacle(CreepState& creep, Obstacle* obstacle, int32_t timePassed)
		{
			auto nearestEdge = std2::minElement(obstacle->edges, 
					[from = creep.unit.location](Edge& edge){return FMath::distanceToLine(from, *edge.p1, *edge.p2);});
					
			Point step = (*nearestEdge->p2 - *nearestEdge->p1).rotate(M_PI_2);
			step.scaleTo(creep.creepProto_->speed * timePassed);
			creep.movement_ = step;
		}
		
		
		void changeTractorOrientation(CreepState& creep, float targetOrientation, int32_t timePassed)
		{
			if (creep.creepProto_->moveType != MOVE_TYPE::TRACTOR)
				return;				
			
			float delta = FMath::angleDelta(targetOrientation, creep.orientation);
			float directionStep = creep.creepProto_->angularSpeed * timePassed;
			if (std::fabs(delta) < directionStep)
			{
				creep.orientation = targetOrientation;
				creep.velocity = Point::fromAngle(targetOrientation, creep.velocity.getLength());
			}
			else
			{
				if (delta > 0)
					directionStep *= -1;
				creep.orientation += directionStep;
				creep.velocity = creep.velocity.rotate(directionStep);
			}
		}
			
		
		CreepState* getBestAssaultTargetForCreep(CreepState& creep, std::vector<CreepState>& creeps)
		{
			CreepState* bestCreep = nullptr;
			float bestDistance = std::numeric_limits<float>::max();
			float distance;
			
			for(CreepState& enemy : creeps)
			{
				if (enemy.unit.force != creep.unit.force && enemy.unit.health > 0)
				{
					distance = enemy.unit.location.distanceTo(creep.unit.location);
					if (distance < bestDistance)
					{
						bestDistance = enemy.unit.location.distanceTo(creep.unit.location);
						bestCreep = &enemy;
					}
				}
			}
			
			return bestCreep;
		}
		
		void preventCreepObstacleCollision(GameState* state, Prototypes* prototypes)
		{
			for(auto& creep : state->creeps)
			{
				Point nextLoc = creep.unit.location + creep.movement_;
				Obstacle* obstacle = Field::findObstacle(nextLoc, prototypes);
				
				if (obstacle)
				{
					Edge* bestEdge = Field::nearestIntersectedEdge(creep.unit.location, nextLoc, obstacle->edges, obstacle->edges);
					
					if (bestEdge == nullptr)
						continue;
					Point* target;
					float val1 = (*bestEdge->p1 - creep.unit.location).cos(creep.movement_);
					float val2 = (*bestEdge->p2 - creep.unit.location).cos(creep.movement_);
					if (val1 > val2) //Choose the point that makes the smaller angle: _\_ vs _/_
						target = bestEdge->p1;
					else
						target = bestEdge->p2;
					
					float length = creep.movement_.getLength();
					creep.movement_ = *target - creep.unit.location;
					if (creep.movement_.getLength())
						creep.movement_.scaleTo(length / 2);
					
				}				
			}
		}	
		
		void processCreeps(GameState* state, Prototypes* prototypes, int32_t timePassed)
		{
			for(auto& creep : state->creeps)
			{
				creep.weapon.attackCooldown -= timePassed;			
				
				
				
				//Collisions
				std::vector<CreepState*> possibleCollisions = state->creepMap_.getInRadius(
						creep.unit.location, creep.creepProto_->size + prototypes->variables.maxCreepSize + prototypes->variables.additionalSpacing);
				
				for(auto& creep2 : possibleCollisions)
				{
					if (creep2 >= &creep)
						continue;
						
					int32_t collisionRadius = creep.creepProto_->size + creep2->creepProto_->size + prototypes->variables.additionalSpacing;
					Point creep2Creep = creep.unit.location - creep2->unit.location;
					if (creep2Creep.getLength() == 0)
						creep2Creep.x = 0.1;
					float penetration = (collisionRadius - creep2Creep.getLength()) / 2;
					if (penetration > 0)
					{
						//float direction = creep2Creep.asAngle();
						int32_t totalWeight = creep.creepProto_->weight + creep2->creepProto_->weight;
						float force = penetration * prototypes->variables.creepRestitution / totalWeight;
						
						if (creep.unit.force == creep2->unit.force && creep.unit.prototypeId != creep2->unit.prototypeId)
							force /= 10;
						
						creep2Creep.scaleTo(force * creep2->creepProto_->weight);
						creep.movement_ += creep2Creep;
						
						creep2Creep.scaleTo(force * creep.creepProto_->weight);
						creep2->movement_ -= creep2Creep;
						
					}
				}
			}
			
		}
		
		void pushCreepsOutOfObstacles(GameState* state, Prototypes* prototypes, int32_t timePassed)
		{
			for(auto& creep : state->creeps)
			{
				if (creep.unit.health <= 0)
					continue;
				
				auto obstacle = Field::findObstacle(creep.unit.location, prototypes);
				
				if (!obstacle)
					continue;
				
				Point force = creep.unit.location - obstacle->centroid;
				force.scaleTo(0.1 * timePassed);
				creep.movement_ += force;
			}
		}
		
		void applyCreepMovement(GameState* state)
		{
			for(auto& creep : state->creeps)
				creep.unit.location += creep.movement_;		
		}
		
		
		void spawnProjectile(const Point& from, const Point& to, const WeaponProto* prototype, int16_t force, GameState* state)
		{
			state->projectiles.emplace_back();
			Projectile& projectile = state->projectiles.back();
			
			auto distance = from.distanceTo(to);
					
			projectile.id = state->idCounter++;
			projectile.prototypeId = prototype->id;
			projectile.speed = prototype->bulletSpeed;
			projectile.splash = prototype->splash;
			projectile.damage = prototype->damage;
			projectile.target = to;
			projectile.location = from;
			projectile.force = force;
			projectile.weapon = prototype->id;
			projectile.spawnedAt = state->time.time;
			
			projectile.target.x += state->random.get(-1.0f, 1.0f) * distance * 0.1;
			projectile.target.y += state->random.get(-1.0f, 1.0f) * distance * 0.1;
		}
		
		void processProjectiles(GameState* state, Prototypes* prototypes, int32_t timePassed)
		{
			for(auto& proj : state->projectiles)
			{
				auto direction = proj.location - proj.target;
				float step = timePassed * proj.speed / 1000.0;
				
				if (direction.getLength() > step)
				{
					direction.scaleTo(-step);
					proj.location += direction;
				}
				else
				{
					direction.scaleBy(-1);
					std::vector<CreepState*> localCreeps = state->creepMap_.getInRadius(proj.target, proj.splash + prototypes->variables.maxCreepSize);
					for(auto& creep : localCreeps)
					{
						int32_t r = proj.splash + creep->creepProto_->size;
						if (creep->unit.location.sqDistanceTo(proj.target) < r*r)
						{
							creep->unit.health -= proj.damage;
							if (creep->unit.health <= 0)
							{
								if (proj.force != creep->unit.force)
									creep->impact_ = direction;
								else
									creep->impact_ = creep->unit.location - proj.target;
							}
						}
					}
					
					auto cars = state->carMap_.getInRadius(proj.target, proj.splash + prototypes->variables.maxCreepSize);
					for(auto& car : cars)
					{
						auto carProto = prototypes->cars[car->unit.prototypeId];
						int32_t r = proj.splash + carProto.size;
						if (car->unit.location.sqDistanceTo(proj.target) < r*r)
						{
							car->unit.health -= proj.damage;
						}
					}
					
					proj.damage = -1;
				}			
			}
		}
		
		void removeDeadProjectiles(GameState* state)
		{
			if (state->isEventLoggerEnabled)
			{
				for(auto& p : state->projectiles)
				{
					if (p.damage == -1)
						state->eventLogger.addProjectileExplosion(state->time.time, p.id, p.prototypeId, p.target);
				}
			}
			
			state->projectiles.erase(std::remove_if(state->projectiles.begin(), state->projectiles.end(), 
			[](Projectile& proj){
				return proj.damage == -1;
				}
			),  state->projectiles.end());
		}
		
		void removeDeadCreeps(GameState* state)
		{
			if (state->creeps.size() > GameState::MAX_CREEPS)
				THROW_FATAL_ERROR("Creeps exceed maximum, pointers are invalid.");
			
			auto removeDeadCreep = [&state](CreepState& creep){
				state->creepById_.erase(creep.unit.id);
				auto formation = GameUtil::formationByCreep(creep, state);
				formation->slots[creep.formationsSlot] = -1;
				if (formation->subObjective == SUB_OBJECTIVE::MOVE && formation->carAgro <= 0)
				{
					formation->subObjective = SUB_OBJECTIVE::ASSAULT;
				}
				if (state->isEventLoggerEnabled)
				{	
					state->eventLogger.addCreepDeath(state->time.time, creep, creep.impact_);
				}
			};
			
			state->creeps.erase(std2::removeAndExecute(
					state->creeps, 
					[](CreepState& c){ return c.unit.health <= 0;},
					removeDeadCreep,
					[&state](CreepState& c){ state->creepById_[c.unit.id] = &c;}
				),
				state->creeps.end()
			);	
		}
		
		void removeDeadFormations(GameState* state)
		{
			for(auto& formation : state->formations)
			{
				bool hasCreeps = false;
				for(auto& slot : formation.slots)
				{
					if (slot > 0)
					{
						hasCreeps = true;
						break;
					}
				}
				if (!hasCreeps)
					formation.isDisposed_ = true;
			}
			for(auto& formation : state->formations)
			{
				if (formation.isDisposed_)
				{
					for(auto& slot : formation.slots)
					{
						if (slot > 0)
						{
							THROW_FATAL_ERROR("not implemented");
							//TODO			
						}
					}
				}			
			}
			state->formations.erase(std::remove_if(state->formations.begin(), state->formations.end(), 
			[](FormationState& formation){
				return formation.isDisposed_;
				}
			), state->formations.end());
		}
	}
}