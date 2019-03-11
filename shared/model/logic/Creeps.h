#include <GameState.h>
#include <Prototypes.h>
#include <limits>

namespace Creeps
{
	void processCreeps(GameState* state, Prototypes* prototypes, int timePassed);
	void processProjectiles(GameState* state, Prototypes* prototypes, int timePassed);
	void removeDeadProjectiles(GameState* state);
	void removeDeadCreeps(GameState* state);
	
	namespace CreepsInternal
	{
		void spawnCreep(int16_t force, GameState* state, Prototypes* prototypes);
		void fireProjectile(Point& from, Point& to, const WeaponProto* prototype, GameState* state);
	}
}