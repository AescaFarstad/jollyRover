#include <Creeps.h>
#include <FMath.h>

namespace Creeps
{
	using namespace CreepsInternal;
	
	void checkCreepProtos(std::vector<CreepState> creeps, Prototypes* prototypes)
	{
		for(auto& creep : creeps)
			if (creep._creepProto != &prototypes->creeps[creep.object.prototypeId])
				THROW_FATAL_ERROR("frefer");
	}
	
	void handleCreeps(GameState* state, Prototypes* prototypes, int timePassed)
	{
		preprocessCreeps(state, prototypes);
		
		
		
		for(auto& force : prototypes->forces)
		{
			if (state->formations.size() == FormationState::MAX_FORMATION_COUNT)
				break;
			if (!force.alwaysOn)
				continue;			
			if (state->forceStrength_[force.id] < prototypes->variables.intensity)
			{
				FormationProto& formation = state->random.getFromVector(prototypes->formations);
				spawnFormation(state, prototypes, force, formation);
				state->forceStrength_[force.id] += formation.strength;
			}
		}
			
		for(auto& creep : state->creeps)
			creep.movement_.scaleTo(0);
		
		if (!state->creepMap_.m_isValid)
		{
			Point BB(prototypes->variables.fieldWidth + 100, prototypes->variables.fieldHeight + 100);
			state->creepMap_ = SpatialMap<CreepState>(50, true, Point(-100, -100), BB);
		}
		state->creepMap_.setUnique(state->creeps);
		
		
			
		processFormations(state, prototypes, timePassed);
		processCreeps(state, prototypes, timePassed);
		processProjectiles(state, prototypes, timePassed);
		
		
		pushOutCreeps(state, prototypes, timePassed);
		preventCreepObstacleCollision(state, prototypes);
		for(auto& creep : state->creeps)
			creep.unit.location += creep.movement_;
		
		
		removeDeadProjectiles(state);
		removeDeadCreeps(state);
		removeDeadFormations(state);
		
	}
		
	Point getCurrentSlotLocation(FormationState& formation, int32_t slot)
	{
		return formation.location + formation.formationPrototype_->slots[slot].offset.rotate(formation.orientation + M_PI / 2);			
	}
	
	Point getTargetSlotLocation(FormationState& formation, int32_t slot)
	{
		return formation.targetLocation + formation.formationPrototype_->slots[slot].offset.rotate(formation.targetOrientation + M_PI / 2);			
	}
	
	
	namespace CreepsInternal
	{
		
		void preprocessCreeps(GameState* state, Prototypes* prototypes)
		{
			state->forceStrength_.clear();
			state->forceStrength_.resize(prototypes->forces.size());
				
			for(CreepState& creep : state->creeps)
			{
				if (creep._creepProto == nullptr)
				{
					THROW_FATAL_ERROR("Prototypes are not resolved");
				}
				state->forceStrength_[creep.unit.force] += creep._creepProto->strength;
			}
		}
		
