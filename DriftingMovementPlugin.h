#ifndef _DRIFTING_MOVEMENT_PLUGIN_
#define _DRIFTING_MOVEMENT_PLUGIN_

#include "MovementPlugin.h"

class DriftingMovementPlugin : public MovementPlugin
{
public:

	static DriftingMovementPlugin			*GetPlugin();

	virtual void							AssignMovementTask(PhysicalObject *pObject);
	virtual void							FromBlock(Block *pBlock);
	virtual const char						*GetClassName() const			{ return "DriftingMovementPlugin"; }
	virtual void							Print(int indent) const;
	virtual void							Refresh(const float &fDeltaTime);
	virtual void							SignalRemoved(PhysicalObject *pObject);
	virtual Block							*ToBlock();

protected:
	DriftingMovementPlugin();
	virtual ~DriftingMovementPlugin();

private:
	void									Init();

};

#endif