#ifndef _POWER_UP_
#define _POWER_UP_

#include "PhysicalObject.h"

class PowerUp : public PhysicalObject
{
public:
	PowerUp();

	virtual const char								*GetClassName() const				{ return "PowerUp"; }
	virtual VECTOR3									GetDebugColor() const				{ return VECTOR3(0.0f, 0.0f, 1.0f); }
	virtual void									Refresh(const float &fDeltaTime);

protected:
	virtual ~PowerUp();

private:
	void											Init();
		

};

#endif