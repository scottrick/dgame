/*
#ifndef _CONVENTIONAL_MOVEMENT_PLUGIN_
#define _CONVENTIONAL_MOVEMENT_PLUGIN_

#include "MovementPlugin.h"

class StandardMovementPlugin : public MovementPlugin
{
public:

	static StandardMovementPlugin			*GetPlugin();

	virtual void							AssignMovementTask(PhysicalObject *pObject);
	virtual void							FromBlock(Block *pBlock);
	virtual const char						*GetClassName() const			{ return "StandardMovementPlugin"; }
	virtual void							Print(int indent) const;
	virtual void							Refresh(const float &fDeltaTime);
	virtual void							SignalRemoved(PhysicalObject *pObject);
	virtual Block							*ToBlock();

protected:
	StandardMovementPlugin();
	virtual ~StandardMovementPlugin();

private:
	void									Init();

};

#endif
*/