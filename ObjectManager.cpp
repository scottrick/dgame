#include "ObjectManager.h"

#include "EventManager.h"
#include "Game.h"
#include "Node.h"
#include "Object.h"
#include "PosQuat.h"
#include "RemoveObjectEvent.h"

#define BOUNDARY_DISTANCE_SQUARED				(GAME_SIZE_X * 1.5f) * (GAME_SIZE_X * 1.5f) + (GAME_SIZE_Y * 1.5) * (GAME_SIZE_Y * 1.5f)

ObjectManager *gObjectManager()
{
	static ObjectManager theObjectManager;
	return &theObjectManager;
}

ObjectManager::ObjectManager()
{
	Init();
}

ObjectManager::~ObjectManager()
{
	Cleanup();
}

/*
	Since Adding and Removing from the ObjectManager is *NOT* handled automatically (unlike Unknowns)
	the ObjectManager does AddRef and Release them.
*/
void ObjectManager::AddObject(Object *pAdd)
{
	if (pAdd)
	{	
		m_vObjects.push_back(pAdd);
		pAdd->AddRef();
	}
}

void ObjectManager::CheckObjectBoundaries()
{ //objects are considered out of bounds if they are more than 1 gamesize off the screen!
	static int dwLastChecked = 0;
	dwLastChecked++;
	if (dwLastChecked % 25 != 0)
	{ //don't always do this check
		return;
	}

	list<Object *>::iterator iter;
	for (iter = m_vObjects.begin(); iter != m_vObjects.end(); iter++)
	{
		Object *pObject = *iter;
		if (pObject && pObject->GetNode())
		{
			Node *pNode = pObject->GetNode();

			float fDistanceSquared	= pNode->m_PosQuat.pos.x * pNode->m_PosQuat.pos.x
									+ pNode->m_PosQuat.pos.y * pNode->m_PosQuat.pos.y;

			if (fDistanceSquared > BOUNDARY_DISTANCE_SQUARED)
			{
				RemoveObjectEvent *pNewRemoveEvent = new RemoveObjectEvent();
				pNewRemoveEvent->SetObject(pObject);
				gEventManager()->AddEvent(pNewRemoveEvent);
				pNewRemoveEvent->Release();				
			}
		}
	}
}

void ObjectManager::Cleanup()
{

}

void ObjectManager::DebugInfoRender(const float &fRatio, const PosQuat &pq)
{
	//nothing at this time
}

void ObjectManager::DebugScreenRender()
{
	
}

/*
Object *ObjectManager::GetObject(unsigned int dwIndex)
{
	if (dwIndex < m_vObjects.size())
	{
		return m_vObjects.at(dwIndex);
	}

	return 0;
}
*/

void ObjectManager::Init()
{

}

void ObjectManager::Print()
{
	printf("ObjectManager Summary\n");

	unsigned int dwPadding	= 0;
	unsigned int dwLength	= 0;
	unsigned int dwSpace	= 25;

	list<Object *>::iterator iter;
	for (iter = m_vObjects.begin(); iter != m_vObjects.end(); iter++)
	{
		dwLength = (unsigned int)strlen((*iter)->GetClassName());
		if (dwLength >= dwSpace)
		{
			dwPadding = 1;
		}
		else
		{
			dwPadding = dwSpace - dwLength;
		}
		
		printf("  %s%*s 0x%p  RefCount %u\n", (*iter)->GetClassName(), dwPadding, "", *iter, (*iter)->GetRefCount());
	}
}

void ObjectManager::Refresh(const float &fDeltaTime)
{

}

void ObjectManager::RemoveObject(Object *pRemove)
{
	if (pRemove)
	{
		list<Object *>::iterator iter;
		for (iter = m_vObjects.begin(); iter != m_vObjects.end(); iter++)
		{
			if ((*iter) == pRemove)
			{
				//found the unknown to remove
				(*iter)->Release();
				m_vObjects.erase(iter);
				break;
			}
		}
	}
}

void ObjectManager::SelectNext()
{
	if ((m_dwSelectedIndex + 1) < m_vObjects.size())
	{
		m_dwSelectedIndex++;
	}
}

void ObjectManager::SelectPrevious()
{
	if (m_dwSelectedIndex != 0)
	{
		m_dwSelectedIndex--;
	}
}