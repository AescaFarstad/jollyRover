#include <GameState.h>
#include <Field.h>
#include <limits>

namespace Creeps
{
	void handleCreeps(GameState* state, Prototypes* prototypes, int timePassed);
	
	namespace CreepsInternal
	{
		CreepState& spawnCreep(int16_t type, const Point& location, GameState* state, Prototypes* prototypes);
		void spawnProjectile(Point& from, Point& to, const WeaponProto* prototype, GameState* state);
		
		void preprocessCreeps(GameState* state, Prototypes* prototypes);
		void spawnFormation(GameState* state, Prototypes* prototypes, ForceProto& forceProto, FormationProto &formationProto);
		void processFormations(GameState* state, Prototypes* prototypes, int timePassed);
		void processCreeps(GameState* state, Prototypes* prototypes, int timePassed);
		void moveCreepTowardsPoint(CreepState& creep, Point& target, Prototypes* prototypes, int timePassed);
		
		void processProjectiles(GameState* state, Prototypes* prototypes, int timePassed);
		
		void processCreepAssault(CreepState& creep, GameState* state, Prototypes* prototypes, int timePassed);
		void performCreepAttack(CreepState& creep, Unit& target, GameState* state, int timePassed);
		void moveCreepTowardsPoint(CreepState& creep, Point& target, Prototypes* prototypes, int timePassed);
		
		void pushOutCreeps(GameState* state, Prototypes* prototypes, int timePassed);
		void preventCreepObstacleCollision(GameState* state, Prototypes* prototypes);
		
		CreepState* creepByid(int32_t id, std::vector<CreepState>& creeps);
		CreepState* getBestAssaultTargetForCreep(CreepState& creep, std::vector<CreepState>& creeps);
		
		void removeDeadProjectiles(GameState* state);
		void removeDeadCreeps(GameState* state);
		void removeDeadFormations(GameState* state);
	}
}