		void spawnFormation(GameState* state, Prototypes* prototypes, ForceProto& forceProto, FormationProto& formationProto)
		{
			Point formationCenter(
				state->random.get(forceProto.spawnAA.x, forceProto.spawnBB.x),
				state->random.get(forceProto.spawnAA.y, forceProto.spawnBB.y)
			);
			
			float angle = prototypes->variables.fieldCenter.subtract(formationCenter).asAngle();
			
			state->formations.emplace_back();
			FormationState& formation = state->formations.back();
			formation.objectiveID = -1;
			formation.object.id = state->idCounter++;
			formation.object.prototypeId = formationProto.id;
			formation.force = forceProto.id;
			formation.formationPrototype_ = &formationProto;
			formation.isDisposed_ = false;
			formation.orientation = angle;
			formation.location = formationCenter;
			formation.speedMulti = 1;
			formation.speed = FMath::F_MAX;
			formation.angularSpeed = 0;
			
			for(auto& slot : formationProto.slots)
			{
				if (!slot.optional)
				{
					
					CreepState& creep = spawnCreep(slot.creepType, getCurrentSlotLocation(formation, formation.slots.size()), state, prototypes);
					creep.mode = CREEP_MODE::FORMATION;
					creep.unit.force = forceProto.id;
					creep.formationId = formation.object.id;
					creep.formationsSlot = formation.slots.size();
					formation.slots.push_back(creep.object.id);
					formation.speed = std::min(formation.speed, creep._creepProto->speed);
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
			creep._creepProto = &(prototypes->creeps[type]);
			creep._weaponProto = &(prototypes->weapons[creep._creepProto->weapon]);
			
			creep.numWhiskers = creep._creepProto->whiskers + 1;
			for(int8_t i = 0; i < creep.numWhiskers; i++)
			{
				creep.whiskers[i] = state->random.get(i * 0.7f, i * 1.3f) / creep.numWhiskers * creep._creepProto->maxWhiskerLength;
				if (i > 0 && creep.whiskers[i] - creep.whiskers[i - 1] < 10)
					creep.whiskers[i] = creep.whiskers[i - 1] + 10;
			}
			for(int8_t i = creep.numWhiskers; i < CreepState::MAX_WHISKER_COUNT; i++)
			{
				creep.whiskers[i] = -1;
			}
			
			creep.object.prototypeId = type;
			creep.object.id = state->idCounter++;
			
			creep.unit.health = creep._creepProto->maxHealth;
			creep.unit.location = location;
			
			creep.weapon.prototypeId = creep._creepProto->weapon;
			creep.weapon.attackCooldown = 0;
			creep.weapon.target = -1;
			
			creep.formationId = -1;
			creep.formationsSlot = -1;
			
			return creep;
		}
		
		void processFormations(GameState* state, Prototypes* prototypes, int timePassed)
		{			
			for(auto& formation : state->formations)
			{
				if (formation.objectiveID == -1)
				{
					ObjectiveProto& objective = state->random.getFromVector(prototypes->objectives[formation.force]);
					formation.objectiveID = objective.id;
					formation.targetLocation = objective.location;
					formation.targetOrientation = prototypes->variables.fieldCenter.subtract(objective.location).asAngle();
					formation.objectivePrototype_ = &objective;
					float expectedTravelTime = formation.targetLocation.subtract(formation.location).getLength() / formation.speed;
					formation.angularSpeed = FMath::angleDelta(formation.orientation, formation.targetOrientation) / expectedTravelTime; 
				}
				
				float balance = 0;
				float maxDisabalance = 0;
				int32_t creepBalanceCount = 1;
				for(auto& creep : state->creeps)
				{
					if (creep.formationId == formation.object.id)
					{
						Point formationSlotLocation = getCurrentSlotLocation(formation, creep.formationsSlot);
						Point targetSlotLocation = getTargetSlotLocation(formation, creep.formationsSlot);
						Point creep2Target = targetSlotLocation - creep.unit.location;
						Point creep2Slot = formationSlotLocation - creep.unit.location;
						
						Obstacle* obstacle = Field::findObstacle(formationSlotLocation, prototypes);
						
						Point* target;
						if (!obstacle)
						{
							creepBalanceCount++;
							float creep2TargetL = creep2Target.getLength();
							float creep2SlotL = creep2Slot.getLength();
							
							if (creep2TargetL < creep2SlotL)
							{
								balance += creep2SlotL - creep2TargetL;
								target = &targetSlotLocation;
							}
							else
							{
								balance -= creep2SlotL;
								maxDisabalance = std::max(maxDisabalance, creep2SlotL);
								target = &formationSlotLocation;
							}							
						}
						else
						{
							target = &targetSlotLocation;
						}
						
						moveCreepTowardsPoint(creep, *target, prototypes, timePassed);						
					}
				}				
				
				balance /= creepBalanceCount;
				//balance = std::min(-maxDisabalance, balance);
				
				formation.speedMulti = FMath::lerp(0, 0.9, -20, 0.5, balance);
				formation.speedMulti = std::max(0.1f, formation.speedMulti);
				
				Point formation2Target = formation.targetLocation - formation.location;
				float stepSize = timePassed * formation.speed * formation.speedMulti;
				
				if (formation2Target.getLength() > stepSize)
				{
					formation2Target.scaleTo(stepSize);
					formation.location += formation2Target;
					formation.orientation += timePassed * formation.angularSpeed * formation.speedMulti;
				}
				else
				{
					formation.isDisposed_ = true;
				}
			}
		}
		
		void processCreeps(GameState* state, Prototypes* prototypes, int timePassed)
		{
			for(auto& creep : state->creeps)
			{
				switch (creep.mode)
				{
					case CREEP_MODE::ASSAULT:
						processCreepAssault(creep, state, prototypes, timePassed);
						break;
					case CREEP_MODE::RETREAT:
						break;
					default:
						break;
				}
			}
			
		}
		
		void processCreepAssault(CreepState& creep, GameState* state, Prototypes* prototypes, int timePassed)
		{
			if (creep.unit.health <= 0)
				return;
			
			CreepState* target = nullptr;
			if (creep.weapon.target > 0)
				target = creepByid(creep.weapon.target, state->creeps);
			if (target == nullptr || target->unit.health <= 0)
				target = getBestAssaultTargetForCreep(creep, state->creeps);
			if (target == nullptr)
			{
				creep.weapon.target = -1;
				return;
			}
			
			if (target->unit.location.distanceTo(creep.unit.location) < creep._weaponProto->range)
			{
				performCreepAttack(creep, target->unit, state, timePassed);
			}
			else
			{
				moveCreepTowardsPoint(creep, target->unit.location, prototypes, timePassed);
			}
		}
		
		void performCreepAttack(CreepState& creep, Unit& target, GameState* state, int timePassed)
		{
			if (creep.weapon.attackCooldown > 0)
			{
				creep.weapon.attackCooldown -= timePassed;				
			}
			else
			{
				creep.weapon.attackCooldown = 1000000 / creep._weaponProto->attackSpeed;
				spawnProjectile(creep.unit.location, target.location, creep._weaponProto, state);
			}
		}
		
		void moveCreepTowardsPoint(CreepState& creep, Point& target, Prototypes* prototypes, int timePassed)
		{
			Point step = target - creep.unit.location;
			
			if (step.getLength() == 0)
				return;
			
			float stepSize = creep._creepProto->speed * timePassed;
			step.scaleTo(stepSize);
			
			for(auto& whisker : creep.whiskers)
			{
				if (whisker < 0 || whisker > (target - creep.unit.location).getLength())
					break;
					
				Point whiskerStep = step;
				whiskerStep.scaleTo(1 + whisker);
				Point nextLoc = creep.unit.location + whiskerStep;
				Obstacle* obstacle = Field::findObstacle(nextLoc, prototypes);
				if (obstacle)
				{
					Point creepToTarget = target - creep.unit.location;
					float direction = creepToTarget.crossProduct(obstacle->centroid - creep.unit.location);
					
					Point* bestPoint = nullptr;
					float bestDistance = FMath::F_MAX;
					for(auto& vert : obstacle->vertices)
					{
						float vertDir = creepToTarget.crossProduct(vert - creep.unit.location);
						if ((vertDir < 0) != (direction < 0))
						{
							float distance = vert.distanceTo(creep.unit.location);
							if (distance < bestDistance)
							{
								bestDistance = distance;
								bestPoint = &vert;
							}
						}
					}
					step = *bestPoint - creep.unit.location;
					step.scaleTo(stepSize);
					creep.movement_ = step;
					return;
				}
			}
			creep.movement_ = step;
		}
		
		CreepState* creepByid(int32_t id, std::vector<CreepState>& creeps)
		{
			auto result = std::find_if(creeps.begin(), creeps.end(), [id](CreepState& creep){ return creep.object.id == id;});
			return result == creeps.end() ? nullptr : &*result;
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
					float bestDistance = FMath::F_MAX;					
					Edge creepEdge(&creep.unit.location, &nextLoc);
					Edge* bestEdge = nullptr;
					
					for(auto& edge : obstacle->edges)
					{
						Point intersection = FMath::getEdgeIntersection(creepEdge, edge);
						if (!intersection.isNaN())
						{
							float distance = creep.unit.location.distanceTo(intersection);
							if (distance < bestDistance)
							{
								bestDistance = distance;
								bestEdge = &edge;
							}
						}
					}
					
					if (bestEdge == nullptr)
						continue;
					Point& p1 = *bestEdge->p1;
					Point& p2 = *bestEdge->p2;
					Point* target;
					
					if (p1.x == p2.x)
					{
						if ((p1.y > p2.y) == (creep.movement_.y > 0))
							target = &p1;
						else
							target = &p2;
					}
					else
					{
						if ((p1.x > p2.x) == (creep.movement_.x > 0))
							target = &p1;
						else
							target = &p2;
					}
					
					float length = creep.movement_.getLength();
					creep.movement_ = *target - creep.unit.location;
					creep.movement_.scaleTo(length / 2);
					
				}				
			}
		}	
		
