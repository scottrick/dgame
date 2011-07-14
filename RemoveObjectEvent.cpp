#include "RemoveObjectEvent.h"

#include "Game.h"
#include "Object.h"

RemoveObjectEvent::RemoveObjectEvent()
{
	Init();
}

RemoveObjectEvent::~RemoveObjectEvent()
{
	SetObject(0);
}

void RemoveObjectEvent::Execute()
{
	assert(m_pObject);

	gGame()->RemoveObject(m_pObject);
	SetObject(0);
}

void RemoveObjectEvent::Init()
{
	m_pObject	= 0;
}

void RemoveObjectEvent::Print(int indent) const
{
	
}

void RemoveObjectEvent::SetObject(Object *pObject)
{
	if (m_pObject != pObject)
	{
		if (m_pObject)
		{
			m_pObject->Release();
			m_pObject = 0;
		}

		if (pObject)
		{
			m_pObject = pObject;
			m_pObject->AddRef();
		}
	}
}