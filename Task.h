#ifndef _TASK_
#define _TASK_

#include "Unknown.h"

class Task : public Unknown
{
public:

	Task()											{ m_bTaskComplete = false; };

	bool					IsComplete() const		{ return m_bTaskComplete; }

protected:
	virtual ~Task()			{};

	bool					m_bTaskComplete;

};

#endif