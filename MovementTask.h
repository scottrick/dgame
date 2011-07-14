#ifndef _MOVEMENT_TASK_
#define _MOVEMENT_TASK_

#include "def.h"
#include "Task.h"

class PhysicalObject;

class MovementTask : public Task
{
public:

	MovementTask();

	virtual void				ApplyTask(PhysicalObject *pObject, float fDeltaTime)		= 0;

protected:
	virtual ~MovementTask()		{};

private:
	void						Init();

};

#endif