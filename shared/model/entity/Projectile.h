
#include <ISerializable.h>
#include <Point.h>
#include <BaseEntities.h>

class Projectile
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
	
	
	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;
};