#ifndef _IDLE_FIRING_TASK_
#define _IDLE_FIRING_TASK_

#include "FiringTask.h"

class PhysicalObject;

class IdleFiringTask : public FiringTask
{
public:
	IdleFiringTask();

	virtual void						ApplyTask(PhysicalObject *pObject, float fDeltaTime);
	virtual const char					*GetClassName() const			{ return "IdleFiringTask"; }
	virtual void						Print(int indent) const;

protected:
	virtual ~IdleFiringTask();

private:
	void								Init();

};

#endif