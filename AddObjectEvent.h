#ifndef _ADD_OBJECT_EVENT_
#define _ADD_OBJECT_EVENT_

#include "Event.h"

class Object;
class Scene;

class AddObjectEvent : public Event
{
public:
	AddObjectEvent();

	virtual void						Execute();
	virtual const char					*GetClassName() const				{ return "AddObjectEvent"; }
	virtual void						Print(int indent) const;

	void								SetObject(Object *pObject, Scene *pDestScene);

protected:
	virtual ~AddObjectEvent();

	Object								*m_pObject;
	Scene								*m_pDestScene;

private:
	void								Init();

};

#endif