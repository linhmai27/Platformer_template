#include "BasePlatformObject.h"
#include "GameLevelLayer.h"
BasePlatformOject::BasePlatformOject(const char* spriteFileName,GameLevelLayer* gameLevelLayer,Point position)
{
	_isDead = false;

	initWithFile(spriteFileName);
	setPosition(position);
	_vVelocity = ccp(0.0f, 0.0f);
	_lGameLevel = gameLevelLayer;
	_lGameLevel->retain();
	_map = _lGameLevel->getMap();
	_map->retain();
	_walls = _lGameLevel->getWalls();
	_walls->retain();
}

BasePlatformOject::~BasePlatformOject()
{
	_lGameLevel->release();
	_map->release();
	_walls->release();
}


CCPoint		BasePlatformOject::tileCoordForPosition( CCPoint position)
{
	float x = floor(position.x / _map->getTileSize().width);
	float levelHeightInPixels = _map->getMapSize().height * _map->getTileSize().height;
	float y = floor((levelHeightInPixels - position.y) / _map->getTileSize().height);
	return ccp(x, y);
}
CCRect		BasePlatformOject::tileRectFromTileCoords( CCPoint tileCoords)
{
	float levelHeightInPixels = _map->getMapSize().height * _map->getTileSize().height;
	CCPoint origin = ccp(tileCoords.x *_map->getTileSize().width, levelHeightInPixels - ((tileCoords.y + 1) * _map->getTileSize().height));
	return CCRectMake(origin.x, origin.y, _map->getTileSize().width, _map->getTileSize().height);
}
CCArray*	BasePlatformOject::getSurroundingTilesAtPosition(CCPoint position)
{
	CCPoint plPos = tileCoordForPosition(position);
	//CCLOG("tile position plPos : %f ", plPos.x);
	CCArray* gids = CCArray::create();

	for (int i = 0; i < 9; i++) {
		int c = i % 3;
		int r = (int)(i / 3);
		CCPoint tilePos = ccp(plPos.x + (c - 1), plPos.y + (r - 1));
 
		int tgid = _walls->tileGIDAt(tilePos);

 
		CCRect tileRect = tileRectFromTileCoords(tilePos);
 
		CCDictionary *tileDict = CCDictionary::create();
		tileDict->setObject(CCInteger::create(tgid),"gid");
		tileDict->setObject(CCInteger::create(tileRect.origin.x),"x");
		tileDict->setObject(CCInteger::create(tileRect.origin.y),"y");
		tileDict->setObject(CCInteger::create(tilePos.x),"tilePosX");
		tileDict->setObject(CCInteger::create(tilePos.y),"tilePosY");

		gids->addObject(tileDict);
	}

	  gids->removeObjectAtIndex(4);
	  gids->insertObject(gids->objectAtIndex(2), 6);
	  gids->removeObjectAtIndex(2);
	  gids->exchangeObjectAtIndex(4,6);
	  gids->exchangeObjectAtIndex(0,4);


	  return gids;
}
void		BasePlatformOject::updateMovement(float dt)
{
}

void		BasePlatformOject::updateCollision()
{
}
void		BasePlatformOject::checkForAndResolveCollisions()
{
}

bool		BasePlatformOject::outOfThisWorld()
{
	Point tilePos = tileCoordForPosition(getDesiredPosition());
	CCLOG("x and y : %f   %f", tilePos.x, tilePos.y);

	if(tilePos.x >= _map->getMapSize().width || tilePos.y >= _map->getMapSize().height || tilePos.x <=0 || tilePos.y <=0)
	{
		_isDead = true;
		return true;
	}
	else 
		return false;
}



CCRect      BasePlatformOject::collisionBoundingBox()
{
	CCRect collisionBox = RectInset(this->boundingBox(), 3.0f, 0.0f);
	CCPoint diff = ccpSub(this->_pDesiredPosition, this->getPosition());
    CCRect returnBoundingBox = RectOffset(collisionBox, diff.x, diff.y);
    return returnBoundingBox;
}
CCRect		BasePlatformOject::RectInset(CCRect box, float dx, float dy)
{
	return CCRect(box.origin.x, box.origin.y, box.size.width - 2*dx, box.size.height -2*dy);
}
CCRect		BasePlatformOject::RectOffset(CCRect box, float dx, float dy)
{
	return CCRect(box.origin.x + dx, box.origin.y + dy, box.size.width, box.size.height);
}


/*___________________________Getter and Setter___________________________*/

void		BasePlatformOject::setDirection(Point direction)
{
	_vDirection = ccp(direction.x, direction.y);
}
Point		BasePlatformOject::getDirection()
{
	return _vDirection;
}

Point		BasePlatformOject::getVelocity()
{
	return _vVelocity;
}
void		BasePlatformOject::setVelocity(Point otherPoint)
{
	_vVelocity = otherPoint;
}



CCPoint		BasePlatformOject::getDesiredPosition()
{
	return _pDesiredPosition;
}
void		BasePlatformOject::setDesiredPosition(CCPoint otherPosition)
{
	_pDesiredPosition = otherPosition;
}

void		BasePlatformOject::setOnGround(bool ground)
{
	_onGround = ground;
}
bool		BasePlatformOject::getOnGround()
{
	return _onGround;
}