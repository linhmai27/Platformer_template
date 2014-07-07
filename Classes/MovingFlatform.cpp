#include "MovingFlatform.h"

MovingFlatform::MovingFlatform(const char* spriteName, Point origin, Point target)
{
	initWithFile(spriteName);
	setPosition(origin);
	settargetMove(target);
}

MovingFlatform::~MovingFlatform()
{
}

void MovingFlatform::settargetMove(Point target)
{
	_pTarget = Point(target);
}

Point MovingFlatform::gettargetMove()
{
	return _pTarget;
}

