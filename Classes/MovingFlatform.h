#ifndef __MOVING_FLATFORM_H__
#define __MOVING_FLATFORM_H__
#include "cocos2d.h"
USING_NS_CC;
class MovingFlatform : public CCSprite
{
	public:
		MovingFlatform(const char* spriteName, Point origin, Point target);
		~MovingFlatform();

	CC_PROPERTY(Point,_pTarget, targetMove);

};

#endif