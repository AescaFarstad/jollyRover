#include <Creeps.h>

namespace Creeps
{
	using namespace CreepsInternal;
	
	void handleCreeps(GameState* state, Prototypes* prototypes, int timePassed)
	{
		preprocessCreeps(state, prototypes);
		
		
		for(auto& force : prototypes->forces)
		{
			if (!force.alwaysOn)
				continue;
			if (state->_forceStrength[force.id] < prototypes->variables.intensity)
			{
				FormationProto& formation = state->random.getFromVector(prototypes->formations);
				spawnFormation(state, prototypes, force, formation);
				state->_forceStrength[force.id] += formation.strength;
			}
		}
		
		processFormations(state, prototypes, timePassed);
		processCreeps(state, prototypes, timePassed);
		
		
		
		
		removeDeadProjectiles(state);
		removeDeadCreeps(state);
		removeDeadFormations(state);
	}
	
	
	namespace CreepsInternal
	{
		
		void preprocessCreeps(GameState* state, Prototypes* prototypes)
		{
			state->_forceStrength.clear();
			for(auto& i : prototypes->forces)
				state->_forceStrength.push_back(0);
				
			for(CreepState& creep : state->creeps)
			{
				if (creep._creepProto == nullptr)
				{
					creep._creepProto = &(prototypes->creeps[creep.object.prototypeId]);
					creep._weaponProto = &(prototypes->weapons[creep._creepProto->weapon]);
				}
				state->_forceStrength[creep.unit.force] += creep._creepProto->strength;
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
			creep.unit.location.setTo(location);
			
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
			step.scaleTo(creep._creepProto->speed * timePassed);
			
			Point nextLoc = creep.unit.location + step;
			Obstacle* obstacle = Field::findObstacle(nextLoc, prototypes);
			if (obstacle)
			{
				direction = -FieldMath.crossProduct(_unit, target, obstacle.polygon.center);
			
				var bestPoint:Dot;
				var bestDistance:Number = Number.POSITIVE_INFINITY;
				for (var j:int = 0; j < obstacle.polygon.vertices.length; j++)
				{
					var dir:Number = FieldMath.crossProduct(_unit, target, obstacle.polygon.vertices[j]);
					if (dir > 0 != direction > 0)
						continue;
					var distance:Number = obstacle.polygon.vertices[j].distanceTo(_unit);
					if (distance < bestDistance)
					{
						bestPoint = obstacle.polygon.vertices[j];
						bestDistance = distance;
					}
				}
			}
			creep.unit.location.setTo(nextLoc);
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