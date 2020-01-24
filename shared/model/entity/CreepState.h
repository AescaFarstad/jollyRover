#pragma once
#include <ISerializable.h>
#include <BaseEntities.h>
#include <CreepProto.h>
#include <Point.h>


class CreepState
{
public:
	CreepState();
	
	static const int8_t MAX_WHISKER_COUNT = 32;
	
	Unit unit;
	Weapon weapon;
	std::array<int16_t, MAX_WHISKER_COUNT> whiskers;
	int8_t numWhiskers;
	int32_t formationId;
	int8_t formationsSlot;
	bool sensedAnObstacle;
	
	Point velocity;
	float orientation;	
	
	CreepProto* creepProto_;
	WeaponProto* weaponProto_;
	Point movement_;
	Point targetLoc_;
	Point impact_;
	Point formationAttraction_;

	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;
	void propagatePrototypes(std::vector<CreepProto>& creepProtos, std::vector<WeaponProto>& weaponProtos);
	
	const Point& getLocation();
	const int32_t& getId();

private:

};