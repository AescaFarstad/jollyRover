#include <Creeps.h>
namespace Creeps
{
	using namespace CreepsInternal;
	
	void processCreeps(GameState* state, Prototypes* prototypes, int timePassed)
	{
		
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
		[](Creep& creep){
			return creep.unit.health <= 0;
			}
		), state->creeps.end());
	}
	
	namespace CreepsInternal
	{
		
		void spawnCreep(int16_t force, GameState* state, Prototypes* prototypes)
		{
			//S::log.add("spawn creep");
			CreepProto& creepProto = state->random.getFromVector(prototypes->creeps);
			WeaponProto& weaponProto = prototypes->weapons[creepProto.weapon];
			ForceProto& forceProto = prototypes->forces[force];
			
			
			state->creeps.emplace_back();
			Creep& creep = state->creeps.back();
			
			creep.object.prototypeId = creepProto.id;
			creep.object.id = state->idCounter++;
			
			creep.unit.force = force;
			creep.unit.health = creepProto.maxHealth;
			creep.unit.location.x = state->random.get(forceProto.spawnAA.x, forceProto.spawnBB.x);
			creep.unit.location.y = state->random.get(forceProto.spawnAA.y, forceProto.spawnBB.y);
			
			creep.weapon.prototypeId = weaponProto.id;
			creep.weapon.attackCooldown = 0;
			
		}
		
		void fireProjectile(Point& from, Point& to, const WeaponProto* prototype, GameState* state)
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
	}
}