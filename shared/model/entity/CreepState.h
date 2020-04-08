#pragma once
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

	void propagatePrototypes(std::vector<CreepProto>& creepProtos, std::vector<WeaponProto>& weaponProtos);
	
	const Point& getLocation();
	const int32_t& getId();

private:

};

#include <Serialization.h>

namespace Serialization
{
	
	//CreepState-------------------------------------------------------
	
	template <typename T>
	void write(const CreepState& object, T& serializer)
	{
		WRITE_FIELD(object, serializer, unit);
		WRITE_FIELD(object, serializer, weapon);
		WRITE_FIELD(object, serializer, numWhiskers);
		WRITE_FIELD(object, serializer, formationId);
		WRITE_FIELD(object, serializer, formationsSlot);
		WRITE_FIELD(object, serializer, sensedAnObstacle);
		WRITE_FIELD(object, serializer, velocity);
		WRITE_FIELD(object, serializer, orientation);
		serializer.writeArray(object.whiskers, object.numWhiskers, FIELD_NAME(whiskers));
	}
	
	template <typename T>
	void read(CreepState& object, T& serializer)
	{
		READ__FIELD(object, serializer, unit);
		READ__FIELD(object, serializer, weapon);
		READ__FIELD(object, serializer, numWhiskers);
		READ__FIELD(object, serializer, formationId);
		READ__FIELD(object, serializer, formationsSlot);
		READ__FIELD(object, serializer, sensedAnObstacle);
		READ__FIELD(object, serializer, velocity);
		READ__FIELD(object, serializer, orientation);
		serializer.readArray(object.whiskers, object.numWhiskers, FIELD_NAME(whiskers));
		for(int8_t i = object.numWhiskers; i < CreepState::MAX_WHISKER_COUNT; i++)
			object.whiskers[i] = -1;
		object.creepProto_ = nullptr;
		object.weaponProto_ = nullptr;
	}
}