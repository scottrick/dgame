#ifndef _HOMING_MOVEMENT_TASK_
#define _HOMING_MOVEMENT_TASK_

#include "MovementTask.h"

class PhysicalObject;

/*
	Describes the movement task of trying to collide with a physical object.
*/
class HomingMovementTask : public MovementTask
{
public:
	HomingMovementTask();

	virtual void						ApplyTask(PhysicalObject *pObject, float fDeltaTime);
	virtual const char					*GetClassName() const			{ return "HomingMovementTask"; }
	virtual void						Print(int indent) const;
	void								SetTarget(PhysicalObject *pObject);

protected:
	virtual ~HomingMovementTask();

	PhysicalObject						*m_pTarget;

private:
	void								Init();

};

#endif