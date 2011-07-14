#ifndef _WEAPON_
#define _WEAPON_

#include "Component.h"

#include "def.h"

class Block;

class Weapon : public Component
{
public:

	struct Damage {
		float			m_fEnergyDamage;
		float			m_fPhysicalDamage;
	};

	Weapon();

	void								Clone(Weapon *pClone);
	void								Fire();
	virtual void						FromBlock(Block *pBlock);
	float								GetAcceleration() const					{ return m_fAcceleration; }
	virtual const char					*GetClassName() const					{ return "Weapon"; }
	float								GetCooldown() const						{ return m_fCooldown; }
	Damage								GetDamage()	const						{ return m_Damage; }
	float								GetSpeed() const						{ return m_fSpeed; }
	bool								IsReady() const;
	void								Refresh(const float &fDeltaTime);
	void								SetAcceleration(float fAcceleration)	{ m_fAcceleration = fAcceleration; }
	void								SetAim(VECTOR3 vAim);
	void								SetCooldown(float fCooldown);
	void								SetDamage(Damage damage)				{ m_Damage = damage; }
	void								SetSpeed(float fSpeed)					{ m_fSpeed = fSpeed; }
	Block								*ToBlock();

protected:	
	virtual ~Weapon();

	float								m_fAcceleration;					//weapon acceleration
	float								m_fCooldown;						//the weapons cooldown
	float								m_fCurrentCooldown;					//current cooldown time remaining
	float								m_fSpeed;							//projectile speed
	float								m_fFiringSpread;					//angle (in radians) where the weapon may actually fire

	VECTOR3								m_vAim;								//the direction the weapon is aiming
	Damage								m_Damage;							//weapon damage

private:
	void								Init();

};


#endif