#include <FormationState.h>

void FormationState::propagatePrototypes(std::vector<FormationProto>& formations)
{
	formationPrototype_ = &(formations[prototypeId]);
	
}