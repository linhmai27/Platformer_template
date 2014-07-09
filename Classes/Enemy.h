#ifndef __ENEMY_H_
#define __ENEMY_H_
#include "BasePlatformObject.h"

class Enemy : public BasePlatformOject
{
public :
	Enemy(const char*, GameLevelLayer*, Point);
	~Enemy();
	void		update(float dt);
	void		checkForAndResolveCollisions();
	void		updateMovement(float);
	void		updateCollision();
};
#endif;