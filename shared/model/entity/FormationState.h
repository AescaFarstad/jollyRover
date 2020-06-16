#include <BaseEntities.h>
#include <FormationProto.h>
#include <ObjectiveProto.h>
#include <Point.h>

enum class SUB_OBJECTIVE : int8_t
{
	NONE,
	MOVE,
	ASSAULT,
	PURSUE,
	APPROACH,
	HARVEST,
	RETREAT,
	ESCAPE,
	RESUPPLY
};

class FormationState
{
public:
	
	static constexpr int8_t MAX_FORMATION_COUNT = 32;
		
	int32_t id;
	int16_t prototypeId;
	int16_t objectiveID;
	int16_t force;
	std::vector<int32_t> slots;
	float speed;
	float angularSpeed;	
	Point location;
	Point targetLocation;
	float orientation;
	float targetOrientation;
	int32_t spawnedAt;
	int32_t objectiveChangedAt;
	SUB_OBJECTIVE subObjective;
	float agroAt;
	float bravery;
	int32_t carAgro;
	int32_t targetFormationId;
	
	bool isDisposed_;
	Point actualLocation_;
	float agro_;
	float hostile_;
	float friendly_;
	FormationProto* formationPrototype_;
	
	void propagatePrototypes(std::vector<FormationProto>& formations);

};

#include <Serialization.h>

namespace Serialization
{
	
	//FormationState-------------------------------------------------------
	
	template <typename T>
	void write(const FormationState& object, T& serializer)
	{
		WRITE_FIELD(object, serializer, id);
		WRITE_FIELD(object, serializer, prototypeId);
		WRITE_FIELD(object, serializer, objectiveID);
		WRITE_FIELD(object, serializer, force);
		WRITE_FIELD(object, serializer, slots);
		WRITE_FIELD(object, serializer, speed);
		WRITE_FIELD(object, serializer, angularSpeed);
		WRITE_FIELD(object, serializer, location);
		WRITE_FIELD(object, serializer, targetLocation);
		WRITE_FIELD(object, serializer, orientation);
		WRITE_FIELD(object, serializer, targetOrientation);
		WRITE_FIELD(object, serializer, spawnedAt);
		WRITE_FIELD(object, serializer, objectiveChangedAt);
		WRITE_FIELD(object, serializer, subObjective);
		WRITE_FIELD(object, serializer, agroAt);
		WRITE_FIELD(object, serializer, bravery);
		WRITE_FIELD(object, serializer, carAgro);
		WRITE_FIELD(object, serializer, targetFormationId);
	}
	
	template <typename T>
	void read(FormationState& object, T& serializer)
	{
		READ__FIELD(object, serializer, id);
		READ__FIELD(object, serializer, prototypeId);
		READ__FIELD(object, serializer, objectiveID);
		READ__FIELD(object, serializer, force);
		READ__FIELD(object, serializer, slots);
		READ__FIELD(object, serializer, speed);
		READ__FIELD(object, serializer, angularSpeed);
		READ__FIELD(object, serializer, location);
		READ__FIELD(object, serializer, targetLocation);
		READ__FIELD(object, serializer, orientation);
		READ__FIELD(object, serializer, targetOrientation);
		READ__FIELD(object, serializer, spawnedAt);
		READ__FIELD(object, serializer, objectiveChangedAt);
		READ__FIELD(object, serializer, subObjective);
		READ__FIELD(object, serializer, agroAt);
		READ__FIELD(object, serializer, bravery);
		READ__FIELD(object, serializer, carAgro);
		READ__FIELD(object, serializer, targetFormationId);
	
		object.formationPrototype_ = nullptr;
		object.isDisposed_ = false;
	}
	
	//SUB_OBJECTIVE-------------------------------------------------------
	
	template <typename T>
	void write(const SUB_OBJECTIVE& object, T& serializer)
	{
		serializer.write((int8_t)object, FIELD_NAME(SUB_OBJECTIVE));
	}
	
	template <typename T>
	void read(SUB_OBJECTIVE& object, T& serializer)
	{		
		int8_t tmp;
		serializer.read(tmp, FIELD_NAME(SUB_OBJECTIVE));
		object = (SUB_OBJECTIVE)tmp;
	}
}