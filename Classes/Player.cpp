#include "Player.h"

Player::Player(const char* fileName)
{
	initWithFile(fileName);
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
	Vector jumpForce = ccp(0.0f,600.0f);
	Vector getHitForce = ccp(0.0f, 310.0f);
	float jumCutOff = 2000.0f;
	Vector forwardMove = ccp(4000.0f, 0.0f);
	Vector gravity = ccp(0.0f, -950.0f);

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
	Vector forwardStep = ccpMult(forwardMove,dt);
	CCPoint gravityStep = ccpMult(gravity,dt);
	//if(_onGround)
		//gravityStep = gravityStep*0.01f;
	this->_vVelocity = ccpAdd(_vVelocity, gravityStep);
	_vVelocity = ccp(_vVelocity.x*0.9f,_vVelocity.y);

	if(_stateRun)
		_vVelocity = ccpAdd(_vVelocity, forwardStep);


    Vector minMovement = ccp(-500.0f, -750.0f);
    Vector maxMovement = ccp(500.0f, 2000.0f);
	//CCLOG(" velocity %f", _vVelocity.x);
	_vVelocity = ccpClamp(_vVelocity,minMovement,maxMovement);
	CCLOG(" velocity %f", _vVelocity.x);
	//CCLOG(" stateRUn %d", _stateRun);
	CCPoint stepVelocity = ccpMult(_vVelocity,dt);
	_pDesiredPosition = ccpAdd(getPosition(),stepVelocity);
	//setPosition(_pDesiredPosition);


}
CCRect      Player::collisionBoundingBox()
{
	CCRect collisionBox = RectInset(this->boundingBox(), 3.0f, 0.0f);
	CCPoint diff = ccpSub(this->_pDesiredPosition, this->getPosition());
    CCRect returnBoundingBox = RectOffset(collisionBox, diff.x, diff.y);
    return returnBoundingBox;
}

/*___________________________Getter and Setter___________________________*/

Vector		Player::getVelocity()
{
	return _vVelocity;
}
void		Player::setVelocity(Vector otherVector)
{
	_vVelocity = otherVector;
}
CCPoint		Player::getDesiredPosition()
{
	return _pDesiredPosition;
}
void		Player::setDesiredPosition(CCPoint otherPosition)
{
	_pDesiredPosition = otherPosition;
}
void		Player::setOnGround(bool ground)
{
	_onGround = ground;
}
bool		Player::getOnGround()
{
	return _onGround;
}
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
void		Player::setDirection(Vector direction)
{
	_vDirection = ccp(direction.x, direction.y);
}
Vector		Player::getDirection()
{
	return _vDirection;
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

CCRect		Player::RectInset(CCRect box, float dx, float dy)
{
	return CCRect(box.origin.x, box.origin.y, box.size.width - 2*dx, box.size.height -2*dy);
}
CCRect		Player::RectOffset(CCRect box, float dx, float dy)
{
	return CCRect(box.origin.x + dx, box.origin.y + dy, box.size.width, box.size.height);
}

