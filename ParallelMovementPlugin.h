#ifndef _PARALLEL_MOVEMENT_PLUGIN_
#define _PARALLEL_MOVEMENT_PLUGIN_

#include "MovementPlugin.h"

class ParallelMovementPlugin : public MovementPlugin
{
public:
	
	static ParallelMovementPlugin			*GetPlugin();

	virtual void							AssignMovementTask(PhysicalObject *pObject);
	virtual void							FromBlock(Block *pBlock);
	virtual const char						*GetClassName() const			{ return "ParallelMovementPlugin"; }
	virtual void							Print(int indent) const;
	virtual void							Refresh(const float &fDeltaTime);
	virtual void							SignalRemoved(PhysicalObject *pObject);
	virtual Block							*ToBlock();

protected:
	ParallelMovementPlugin();
	virtual ~ParallelMovementPlugin();

private:
	void									Init();

};

#endif