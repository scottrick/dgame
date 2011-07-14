#ifndef _REMOVE_OBJECT_EVENT_
#define _REMOVE_OBJECT_EVENT_

#include "Event.h"

class Object;
class Scene;

class RemoveObjectEvent : public Event
{
public:
	RemoveObjectEvent();

	virtual void						Execute();
	virtual const char					*GetClassName() const				{ return "RemoveObjectEvent"; }
	virtual void						Print(int indent) const;

	void								SetObject(Object *pObject);

protected:
	virtual ~RemoveObjectEvent();

	Object								*m_pObject;

private:
	void								Init();

};

#endif