#include "Component.h"

#include "PhysicalObject.h"

Component::Component()
{
	Init();
}

Component::~Component()
{
	SetOwner(0);
}

void Component::Clone(Component *pClone)
{
	Object::Clone(pClone);

	SetOwner(pClone->GetOwner());
}

void Component::Init() 
{
	m_pOwner = 0;
}

/*
	DO NOT ADDREF the owner!  otherwise we will have a circular addref thingy where nothing gets released!
*/
void Component::SetOwner(PhysicalObject *pObject)
{
	if (m_pOwner != pObject)
	{
		if (m_pOwner)
		{
			m_pOwner = 0;
		}
		
		if (pObject)
		{
			m_pOwner = pObject;
		}
	}
}