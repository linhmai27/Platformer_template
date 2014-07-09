#include "Enemy.h"

Enemy::Enemy(const char* spriteFileName, GameLevelLayer* gamelevelLayer, Point position):BasePlatformOject(spriteFileName,gamelevelLayer,position)
{
	_onGround = false;
}

Enemy::~Enemy()
{

}

void		Enemy::update(float dt)
{
	updateMovement(dt);
	updateCollision();
}

void		Enemy::updateMovement(float dt)
{
	//Point jumpForce = ccp(0.0f,600.0f);
	//Point getHitForce = ccp(0.0f, 310.0f);
	//float jumCutOff = 2000.0f;
	//Point forwardMove = ccp(4000.0f, 0.0f);
	Point gravity = ccp(0.0f, -950.0f);

	//if(_onGround && _stateJump)
	//	_vVelocity = ccpAdd(_vVelocity, jumpForce);
	//else if(_stateJump && _vVelocity.y > jumCutOff)
	//	_vVelocity.y = jumCutOff;

	//if(_stateHitAbove)
	//{
	//	_vVelocity = ccpSub(_vVelocity,getHitForce);
	//	_stateHitAbove = false;
	//}



	//forwardMove.x = forwardMove.x*_vDirection.x;
	//Point forwardStep = ccpMult(forwardMove,dt);
	Point gravityStep = ccpMult(gravity,dt);
	////if(_onGround)
		//gravityStep = gravityStep*0.01f;

	_vVelocity = ccpAdd(_vVelocity, gravityStep);
	//_vVelocity = ccp(_vVelocity.x*0.9f,_vVelocity.y);

	//if(_stateRun)
	//	_vVelocity = ccpAdd(_vVelocity, forwardStep);

	const Point constVelocity = ccp(-50.0f,0.0f);
	if(_onGround)
		_vVelocity = ccpAdd(_vVelocity, constVelocity);
    Point minMovement = ccp(-50.0f, -150.0f);
    Point maxMovement = ccp(50.0f, 200.0f);
	//CCLOG(" velocity %f", _vVelocity.x);
	_vVelocity = ccpClamp(_vVelocity,minMovement,maxMovement);
	//CCLOG(" velocity %f", _vVelocity.x);
	//CCLOG(" stateRUn %d", _stateRun);
	

	CCPoint stepVelocity = ccpMult(_vVelocity,dt);
	_pDesiredPosition = ccpAdd(getPosition(),stepVelocity);
	//setPosition(_pDesiredPosition);
}

void		Enemy::updateCollision()
{
	if(outOfThisWorld())
		return;
	checkForAndResolveCollisions();
}

void		Enemy::checkForAndResolveCollisions()
{


	CCPoint playerPos = getPosition();
	CCArray* tiles = getSurroundingTilesAtPosition(playerPos);
	CCObject* obj;
	_onGround = false;

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
					_onGround = true;
					setVelocity(ccp(getVelocity().x, 0.0f));
					setDesiredPosition(ccp(getDesiredPosition().x, getDesiredPosition().y + intersectRect.size.height));
					//_player->setDesiredPosition(ccp(_player->getDesiredPosition().x, 13));
					//CCLOG("Player box %f", tileRect.getMaxY() - pRect.getMinY());
					//CCLOG("tile box %f", tileRect.getMaxY());
					//CCLOG("intersectRect box %f", intersectRect.size.height);
				}
				else if(tileIndx == 1)
				{
					//CCLOG("1 intersect ");
					
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
							_onGround = true;
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
					//if((tileIndx == 5 || tileIndx == 4) && getVelocity().y > 0 && !isSideHit)

				}
			}
		}

	}
	setPosition(getDesiredPosition());
}