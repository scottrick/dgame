#ifndef _TEST_SHOCKWAVE_THING_
#define _TEST_SHOCKWAVE_THING_

#include "Object.h"
#include "Shockwave.h"

class Block;

class TestShockwaveThing : public Object
{
public:
	TestShockwaveThing();

	void						FromBlock(Block *pBlock);
	virtual const char			*GetClassName() const			{ return "TestShockwaveThing"; }
	virtual void				Refresh(const float &fDeltaTime);
	Block						*ToBlock();

protected:
	virtual ~TestShockwaveThing();

	float						m_fTimeTillNextThing;

private:
	

};

#endif