		void pushOutCreeps(GameState* state, Prototypes* prototypes, int timePassed)
		{
			for(auto& creep : state->creeps)
			{
				std::vector<CreepState*> possibleCollisions = state->creepMap_.getInRadius(
						creep.unit.location, creep._creepProto->size + prototypes->variables.maxCreepSize);
				
				for(auto& creep2 : possibleCollisions)
				{
					if (creep2 >= &creep)
						continue;
						
					int32_t collisionRadius = creep._creepProto->size + creep2->_creepProto->size;
					Point creep2Creep = creep.unit.location - creep2->unit.location;
					if (creep2Creep.getLength() == 0)
						creep2Creep.x = 0.1;
					float penetration = (collisionRadius - creep2Creep.getLength()) / 2;
					if (penetration > 0)
					{
						//float direction = creep2Creep.asAngle();
						int32_t totalWeight = creep._creepProto->weight + creep2->_creepProto->weight;
						float force = penetration * prototypes->variables.creepRestitution / totalWeight;
						
						if (creep.unit.force == creep2->unit.force && creep.object.prototypeId != creep2->object.prototypeId)
							force /= 10;
						
						creep2Creep.scaleTo(force * creep2->_creepProto->weight);
						creep.movement_ += creep2Creep;
						
						creep2Creep.scaleTo(force * creep._creepProto->weight);
						creep2->movement_ -= creep2Creep;
						
					}
				}
			}
			
		}
		
