#ifndef _HUMAN_MOVEMENT_PLUGIN_
#define _HUMAN_MOVEMENT_PLUGIN_

#include "MovementPlugin.h"

#include <vector>
using namespace std;

class PhysicalObject;

/*
	Special case movement plugin.  Unlike other ControlPlugins, human control plugins do NOT
	use tasks to dictate actions.  They use their refresh function to look for input and then 
	move the physical objects they hold.
*/
class HumanMovementPlugin : public MovementPlugin
{
public:

	static HumanMovementPlugin				*GetPlugin();

	void									AddControlledObject(PhysicalObject *pNewObject);
	virtual void							AssignMovementTask(PhysicalObject *pObject);
	virtual void							FromBlock(Block *pBlock);
	virtual const char						*GetClassName() const			{ return "HumanMovementPlugin"; }
	virtual void							Print(int indent) const;
	virtual void							Refresh(const float &fDeltaTime);
	virtual void							RemoveControlledObject(PhysicalObject *pRemove);
	virtual void							SignalRemoved(PhysicalObject *pObject);
	virtual Block							*ToBlock();

protected:
	HumanMovementPlugin();
	virtual ~HumanMovementPlugin();

	float									m_fAcceleration;
	float									m_fSpeed;

	vector<PhysicalObject *>				m_vControlledObjects;

private:
	void									ClearControlledObjects();
	void									Init();

};

#endif