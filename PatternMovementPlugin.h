#ifndef _PATTERN_MOVEMENT_PLUGIN_
#define _PATTERN_MOVEMENT_PLUGIN_

#include "MovementPlugin.h"

#include "MovementWaypoint.h"

#include <vector>
using namespace std;

#define NUMBER_SHIPS_PER_PATTERN		15

class PatternMovementPlugin : public MovementPlugin
{
public:
	
	static PatternMovementPlugin			*GetPlugin();

	virtual void							AssignMovementTask(PhysicalObject *pObject);
	virtual void							FromBlock(Block *pBlock);
	virtual const char						*GetClassName() const			{ return "PatternMovementPlugin"; }
	virtual void							Print(int indent) const;
	virtual void							Refresh(const float &fDeltaTime);
	virtual void							SignalRemoved(PhysicalObject *pObject);
	virtual Block							*ToBlock();

protected:
	PatternMovementPlugin();
	virtual ~PatternMovementPlugin();

	void									ClearPattern();
	void									GeneratePattern();

	vector<MovementWaypoint *>				m_vPattern;
	unsigned int							m_dwPatternAssignmentCount;

private:
	void									Init();

};

#endif