		void spawnProjectile(Point& from, Point& to, const WeaponProto* prototype, GameState* state)
		{
			state->projectiles.emplace_back();
			Projectile& projectile = state->projectiles.back();
			
			auto distance = from.distanceTo(to);
			
			projectile.object.id = state->idCounter++;
			projectile.object.prototypeId = prototype->id;
			projectile.speed = prototype->bulletSpeed;
			projectile.splash = prototype->splash;
			projectile.damage = prototype->damage;
			projectile.target = to;
			projectile.location = from;
			
			projectile.target.x += state->random.get(-1.0f, 1.0f) * distance * 0.1;
			projectile.target.y += state->random.get(-1.0f, 1.0f) * distance * 0.1;
		}
		
		void processProjectiles(GameState* state, Prototypes* prototypes, int timePassed)
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
					for(CreepState& creep : state->creeps)
					{
						if (creep.unit.location.distanceTo(proj.target) < proj.splash)
						{
							creep.unit.health -= proj.damage;
						}
					}
					proj.damage = -1;
				}			
			}
		}
		
		void removeDeadProjectiles(GameState* state)
		{
			state->projectiles.erase(std::remove_if(state->projectiles.begin(), state->projectiles.end(), 
			[](Projectile& proj){
				return proj.damage == -1;
				}
			), state->projectiles.end());
		}
		
		void removeDeadCreeps(GameState* state)
		{
			auto removeFrom = std::remove_if(state->creeps.begin(), state->creeps.end(), 
			[](CreepState& creep){
				return creep.unit.health <= 0;
				});
			auto iter = removeFrom;
			while(iter != state->creeps.end())
			{
				if (iter->formationId)
				{
					auto formation = std::find_if(state->formations.begin(), state->formations.end(), [id = iter->formationId](FormationState& form){ 
						return form.object.id == id;
					});
					if (formation != state->formations.end())
					{
						formation->isDisposed_ = true;
					}
				}
				iter++;
			}
			
			state->creeps.erase(removeFrom,  state->creeps.end());
		}
		
		void removeDeadFormations(GameState* state)
		{
			for(auto& formation : state->formations)
			{
				if (formation.isDisposed_)
				{
					for(auto& slot : formation.slots)
					{
						if (slot > 0)
						{
							for(auto& creep : state->creeps)
							{
								if (creep.object.id == slot)
									creep.mode = CREEP_MODE::ASSAULT;
							}						
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