#include <Point.h>
#include <BaseEntities.h>

class ProjectileState
{
public:
		
	int32_t id;
	int16_t prototypeId;
	int32_t damage;
	int16_t force;
	int32_t splash;
	int32_t speed;
	int32_t weapon;
	int32_t spawnedAt;
	Point target;
	Point location;
};

#include <Serialization.h>

namespace Serialization
{
	
	//ProjectileState-------------------------------------------------------
	
	template <typename T>
	void write(const ProjectileState& object, T& serializer)
	{
		WRITE_FIELD(object, serializer, id);
		WRITE_FIELD(object, serializer, prototypeId);
		WRITE_FIELD(object, serializer, damage);
		WRITE_FIELD(object, serializer, force);
		WRITE_FIELD(object, serializer, splash);
		WRITE_FIELD(object, serializer, speed);
		WRITE_FIELD(object, serializer, weapon);
		WRITE_FIELD(object, serializer, spawnedAt);
		WRITE_FIELD(object, serializer, target);
		WRITE_FIELD(object, serializer, location);
	}
	
	template <typename T>
	void read(ProjectileState& object, T& serializer)
	{
		READ__FIELD(object, serializer, id);
		READ__FIELD(object, serializer, prototypeId);
		READ__FIELD(object, serializer, damage);
		READ__FIELD(object, serializer, force);
		READ__FIELD(object, serializer, splash);
		READ__FIELD(object, serializer, speed);
		READ__FIELD(object, serializer, weapon);
		READ__FIELD(object, serializer, spawnedAt);
		READ__FIELD(object, serializer, target);
		READ__FIELD(object, serializer, location);
	}
}