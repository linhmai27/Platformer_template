#ifndef __BASEPLATFORMOBJECT_H_
#define __BASEPLATFORMOBJECT_H_
#include "cocos2d.h"
USING_NS_CC;
class GameLevelLayer;
class BasePlatformOject : public Sprite
{
	CC_PROPERTY(Point, _vVelocity, Velocity);
	CC_PROPERTY(CCPoint, _pDesiredPosition, DesiredPosition);
	CC_PROPERTY(Point, _vDirection, Direction);
	CC_PROPERTY(bool, _onGround, OnGround);
public:
	BasePlatformOject(const char* ,GameLevelLayer*, Point position);
	~BasePlatformOject();
	CCPoint				tileCoordForPosition( CCPoint pos);
	CCRect				tileRectFromTileCoords( CCPoint pos);
	CCArray*			getSurroundingTilesAtPosition(CCPoint position);
	CCRect				collisionBoundingBox();
	CCRect				RectInset(CCRect, float, float);
	CCRect				RectOffset(CCRect, float, float);
	bool				outOfThisWorld();

	virtual void		checkForAndResolveCollisions();
	virtual void		updateMovement(float);
	virtual void		updateCollision();

	bool				isDead() { return _isDead; }
	void				Dead(bool dead) { _isDead = dead; }
protected:
	TMXTiledMap*		_map;
	TMXLayer*			_walls;
	GameLevelLayer*		_lGameLevel;
	bool				_isOutOfThisWorld;
	bool				_isDead;
};

#endif;

