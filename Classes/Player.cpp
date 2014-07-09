#include "Player.h"

Player::Player(const char* spriteFileName,GameLevelLayer* gameLevelLayer, Point position)
														:BasePlatformOject(spriteFileName
																			, gameLevelLayer
																			, position)
{
	
	//this->scheduleUpdate();
	_vVelocity = ccp(0.0f, 0.0f);
	_pDesiredPosition = this->getPosition();
	//CCLOG("_pDesiredPosition x and y : %f   %f", _pDesiredPosition.x, _pDesiredPosition.y);
	_vDirection = ccp(1.0f, 0.0f);
	_onGround = false;
	_stateRun = false;
	_stateJump = false;
	_stateHitAbove = false;
}
Player::~Player()
{
}
void		Player::update(float dt)
{
	updateMovement(dt);
	updateCollision();


}

void		Player::updateMovement(float dt)
{
	Point jumpForce = ccp(0.0f,600.0f);
	Point getHitForce = ccp(0.0f, 310.0f);
	float jumCutOff = 2000.0f;
	Point forwardMove = ccp(4000.0f, 0.0f);
	Point gravity = ccp(0.0f, -950.0f);

	if(_onGround && _stateJump)
		_vVelocity = ccpAdd(_vVelocity, jumpForce);
	else if(_stateJump && _vVelocity.y > jumCutOff)
		_vVelocity.y = jumCutOff;

	if(_stateHitAbove)
	{
		_vVelocity = ccpSub(_vVelocity,getHitForce);
		_stateHitAbove = false;
	}



	forwardMove.x = forwardMove.x*_vDirection.x;
	Point forwardStep = ccpMult(forwardMove,dt);
	CCPoint gravityStep = ccpMult(gravity,dt);
	//if(_onGround)
		//gravityStep = gravityStep*0.01f;
	this->_vVelocity = ccpAdd(_vVelocity, gravityStep);
	_vVelocity = ccp(_vVelocity.x*0.9f,_vVelocity.y);

	if(_stateRun)
		_vVelocity = ccpAdd(_vVelocity, forwardStep);


    Point minMovement = ccp(-500.0f, -750.0f);
    Point maxMovement = ccp(500.0f, 2000.0f);
	//CCLOG(" velocity %f", _vVelocity.x);
	_vVelocity = ccpClamp(_vVelocity,minMovement,maxMovement);
	//CCLOG(" velocity %f", _vVelocity.x);
	//CCLOG(" stateRUn %d", _stateRun);
	CCPoint stepVelocity = ccpMult(_vVelocity,dt);
	_pDesiredPosition = ccpAdd(getPosition(),stepVelocity);
	//setPosition(_pDesiredPosition);
}

void		Player::updateCollision()
{
	checkForAndResolveCollisions();
}

