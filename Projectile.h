#ifndef _PROJECTILE_
#define _PROJECTILE_

#include "PhysicalObject.h"

class Weapon;

class Projectile : public PhysicalObject
{
public:

	Projectile();
	Projectile(Node *pNode);

	void											Clone(Projectile *pClone);
	virtual void									Collide(PhysicalObject *pOtherObject);
	virtual PhysicalObject							*Duplicate();
	virtual void									FromBlock(Block *pBlock);

	PhysicalObject									*GetCreator() const					{ return m_pCreator; }
	virtual const char								*GetClassName() const				{ return "Projectile"; }
	virtual VECTOR3									GetDebugColor() const				{ return VECTOR3(0.0f, 1.0f, 1.0f); }
	virtual Shockwave								*GetShockwave();
	Weapon											*GetWeapon() const					{ return m_pWeapon; }

	virtual void									Refresh(const float &fDeltaTime);
	void											SetCreator(PhysicalObject *pCreator);
	void											SetWeapon(Weapon *pWeapon);
	virtual Block									*ToBlock();

protected:
	
	virtual ~Projectile();

	virtual void									OnDeath();

	PhysicalObject									*m_pCreator;
	Weapon											*m_pWeapon;

private:
	void											Init();

};

#endif