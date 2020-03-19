#include <CreepState.h>


CreepState::CreepState()
{
	creepProto_ = nullptr;
	weaponProto_ = nullptr;
	unit.host_.type = ENTTITY_TYPE::CREEP;
	unit.host_.pointer = this;
}

void CreepState::propagatePrototypes(std::vector<CreepProto>& creepProtos, std::vector<WeaponProto>& weaponProtos)
{
	creepProto_ = &(creepProtos[unit.prototypeId]);
	weaponProto_ = &(weaponProtos[creepProto_->weapon]);
	
}

const Point& CreepState::getLocation()
{
	return unit.location;
}

const int32_t& CreepState::getId()
{
	return unit.id;
}