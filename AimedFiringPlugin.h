#ifndef _AIMED_FIRING_PLUGIN_
#define _AIMED_FIRING_PLUGIN_

#include "FiringPlugin.h"

class AimedFiringPlugin : public FiringPlugin
{
public:
	static AimedFiringPlugin				*GetPlugin();

	virtual void							AssignFiringTask(PhysicalObject *pObject);
	virtual void							FromBlock(Block *pBlock);
	virtual const char						*GetClassName() const			{ return "AimedFiringPlugin"; }
	virtual void							Print(int indent) const;
	virtual void							Refresh(const float &fDeltaTime);
	virtual void							SignalRemoved(PhysicalObject *pObject);
	virtual Block							*ToBlock();

protected:
	AimedFiringPlugin();
	virtual ~AimedFiringPlugin();

private:
	void									Init();

};

#endif