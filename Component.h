#ifndef _COMPONENT_
#define _COMPONENT_

#include "Object.h"

class PhysicalObject;

/*
	A Component are the "items" that you can customize your ship with, and help give 
	variety to enemy ships.
*/
class Component : public Object
{
public:
	Component();

	void							Clone(Component *pClone);
	PhysicalObject					*GetOwner() const						{ return m_pOwner; }
	void							SetOwner(PhysicalObject *pObject);

protected:
	virtual ~Component();

	PhysicalObject					*m_pOwner;		//owner of this component!  DO NOT ADDREF!

private:
	void							Init();

};

#endif