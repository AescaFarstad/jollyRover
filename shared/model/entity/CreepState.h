#pragma once
#include <ISerializable.h>
#include <BaseEntities.h>
#include <CreepProto.h>
#include <Point.h>

enum class CREEP_MODE : int32_t
{
	FORMATION = 1,
	ASSAULT = 2,
	RETREAT = 3
};


class CreepState
{
public:
	CreepState();
	~CreepState() = default;
	
	static const int8_t MAX_WHISKER_COUNT = 32;
	
	Object object;
	Unit unit;
	Weapon weapon;
	CREEP_MODE mode;
	int16_t whiskers[MAX_WHISKER_COUNT];
	int8_t numWhiskers;
	int32_t formationId;
	int8_t formationsSlot;
	bool sensedAnObstacle;
	
	Point velocity;
	float orientation;	
	
	CreepProto* _creepProto;
	WeaponProto* _weaponProto;
	Point movement_;
	Point targetLoc_;
	Point impact_;
	Point formationAttraction_;

	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;
	void propagatePrototypes(std::vector<CreepProto>& creepProtos, std::vector<WeaponProto>& weaponProtos);
	
	const Point& getLocation();

private:

};


namespace Serializer {

	void write(const CREEP_MODE& value, SerializationStream& stream);
	void read(CREEP_MODE& value, SerializationStream& stream);
}