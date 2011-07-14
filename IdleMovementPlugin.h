#ifndef _IDLE_MOVEMENT_PLUGIN_
#define _IDLE_MOVEMENT_PLUGIN_

#include "MovementPlugin.h"

class IdleMovementPlugin : public MovementPlugin
{
public:

	static IdleMovementPlugin			*GetPlugin();

	virtual void							AssignMovementTask(PhysicalObject *pObject);
	virtual void							FromBlock(Block *pBlock);
	virtual const char						*GetClassName() const			{ return "IdleMovementPlugin"; }
	virtual void							Print(int indent) const;
	virtual void							Refresh(const float &fDeltaTime);
	virtual void							SignalRemoved(PhysicalObject *pObject);
	virtual Block							*ToBlock();

protected:
	IdleMovementPlugin();
	virtual ~IdleMovementPlugin();

private:
	void									Init();

};

#endif