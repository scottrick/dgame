#ifndef _STANDARD_MOVEMENT_TASK_
#define _STANDARD_MOVEMENT_TASK_

#include "MovementTask.h"

#include <vector>
using namespace std;

class MovementWaypoint;

/*
	Describes a movement path for a physical object with one or more MoveWaypoints
*/
class StandardMovementTask : public MovementTask
{
public:
	StandardMovementTask();

	void								AddMovementWaypoint(MovementWaypoint *pWaypoint);
	virtual void						ApplyTask(PhysicalObject *pObject, float fDeltaTime);
	virtual const char					*GetClassName() const			{ return "StandardMovementTask"; }
	virtual void						Print(int indent) const;
	void								SetRepeating(bool bRepeating)	{ m_bRepeating = bRepeating; }

protected:
	virtual ~StandardMovementTask();

	vector<MovementWaypoint *>			m_vMovementWaypoints;

	bool								m_bRepeating;

private:
	void								Init();

};

#endif