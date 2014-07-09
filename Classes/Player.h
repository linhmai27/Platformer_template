#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
using namespace cocos2d;
#include "BasePlatformObject.h"



class Player : public BasePlatformOject 
{
public:

	CC_PROPERTY(bool, _stateRun, StateRun);
	CC_PROPERTY(bool, _stateJump, StateJump);
	CC_PROPERTY(bool, _stateHitAbove, StateHitAbove);
	CC_PROPERTY(bool, _onFlatform, OnFlatform);


	Player(const char* ,GameLevelLayer*, Point position);
	~Player();


	void		update(float dt);
	void		checkForAndResolveCollisions();
	void		updateMovement(float);
	void		updateCollision();

};

#endif