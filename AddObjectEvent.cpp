#include "AddObjectEvent.h"

#include "Game.h"
#include "Object.h"
#include "Scene.h"

AddObjectEvent::AddObjectEvent()
{
	Init();
}

AddObjectEvent::~AddObjectEvent()
{
	SetObject(0, 0);
}

void AddObjectEvent::Execute()
{
	if (m_pObject)
	{
		if (m_pDestScene)
		{
			m_pDestScene->AddObject(m_pObject);
		}
		else
		{
			gGame()->GetCurrentScene()->AddObject(m_pObject);
		}
		
		SetObject(0, 0);
	}
}

void AddObjectEvent::Init()
{
	m_pDestScene	= 0;
	m_pObject		= 0;
}

void AddObjectEvent::Print(int indent) const
{
	
}

void AddObjectEvent::SetObject(Object *pObject, Scene *pDestScene)
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

	if (pDestScene != m_pDestScene)
	{
		if (m_pDestScene)
		{
			m_pDestScene->Release();
			m_pDestScene = 0;
		}

		if (pDestScene)
		{
			m_pDestScene = pDestScene;
			m_pDestScene->AddRef();
		}
	}
}