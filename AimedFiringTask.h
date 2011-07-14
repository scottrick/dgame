#ifndef _AIMED_FIRING_TASK_
#define _AIMED_FIRING_TASK_

#include "FiringTask.h"

class PhysicalObject;

class AimedFiringTask : public FiringTask
{
public:
	AimedFiringTask();

	virtual void						ApplyTask(PhysicalObject *pObject, float fDeltaTime);
	virtual const char					*GetClassName() const			{ return "AimedFiringTask"; }
	virtual void						Print(int indent) const;
	void								SetTarget(PhysicalObject *pTarget);

protected:
	virtual ~AimedFiringTask();

	PhysicalObject						*m_pTarget;

private:
	void								Init();

};

#endif