#include "GameLevelLayer.h"
#include "Player.h"
#include "Enemy.h"
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

	_player = new Player("mario.png",this,ccp(100.0f,300.0f) );
	//_player->setAnchorPoint(ccp(0.0f,0.0f));
	_map->addChild(_player,1,PLAYER);

	//TMXObjectGroup* objectgroup = _map->getObjectGroup("movingFlatform");
	//CCArray* objects = objectgroup->getObjects();
 //   Dictionary* dict;
 //   Object* pObj = NULL;


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

	

	initializeEnemy();
	_debugDraw = DebugDraw::create();

    _map->addChild(_debugDraw,2,DEBUGPOLYGON);
	this->scheduleUpdate();

    EventListenerKeyboard* listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(GameLevelLayer::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(GameLevelLayer::onKeyReleased, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}


void		GameLevelLayer::initializeEnemy()
{
	TMXObjectGroup* objectgroup = _map->getObjectGroup("monster");
	CCArray* objects = objectgroup->getObjects();
    Dictionary* dict;
    Object* pObj = NULL;


    CCARRAY_FOREACH(objects, pObj)
    {
        dict = static_cast<Dictionary*>(pObj);

        if(!dict)
            break;
        const char* key = "x";
        float x = static_cast<String*>(dict->objectForKey(key))->floatValue();
        key = "y";
		float y = static_cast<String*>(dict->objectForKey(key))->floatValue();
		key = "sprite";
		CCString* spriteName = static_cast<String*>(dict->objectForKey(key));



		Enemy* temp = new Enemy(spriteName->getCString(), this, ccp(x + 48/2,y+48/2));
		temp->autorelease();
		_map->addChild(temp,11);
		_enemies.push_back(temp);
		//_enemies.back()->retain();

    } 
}

void		GameLevelLayer::update(float dt)
{
	//Rect pRect = _player->getBoundingBox();
	//CCLOG("x and y : %f   %f", _player->getPosition().x, _player->getPosition().y);

	_player->update(dt);

	std::list<Enemy*>::iterator curEnemy = _enemies.begin();
	while(curEnemy != _enemies.end())
	{
		if(!(*curEnemy)->isDead())
		{
			(*curEnemy)->update(dt);
			++curEnemy;
		}
		else
		{

			_map->removeChild((*curEnemy),true);
			//delete *curEnemy;
			//(*curEnemy)->release();
 			curEnemy = _enemies.erase(curEnemy);
			
		}
	}

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




void		GameLevelLayer::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{

	if(keyCode == EventKeyboard::KeyCode::KEY_A)
	{
		_player->setStateRun(true);
		_player->setDirection(ccp(-1.0f,0.0f));
	
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
