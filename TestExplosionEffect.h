#ifndef _TEST_EXPLOSION_EFFECT_
#define _TEST_EXPLOSION_EFFECT_

#include "Object.h"

class PhysicalObject;

#define				EXPLOSION_GROW_SPEED		3.50f  //its size multiplier after one second
#define				EXPLOSION_SHRINK_SPEED		6.00f

//NOT REALLY AN EFFECT ANYMORE...
class TestExplosionEffect : public Object
{
public:
	TestExplosionEffect();

	void							CreateFromPhysicalObject(PhysicalObject *pPhysObject);
	virtual void					FromBlock(Block *pBlock);
	virtual const char				*GetClassName() const			{ return "TestExplosionEffect"; }

	virtual void					Refresh(const float &fDeltaTime);
	virtual Block					*ToBlock();

protected:
	~TestExplosionEffect();

	float							m_fCurrentAge;
	float							m_fDuration; //in seconds
	float							m_fRotation;

private:
	void							Init();

};

#endif