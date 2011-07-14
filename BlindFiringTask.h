#ifndef _BLIND_FIRING_TASK_
#define _BLIND_FIRING_TASK_

#include "FiringTask.h"

class PhysicalObject;

class BlindFiringTask : public FiringTask
{
public:
	BlindFiringTask();

	virtual void						ApplyTask(PhysicalObject *pObject, float fDeltaTime);
	virtual const char					*GetClassName() const			{ return "BlindFiringTask"; }
	virtual void						Print(int indent) const;

protected:
	virtual ~BlindFiringTask();

private:
	void								Init();

};

#endif