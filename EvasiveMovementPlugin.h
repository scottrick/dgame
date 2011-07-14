#ifndef _EVASIVE_MOVEMENT_PLUGIN_
#define _EVASIVE_MOVEMENT_PLUGIN_

#include "MovementPlugin.h"

class EvasiveMovementPlugin : public MovementPlugin
{
public:
	
	static EvasiveMovementPlugin				*GetPlugin();

	virtual void							AssignMovementTask(PhysicalObject *pObject);
	virtual void							FromBlock(Block *pBlock);
	virtual const char						*GetClassName() const			{ return "EvasiveMovementPlugin"; }
	virtual void							Print(int indent) const;
	virtual void							Refresh(const float &fDeltaTime);
	virtual void							SignalRemoved(PhysicalObject *pObject);
	virtual Block							*ToBlock();

protected:
	EvasiveMovementPlugin();
	virtual ~EvasiveMovementPlugin();

private:
	void									Init();

	static float							m_fEvasiveDistanceSquared;

};

#endif