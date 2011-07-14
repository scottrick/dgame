#ifndef _TEST_FIRE_EVENT_
#define _TEST_FIRE_EVENT_

#include "Event.h"

class Projectile;

class TestFireEvent : public Event
{
public:
	TestFireEvent();

	virtual void						Execute();
	virtual const char					*GetClassName() const				{ return "TestFireEvent"; }
	virtual void						Print(int indent) const;

	void								SetProjectile(Projectile *pProjectile);

protected:
	virtual ~TestFireEvent();

	Projectile							*m_pProjectile;

private:
	void								Init();

};

#endif