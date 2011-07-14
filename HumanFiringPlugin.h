#ifndef _HUMAN_FIRING_PLUGIN_
#define _HUMAN_FIRING_PLUGIN_

#include "FiringPlugin.h"

#include <vector>
using namespace std;

class PhysicalObject;

/*
	Special case firing plugin.  Unlike other ControlPlugins, human control plugins do NOT
	use tasks to dictate actions.  They use their refresh function to look for input and then 
	trigger fire actions on the physical objects they control.
*/
class HumanFiringPlugin : public FiringPlugin
{
public:
	static HumanFiringPlugin				*GetPlugin();

	void									AddControlledObject(PhysicalObject *pNewObject);
	virtual void							AssignFiringTask(PhysicalObject *pObject);
	virtual void							FromBlock(Block *pBlock);
	virtual const char						*GetClassName() const			{ return "HumanFiringPlugin"; }
	virtual void							Print(int indent) const;
	virtual void							Refresh(const float &fDeltaTime);
	virtual void							RemoveControlledObject(PhysicalObject *pRemove);
	virtual void							SignalRemoved(PhysicalObject *pObject);
	virtual Block							*ToBlock();

protected:
	HumanFiringPlugin();
	virtual ~HumanFiringPlugin();

	vector<PhysicalObject *>				m_vControlledObjects;

private:
	void									ClearControlledObjects();
	void									Init();

};

#endif