#include <Creeps.h>

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
			state->creepMap_ = SpatialMap<CreepState>(50, Point(-100, -100), BB);
		}
		state->creepMap_.set(state->creeps);
		
		
			
		processFormations(state, prototypes, timePassed);
		processCreeps(state, prototypes, timePassed);
		processProjectiles(state, prototypes, timePassed);
		
		
		pushOutCreeps(state, prototypes, timePassed);
		for(auto& creep : state->creeps)
			creep.unit.location += creep.movement_;
		
		
		removeDeadProjectiles(state);
		removeDeadCreeps(state);
		removeDeadFormations(state);
		
	}
	
	
	namespace CreepsInternal
	{
		
		void preprocessCreeps(GameState* state, Prototypes* prototypes)
		{
			state->forceStrength_.clear();
			for(auto& i : prototypes->forces)
				state->forceStrength_.push_back(0);
				
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
			formation.object.prototypeId = forceProto.id;
			formation.force = forceProto.id;
			formation._prototype = &formationProto;
			formation._isDisposed = false;
			formation.direction = angle;
			
			for(auto& slot : formationProto.slots)
			{
				if (!slot.optional)
				{
					
					CreepState& creep = spawnCreep(slot.creepType, formationCenter + slot.offset.rotate(angle), state, prototypes);
					creep.mode = CREEP_MODE::FORMATION;
					creep.unit.force = forceProto.id;
					formation.slots.push_back(creep.object.id);
				}
			}
		}
		
		
		CreepState& spawnCreep(int16_t type, const Point& location, GameState* state, Prototypes* prototypes)
		{
			state->creeps.emplace_back();
			CreepState& creep = state->creeps.back();
			creep._creepProto = &(prototypes->creeps[type]);
			creep._weaponProto = &(prototypes->weapons[creep._creepProto->weapon]);
			
			creep.object.prototypeId = type;
			creep.object.id = state->idCounter++;
			
			creep.unit.health = creep._creepProto->maxHealth;
			creep.unit.location = location;
			
			creep.weapon.prototypeId = creep._creepProto->weapon;
			creep.weapon.attackCooldown = 0;
			creep.weapon.target = -1;
			
			return creep;
		}		
		
		void processFormations(GameState* state, Prototypes* prototypes, int timePassed)
		{
			for(auto& formation : state->formations)
			{
				formation._isDisposed = true;
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
			float stepSize = creep._creepProto->speed * timePassed;
			step.scaleTo(stepSize);
			
			Point nextLoc = creep.unit.location + step;
			Obstacle* obstacle = Field::findObstacle(nextLoc, prototypes);
			if (obstacle)
			{
				Point creepToTarget = target - creep.unit.location;
				float direction = creepToTarget.crossProduct(obstacle->centroid - creep.unit.location);
				
				Point* bestPoint = nullptr;
				float bestDistance = std::numeric_limits<float>::max();
				for(auto& vert : obstacle->vertices)
				{
					float vertDir = creepToTarget.crossProduct(vert - creep.unit.location);
					if (vertDir * direction < 0)
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
				nextLoc = creep.unit.location + step;
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
			state->creeps.erase(std::remove_if(state->creeps.begin(), state->creeps.end(), 
			[](CreepState& creep){
				return creep.unit.health <= 0;
				}
			), state->creeps.end());
		}
		
		void removeDeadFormations(GameState* state)
		{
			state->formations.erase(std::remove_if(state->formations.begin(), state->formations.end(), 
			[](FormationState& formation){
				return formation._isDisposed;
				}
			), state->formations.end());
		}
	}
}