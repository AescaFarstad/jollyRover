#include <ISerializable.h>
#include <BaseEntities.h>
#include <FormationProto.h>
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
	FormationState() = default;;
	~FormationState() = default;;
	
	Object object;
	int16_t objectiveID;
	int16_t force;
	float direction;
	Point _location;
	bool _isDisposed;
	FormationProto* _prototype;	
	std::vector<int32_t> slots;
	

	void deserialize(SerializationStream& stream);
	void serialize(SerializationStream& stream) const;

};
	