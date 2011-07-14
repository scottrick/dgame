#ifndef _JUMP_MOVEMENT_TASK_
#define _JUMP_MOVEMENT_TASK_

#include "MovementTask.h"

#include <vector>
using namespace std;

class MovementWaypoint;

/*
	Describes movement by jumping to the destination location
*/
class JumpMovementTask : public MovementTask
{
public:
	JumpMovementTask();

	void								AddMovementWaypoint(MovementWaypoint *pWaypoint);
	virtual void						ApplyTask(PhysicalObject *pObject, float fDeltaTime);
	virtual const char					*GetClassName() const			{ return "JumpMovementTask"; }
	virtual void						Print(int indent) const;
	void								SetDelay(float fDelay);
	void								SetRepeating(bool bRepeating)	{ m_bRepeating = bRepeating; }

protected:
	virtual ~JumpMovementTask();

	vector<MovementWaypoint *>			m_vMovementWaypoints;

	float								m_fCurrentDelay;
	float								m_fDelay;
	bool								m_bRepeating;

private:
	void								Init();

};

#endif