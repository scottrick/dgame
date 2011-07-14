#ifndef _EVENT_
#define _EVENT_

#include "Unknown.h"

class Event : public Unknown
{
public:
	Event();

	virtual void				Execute()							= 0;	//executes the particular event
	virtual const char			*GetClassName() const				= 0;
	float						GetTimeTillExecution() const		{ return m_fTimeTillExecution; }
	virtual void				Print(int indent) const				= 0;
	void						SetTimeTillExecution(float fTime)	{ m_fTimeTillExecution = fTime; }
	void						Update(float fDeltaTime);

protected:
	virtual ~Event();

	float						m_fTimeTillExecution;		//time until this event occurs!

private:
	void						Init();

};

#endif