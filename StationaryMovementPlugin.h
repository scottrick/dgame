#ifndef _STATIONARY_MOVEMENT_PLUGIN_
#define _STATIONARY_MOVEMENT_PLUGIN_

#include "MovementPlugin.h"

class StationaryMovementPlugin : public MovementPlugin
{
public:

	static StationaryMovementPlugin			*GetPlugin();

	virtual void							AssignMovementTask(PhysicalObject *pObject);
	virtual void							FromBlock(Block *pBlock);
	virtual const char						*GetClassName() const			{ return "StationaryMovementPlugin"; }
	virtual void							Print(int indent) const;
	virtual void							Refresh(const float &fDeltaTime);
	virtual void							SignalRemoved(PhysicalObject *pObject);
	virtual Block							*ToBlock();

protected:
	StationaryMovementPlugin();
	virtual ~StationaryMovementPlugin();

private:
	void									Init();

};

#endif