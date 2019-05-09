#include <GameState.h>

namespace Creeps
{
	void handleCreeps(GameState* state, Prototypes* prototypes, int32_t timePassed);
	Point getCurrentSlotLocation(FormationState& formation, int32_t slot);
	Point getTargetSlotLocation(FormationState& formation, int32_t slot);
		
	
	namespace CreepsInternal
	{
		
		void preprocessCreeps(GameState* state, Prototypes* prototypes);
		void runDebugAsserts(GameState* state, Prototypes* prototypes);
		void spawnFormations(GameState* state, Prototypes* prototypes);
		void updateCache(GameState* state, Prototypes* prototypes);
		
		void spawnFormation(GameState* state, Prototypes* prototypes, ForceProto& forceProto, FormationProto &formationProto);
		CreepState& spawnCreep(int16_t type, const Point& location, GameState* state, Prototypes* prototypes);
		void spawnProjectile(Point& from, Point& to, const WeaponProto* prototype, int16_t force, GameState* state);
		
		void processFormations(GameState* state, Prototypes* prototypes, int32_t timePassed);
		void tryOptimiseFormation(FormationState& formation, GameState* state, int32_t timePassed);		
		void findObjective(FormationState& formation, GameState* state, Prototypes* prototypes);
		void moveFormation(FormationState& formation, GameState* state, Prototypes* prototypes, int32_t timePassed);		
		void performAssault(FormationState& formation, GameState* state, Prototypes* prototypes, int32_t timePassed);
		void accelerateTractor(CreepState& creep, float amount);
		void changeTractorOrientation(CreepState& creep, float targetOrientation, int32_t timePassed);
		CreepState* getBestAssaultTargetForCreep(CreepState& creep, std::vector<CreepState>& creeps);
		void performCreepAttack(CreepState& creep, Unit& target, GameState* state, int32_t timePassed);
		void moveUnitOutOfObstacle(CreepState& creep, Obstacle* obstacle, int32_t timePassed);		
		void moveCreepTowardsPoint(CreepState& creep, Point& target, Prototypes* prototypes, int32_t timePassed);
		void moveWalkerTowardsPoint(CreepState& creep, Point& target, Prototypes* prototypes, int32_t timePassed);
		void moveTractorTowardsPoint(CreepState& creep, Point& target, Prototypes* prototypes, int32_t timePassed);
		Point avoidObstacles(CreepState& creep, float stepSize, const Point& target, Prototypes* prototypes);
		
		void processProjectiles(GameState* state, Prototypes* prototypes, int32_t timePassed);		
		
		void applyCreepMovement(GameState* state);
		void pushOutCreeps(GameState* state, Prototypes* prototypes, int32_t timePassed);
		void pushCreepsOutOfObstacles(GameState* state, Prototypes* prototypes, int32_t timePassed);
		void preventCreepObstacleCollision(GameState* state, Prototypes* prototypes);		
		
		void removeDeadProjectiles(GameState* state);
		void removeDeadCreeps(GameState* state);
		void removeDeadFormations(GameState* state);
	}
}