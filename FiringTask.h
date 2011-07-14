#ifndef _FIRING_TASK_
#define _FIRING_TASK_

#include "Task.h"

class PhysicalObject;

class FiringTask : public Task
{
public:
	FiringTask()					{};

	virtual void					ApplyTask(PhysicalObject *pObject, float fDeltaTime)		= 0;

protected:
	virtual ~FiringTask()			{};

private:


};

#endif