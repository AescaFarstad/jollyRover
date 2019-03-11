
#include <ISerializable.h>
#include <Point.h>
#include <BaseEntities.h>

class Projectile
{
public:
	Projectile() = default;
	~Projectile() = default;
	
	Object object;
	int32_t damage;
	int32_t splash;
	int32_t speed;
	Point target;
	Point location;
	
	
	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;
};