void		Player::checkForAndResolveCollisions()
{
	CCPoint playerPos = getPosition();
	CCArray* tiles = getSurroundingTilesAtPosition(playerPos);
	setOnGround(false);
	CCObject* obj;



	bool isSideHit = false;
	CCARRAY_FOREACH(tiles,obj)	
	{
		CCDictionary* dict = (CCDictionary*)obj;



		Integer* x = (Integer*)dict->objectForKey("x");
		Integer* y = (Integer*)dict->objectForKey("y");
		float height = _map->getTileSize().height;
		float width = _map->getTileSize().width;
		//_debugDraw->appendRect(ccp(x->getValue(), y->getValue()), width, height);


		CCRect pRect = collisionBoundingBox();

		//_debugDraw->appendRect(ccp(pRect.getMinX(), pRect.getMinY()), pRect.getMaxX() - pRect.getMinX() , pRect.getMaxY() - pRect.getMinY(), 1.0f, 0.0f, 0.0f);
		Integer* gid = (Integer*)dict->objectForKey("gid");
		if(gid->getValue())
		{

			CCRect tileRect = CCRectMake((float)x->getValue(),(float)y->getValue(), _map->getTileSize().width, _map->getTileSize().height);
	
			if(pRect.intersectsRect(tileRect))
			{

				CCRect intersectRect = pRect.intersectsWithRect(tileRect);	
				int tileIndx = tiles->getIndexOfObject(obj);
				//CCLOG("tileIndx %d : " ,tileIndx);
				if(tileIndx == 0)
				{
					//CCLOG("0 intersect ");
					//_debugDraw->appendRect(ccp(intersectRect.getMinX(), intersectRect.getMinY()), intersectRect.getMaxX() - intersectRect.getMinX() , intersectRect.getMaxY() - intersectRect.getMinY(), 1.0f, 0.0f, 0.0f);
					setOnGround(true);
					setVelocity(ccp(getVelocity().x, 0.0f));
					setDesiredPosition(ccp(getDesiredPosition().x, getDesiredPosition().y + intersectRect.size.height));
					//setDesiredPosition(ccp(getDesiredPosition().x, 13));
					//CCLOG("Player box %f", tileRect.getMaxY() - pRect.getMinY());
					//CCLOG("tile box %f", tileRect.getMaxY());
					//CCLOG("intersectRect box %f", intersectRect.size.height);
				}
				else if(tileIndx == 1)
				{
					//CCLOG("1 intersect ");
					setStateHitAbove(true);
					setDesiredPosition(ccp(getDesiredPosition().x, getDesiredPosition().y - intersectRect.size.height));
				}	
				else if(tileIndx == 2)
				{
					//CCLOG("2 intersect ");
					isSideHit = true;
					setVelocity(ccp(0.0f,getVelocity().y));
					setDesiredPosition(ccp(getDesiredPosition().x  + intersectRect.size.width, getDesiredPosition().y));
				}
				else if(tileIndx == 3)
				{
					//CCLOG("3 intersect ");
					isSideHit = true;
					setVelocity(ccp(0.0f,getVelocity().y));
					setDesiredPosition(ccp(getDesiredPosition().x  - intersectRect.size.width, getDesiredPosition().y));
				}
				else
				{
					if(intersectRect.size.width > intersectRect.size.height)
					{
						float resolutionHeight;
						if(tileIndx > 5)
						{
							resolutionHeight  = intersectRect.size.height;
							setVelocity(ccp(getVelocity().x, 0.0f));
							setOnGround(true);
						}
						else
						{
							resolutionHeight  = -intersectRect.size.height;
						}
						setDesiredPosition(ccp(getDesiredPosition().x, getDesiredPosition().y + resolutionHeight ));
					}
					else
					{
						float resolutionWidth;
						if (tileIndx == 6 || tileIndx == 4) 
						{
							resolutionWidth = intersectRect.size.width;
						}
						else
						{
							resolutionWidth = -intersectRect.size.width;
						}
						setDesiredPosition(ccp(getDesiredPosition().x + resolutionWidth, getDesiredPosition().y ));
					}
					if((tileIndx == 5 || tileIndx == 4) && getVelocity().y > 0 && !isSideHit)
						setStateHitAbove(true);
				}
			}
		}

	}
	setPosition(getDesiredPosition());
	
	//CCLOG("bounding box 2 x and y %f %f", getDesiredPosition().x, pRect.getMaxY());
	//CCLOG("DONE CHECK");


}

/*___________________________Getter and Setter___________________________*/


void		Player::setStateJump(bool stateJump)
{
	_stateJump = stateJump;
}
bool		Player::getStateJump()
{
	return _stateJump;
}
void		Player::setStateRun(bool stateRun)
{
	_stateRun = stateRun;
}
bool		Player::getStateRun()
{
	return _stateRun;
}

void		Player::setStateHitAbove(bool hit)
{
	_stateHitAbove = hit;
}
bool		Player::getStateHitAbove()
{
	return _stateHitAbove;
}
void		Player::setOnFlatform(bool flatform)
{
		_onFlatform = flatform;
}
bool		Player::getOnFlatform()
{
		return _onFlatform;
}
/*___________________________Helper Functions___________________________*/

