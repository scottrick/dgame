#ifndef _MOVEMENT_PLUGIN_
#define _MOVEMENT_PLUGIN_

#include "Object.h"

class PhysicalObject;

class MovementPlugin : public Object
{
public:

	virtual	void						AssignMovementTask(PhysicalObject *pObject)		= 0;
	virtual void						FromBlock(Block *pBlock)						= 0;
	virtual void						Print(int indent) const							= 0;
	virtual void						SignalRemoved(PhysicalObject *pObject)			= 0;
	virtual Block						*ToBlock()										= 0;

protected:
	MovementPlugin()					{};
	virtual ~MovementPlugin()			{};

private:

};

#endif