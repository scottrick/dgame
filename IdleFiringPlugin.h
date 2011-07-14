#ifndef _IDLE_FIRING_PLUGIN_
#define _IDLE_FIRING_PLUGIN_

#include "FiringPlugin.h"

class IdleFiringPlugin : public FiringPlugin
{
public:
	static IdleFiringPlugin					*GetPlugin();

	virtual void							AssignFiringTask(PhysicalObject *pObject);
	virtual void							FromBlock(Block *pBlock);
	virtual const char						*GetClassName() const			{ return "IdleFiringPlugin"; }
	virtual void							Print(int indent) const;
	virtual void							Refresh(const float &fDeltaTime);
	virtual void							SignalRemoved(PhysicalObject *pObject);
	virtual Block							*ToBlock();

protected:
	IdleFiringPlugin();
	virtual ~IdleFiringPlugin();

private:
	void									Init();

};

#endif