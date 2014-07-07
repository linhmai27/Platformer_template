#include "GameLevelLayer.h"
#include "Player.h"
#include "MovingFlatform.h"
USING_NS_CC;

Scene*		GameLevelLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameLevelLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}
bool		GameLevelLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();



    /////////////////////////////
    // 3. add your codes below...

	CCLayerColor* colorLayer = CCLayerColor::create(ccc4(255, 0, 0, 255));
	//this->addChild(colorLayer,-2);

	_map = TMXTiledMap::create("levelTest.tmx");
	//_map->setPosition(ccp(0,300.0f));
	_walls = _map->layerNamed("walls");
	this->addChild(_map,10,BACKGROUND);

	//TMXObjectGroup* objectgroup = _map->getObjectGroup("movingFlatform");
	//CCArray* objects = objectgroup->getObjects();
 //   Dictionary* dict;
 //   Object* pObj = NULL;

	_player = new Player("mario.png");
	_player->setPosition(ccp(100.0f,300.0f));
	//_player->setAnchorPoint(ccp(0.0,0.0f));
	_map->addChild(_player,1,PLAYER);

  //  CCARRAY_FOREACH(objects, pObj)
  //  {
  //      dict = static_cast<Dictionary*>(pObj);

  //      if(!dict)
  //          break;
  //      const char* key = "x";
  //      float x = static_cast<String*>(dict->objectForKey(key))->floatValue();
  //      key = "y";
  //      float y = static_cast<String*>(dict->objectForKey(key))->floatValue();
		//key = "targetX";
		//float targetX = static_cast<String*>(dict->objectForKey(key))->floatValue();
		//key = "targetY";
		//float targetY = static_cast<String*>(dict->objectForKey(key))->floatValue();
		//key = "sprite";
		//CCString* spriteName = static_cast<String*>(dict->objectForKey(key));

		//targetX = targetX*_map->getTileSize().width;
		//targetY = _map->getTileSize().height*targetY;

		//CCLOG("TARGET X, TARGET y %f    %f",targetX,targetX); 

		//MovingFlatform* temp = new MovingFlatform(spriteName->getCString(), ccp(x,y), ccp(targetX, targetY));
		//_map->addChild(temp);
		//temp->addChild(_player,1);
		//CCMoveBy* actionMove = CCMoveBy::create(2.0f,temp->gettargetMove());
		//CCMoveBy* actionMoveBack = actionMove->reverse();
		//temp->runAction(RepeatForever::create(Sequence::create(actionMove,actionMoveBack,NULL)));
		//_movingFlatforms.push_back(temp);

  //  } 





	_debugDraw = DebugDraw::create();

    _map->addChild(_debugDraw,2,DEBUGPOLYGON);
	this->scheduleUpdate();

    EventListenerKeyboard* listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(GameLevelLayer::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(GameLevelLayer::onKeyReleased, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}
void		GameLevelLayer::update(float dt)
{
	//Rect pRect = _player->getBoundingBox();
	//CCLOG("x and y : %f   %f", _player->getPosition().x, _player->getPosition().y);

	_player->update(dt);
	checkForAndResolveCollisions();
	setViewPointCenter();
	//_debugDraw->appendRect(pRect.origin, pRect.size.width, pRect.size.height);
	//CCLOG("Player veolocity %f %f", _player->getVelocity().x,_player->getVelocity().y);

}
void		GameLevelLayer::menuCloseCallback(Object* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
CCPoint		GameLevelLayer::tileCoordForPosition( CCPoint position)
{
	float x = floor(position.x / _map->getTileSize().width);
	float levelHeightInPixels = _map->getMapSize().height * _map->getTileSize().height;
	float y = floor((levelHeightInPixels - position.y) / _map->getTileSize().height);
	return ccp(x, y);
}
CCRect		GameLevelLayer::tileRectFromTileCoords( CCPoint tileCoords)
{
	float levelHeightInPixels = _map->getMapSize().height * _map->getTileSize().height;
	CCPoint origin = ccp(tileCoords.x *_map->getTileSize().width, levelHeightInPixels - ((tileCoords.y + 1) * _map->getTileSize().height));
	return CCRectMake(origin.x, origin.y, _map->getTileSize().width, _map->getTileSize().height);
}
CCArray*	GameLevelLayer::getSurroundingTilesAtPosition(CCPoint position)
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
void		GameLevelLayer::checkForAndResolveCollisions()
{
	CCPoint playerPos = _player->getPosition();
	CCArray* tiles = getSurroundingTilesAtPosition(playerPos);
	_player->setOnGround(false);
	CCObject* obj;

	if(! _debugDraw->isRectEmpty() )
	{
		_debugDraw->removeAllRect();
	}


	bool isSideHit = false;
	CCARRAY_FOREACH(tiles,obj)	
	{
		CCDictionary* dict = (CCDictionary*)obj;



		Integer* x = (Integer*)dict->objectForKey("x");
		Integer* y = (Integer*)dict->objectForKey("y");
		float height = _map->getTileSize().height;
		float width = _map->getTileSize().width;
		//_debugDraw->appendRect(ccp(x->getValue(), y->getValue()), width, height);


		CCRect pRect = _player->collisionBoundingBox();

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
					_player->setOnGround(true);
					_player->setVelocity(ccp(_player->getVelocity().x, 0.0f));
					_player->setDesiredPosition(ccp(_player->getDesiredPosition().x, _player->getDesiredPosition().y + intersectRect.size.height));
					//_player->setDesiredPosition(ccp(_player->getDesiredPosition().x, 13));
					//CCLOG("Player box %f", tileRect.getMaxY() - pRect.getMinY());
					//CCLOG("tile box %f", tileRect.getMaxY());
					//CCLOG("intersectRect box %f", intersectRect.size.height);
				}
				else if(tileIndx == 1)
				{
					//CCLOG("1 intersect ");
					_player->setStateHitAbove(true);
					_player->setDesiredPosition(ccp(_player->getDesiredPosition().x, _player->getDesiredPosition().y - intersectRect.size.height));
				}	
				else if(tileIndx == 2)
				{
					//CCLOG("2 intersect ");
					isSideHit = true;
					_player->setVelocity(ccp(0.0f,_player->getVelocity().y));
					_player->setDesiredPosition(ccp(_player->getDesiredPosition().x  + intersectRect.size.width, _player->getDesiredPosition().y));
				}
				else if(tileIndx == 3)
				{
					//CCLOG("3 intersect ");
					isSideHit = true;
					_player->setVelocity(ccp(0.0f,_player->getVelocity().y));
					_player->setDesiredPosition(ccp(_player->getDesiredPosition().x  - intersectRect.size.width, _player->getDesiredPosition().y));
				}
				else
				{
					if(intersectRect.size.width > intersectRect.size.height)
					{
						float resolutionHeight;
						if(tileIndx > 5)
						{
							resolutionHeight  = intersectRect.size.height;
							_player->setVelocity(ccp(_player->getVelocity().x, 0.0f));
							_player->setOnGround(true);
						}
						else
						{
							resolutionHeight  = -intersectRect.size.height;
						}
						_player->setDesiredPosition(ccp(_player->getDesiredPosition().x, _player->getDesiredPosition().y + resolutionHeight ));
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
						_player->setDesiredPosition(ccp(_player->getDesiredPosition().x + resolutionWidth, _player->getDesiredPosition().y ));
					}
					if((tileIndx == 5 || tileIndx == 4) && _player->getVelocity().y > 0 && !isSideHit)
						_player->setStateHitAbove(true);
				}
			}
		}

	}
	_player->setPosition(_player->getDesiredPosition());
	
	//CCLOG("bounding box 2 x and y %f %f", _player->getDesiredPosition().x, pRect.getMaxY());
	//CCLOG("DONE CHECK");


}
void		GameLevelLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{

	if(keyCode == EventKeyboard::KeyCode::KEY_A)
	{
		_player->setStateRun(true);
		_player->setDirection(ccp(-1.0,0.0));
	
	}
	if(keyCode == EventKeyboard::KeyCode::KEY_D)
	{
		_player->setStateRun(true);
		_player->setDirection(ccp(1.0f,0.0f));
	}
	if(keyCode == EventKeyboard::KeyCode::KEY_W)
	{
	}
	if(keyCode == EventKeyboard::KeyCode::KEY_S)
	{
	}
	if(keyCode == EventKeyboard::KeyCode::KEY_SPACE)
	{
		_player->setStateJump(true);
	}

}
void		GameLevelLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if(keyCode == EventKeyboard::KeyCode::KEY_A)
	{
		_player->setStateRun(false);
	}
	if(keyCode == EventKeyboard::KeyCode::KEY_D)
	{
		_player->setStateRun(false);
	}
	if(keyCode == EventKeyboard::KeyCode::KEY_W)
	{
	}
	if(keyCode == EventKeyboard::KeyCode::KEY_S)
	{
	}
	if(keyCode == EventKeyboard::KeyCode::KEY_SPACE)
	{
		_player->setStateJump(false);
	}

}
void		GameLevelLayer::setViewPointCenter()
{
	CCSize winSize = CCDirector::getInstance()->getWinSize();

	int x = MAX(_player->getPositionX(), winSize.width/2);
	float y = _map->getPositionY();

	x = MIN(x, (_map->getMapSize().width*_map->getTileSize().width) - winSize.width/2);

	Point actualPos = ccp(x,y);
	Point centerOfView = ccp(winSize.width/2, winSize.height/2);
	Point viewPoint = ccpSub(centerOfView, actualPos);
	_map->setPosition(viewPoint.x, _map->getPositionY());
}
void		GameLevelLayer::checkForAndResolveCollisionsMovingFlatform()
{
	//std::list<MovingFlatform*>::iterator movingFlatform = _movingFlatforms.begin();
	//for(movingFlatform, movingFlatform != _movingFlatforms.end(), movingFlatform++)
	//{

	//}
	//
}