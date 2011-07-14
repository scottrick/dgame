#ifndef _PLAYER_SHIP_
#define _PLAYER_SHIP_

#include "PhysicalObject.h"

class PowerUp;

class PlayerShip : public PhysicalObject
{
public:
	PlayerShip();

	void										Clone(PlayerShip *pClone);
	void										CollectPowerup(PowerUp *pPowerUp);
	virtual void								Collide(PhysicalObject *pOtherObject);
	virtual PhysicalObject						*Duplicate();
	virtual void								FromBlock(Block *pBlock);

	virtual const char							*GetClassName() const			{ return "PlayerShip"; }
	virtual VECTOR3								GetDebugColor() const			{ return VECTOR3(1.0f, 0.0f, 1.0f); }
	virtual Shockwave							*GetShockwave();

	void										InitPlayerPosition();

	void										TestFire();
	virtual Block								*ToBlock();

protected:
	virtual ~PlayerShip();

	virtual void								OnDeath();
	virtual void								RefreshPosition(const float &fDeltaTime);

private:
	void										Init();

};

#endif