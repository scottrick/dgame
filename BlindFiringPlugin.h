#ifndef _BLIND_FIRING_PLUGIN_
#define _BLIND_FIRING_PLUGIN_

#include "FiringPlugin.h"

class BlindFiringPlugin : public FiringPlugin
{
public:
	static BlindFiringPlugin				*GetPlugin();

	virtual void							AssignFiringTask(PhysicalObject *pObject);
	virtual void							FromBlock(Block *pBlock);
	virtual const char						*GetClassName() const			{ return "BlindFiringPlugin"; }
	virtual void							Print(int indent) const;
	virtual void							Refresh(const float &fDeltaTime);
	virtual void							SignalRemoved(PhysicalObject *pObject);
	virtual Block							*ToBlock();

protected:
	BlindFiringPlugin();
	virtual ~BlindFiringPlugin();

private:
	void									Init();

};

#endif