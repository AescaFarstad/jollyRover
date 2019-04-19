#include <ISerializable.h>
#include <BaseEntities.h>
#include <FormationProto.h>
#include <ObjectiveProto.h>
#include <Point.h>

/*
class FormationSlot
{
public:
	FormationSlot() = default;
	~FormationSlot() = default;
	

	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;

};
*/

class FormationState
{
public:
	FormationState() = default;
	~FormationState() = default;
	
	static const int8_t MAX_FORMATION_COUNT = 32;
	
	Object object;
	int16_t objectiveID;
	int16_t force;
	std::vector<int32_t> slots;
	float speedMulti;
	float speed;
	float angularSpeed;	
	Point location;
	Point targetLocation;
	float orientation;
	float targetOrientation;
	int32_t spawnedAt;
	
	bool isDisposed_;
	FormationProto* formationPrototype_;
	ObjectiveProto* objectivePrototype_;
	

	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;
	
	void propagatePrototypes(std::vector<FormationProto>& formations, std::vector<std::vector<ObjectiveProto>>& objectives);

};
	