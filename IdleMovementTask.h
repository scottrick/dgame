#ifndef _IDLE_MOVEMENT_TASK_
#define _IDLE_MOVEMENT_TASK_

#include "MovementTask.h"

/*
	Describes the movement task of being idle!
*/
class IdleMovementTask : public MovementTask
{
public:
	IdleMovementTask();

	virtual void						ApplyTask(PhysicalObject *pObject, float fDeltaTime);
	virtual const char					*GetClassName() const			{ return "IdleMovementTask"; }
	virtual void						Print(int indent) const;

protected:
	virtual ~IdleMovementTask();

private:
	void								Init();

};

#endif