#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "DebugDraw.h"
using namespace cocos2d;
enum gameObjectTag {  };
enum gameStaticObjectTag { PLAYER, BACKGROUND , DEBUGPOLYGON};
class Player;
class MovingFlatform;
class Enemy;
class GameLevelLayer : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    void initializeEnemy();
    // a selector callback
    void		menuCloseCallback(Object* pSender);

	void		setViewPointCenter();

	// keyboard callback
    void		onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void		onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    
	void		update(float dt);
    // implement the "static create()" method manually
    CREATE_FUNC(GameLevelLayer);
	TMXTiledMap* getMap() { return _map;}
	TMXLayer* getWalls() { return _walls;}
private:
	TMXTiledMap*		_map;
	TMXLayer*			_walls;
	TMXLayer*			_lMonster;
	DebugDraw*			_debugDraw;
	Player*				_player;
	std::list<Enemy*> _enemies;
	std::list<MovingFlatform*> _movingFlatforms;
};

#endif // __HELLOWORLD_SCENE_H__
