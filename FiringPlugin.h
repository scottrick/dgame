#ifndef _FIRING_PLUGIN_
#define _FIRING_PLUGIN_

#include "Object.h"

class PhysicalObject;

class FiringPlugin : public Object
{
public:

	virtual	void						AssignFiringTask(PhysicalObject *pObject)		= 0;
	virtual void						FromBlock(Block *pBlock)						= 0;
	virtual void						Print(int indent) const							= 0;
	virtual void						SignalRemoved(PhysicalObject *pObject)			= 0;
	virtual Block						*ToBlock()										= 0;

protected:
	FiringPlugin()						{};
	virtual ~FiringPlugin()				{};

private:
	

};

#endif