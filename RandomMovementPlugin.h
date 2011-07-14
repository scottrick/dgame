#ifndef _RANDOM_MOVEMENT_PLUGIN_
#define _RANDOM_MOVEMENT_PLUGIN_

#include "MovementPlugin.h"

class RandomMovementPlugin : public MovementPlugin
{
public:

	static RandomMovementPlugin			*GetPlugin();

	virtual void							AssignMovementTask(PhysicalObject *pObject);
	virtual void							FromBlock(Block *pBlock);
	virtual const char						*GetClassName() const			{ return "RandomMovementPlugin"; }
	virtual void							Print(int indent) const;
	virtual void							Refresh(const float &fDeltaTime);
	virtual void							SignalRemoved(PhysicalObject *pObject);
	virtual Block							*ToBlock();

protected:
	RandomMovementPlugin();
	virtual ~RandomMovementPlugin();

private:
	void									Init();

};

#endif