#ifndef _HOMING_MOVEMENT_PLUGIN_
#define _HOMING_MOVEMENT_PLUGIN_

#include "MovementPlugin.h"

class HomingMovementPlugin : public MovementPlugin
{
public:
	
	static HomingMovementPlugin				*GetPlugin();

	virtual void							AssignMovementTask(PhysicalObject *pObject);
	virtual void							FromBlock(Block *pBlock);
	virtual const char						*GetClassName() const			{ return "HomingMovementPlugin"; }
	virtual void							Print(int indent) const;
	virtual void							Refresh(const float &fDeltaTime);
	virtual void							SignalRemoved(PhysicalObject *pObject);
	virtual Block							*ToBlock();

protected:
	HomingMovementPlugin();
	virtual ~HomingMovementPlugin();

private:
	void									Init();

};

#endif