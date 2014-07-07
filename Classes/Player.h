#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
using namespace cocos2d;

#define Vector CCPoint



class Player : public CCSprite 
{
public:
	CC_PROPERTY(Vector, _vVelocity, Velocity);
	CC_PROPERTY(CCPoint, _pDesiredPosition, DesiredPosition);
	CC_PROPERTY(bool, _onGround, OnGround);
	CC_PROPERTY(bool, _stateRun, StateRun);
	CC_PROPERTY(bool, _stateJump, StateJump);
	CC_PROPERTY(bool, _stateHitAbove, StateHitAbove);
	CC_PROPERTY(Vector, _vDirection, Direction);
	CC_PROPERTY(bool, _onFlatform, OnFlatform);


	Player(const char*);
	~Player();

	void		update(float dt);
	CCRect		collisionBoundingBox();
	CCRect		RectInset(CCRect, float, float);
	CCRect		RectOffset(CCRect, float, float);



};

#endif