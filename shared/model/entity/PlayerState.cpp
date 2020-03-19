#include <PlayerState.h>


PlayerState::PlayerState()
{
	login = 0; 
	score = 0;
	isHeadless = false;
	isAI = false;
	repairsTotal = 0;
	repairsLeft = 0;
	refuelTotal = 0;
	refuelLeft = 0;
	wentOfflineAt = 0;
}

PlayerState::PlayerState(int32_t login) : PlayerState()
{
	this->login = login;
}
