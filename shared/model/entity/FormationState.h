#include <ISerializable.h>
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
	FormationState() = default;
	~FormationState() = default;
	
	static const int8_t MAX_FORMATION_COUNT = 32;
		
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
	

	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;
	
	void propagatePrototypes(std::vector<FormationProto>& formations);

};
	

namespace Serializer {

	void write(const SUB_OBJECTIVE& value, SerializationStream& stream);
	void read(SUB_OBJECTIVE& value, SerializationStream& stream);
}