#ifndef _JUMP_MOVEMENT_PLUGIN_
#define _JUMP_MOVEMENT_PLUGIN_

#include "MovementPlugin.h"

class JumpMovementPlugin : public MovementPlugin
{
public:

	static JumpMovementPlugin				*GetPlugin();

	virtual void							AssignMovementTask(PhysicalObject *pObject);
	virtual void							FromBlock(Block *pBlock);
	virtual const char						*GetClassName() const			{ return "JumpMovementPlugin"; }
	virtual void							Print(int indent) const;
	virtual void							Refresh(const float &fDeltaTime);
	virtual void							SignalRemoved(PhysicalObject *pObject);
	virtual Block							*ToBlock();

protected:
	JumpMovementPlugin();
	virtual ~JumpMovementPlugin();

private:
	void									Init();

};

#endif