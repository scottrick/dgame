#include "EventManager.h"

#include "Event.h"
#include "Object.h"
#include "RemoveObjectEvent.h"

EventManager *gEventManager()
{
	static EventManager theEventManager;
	return &theEventManager;
}

EventManager::EventManager()
{
	Init();
}

EventManager::~EventManager()
{
	Cleanup();
}	

void EventManager::AddEvent(Event *pAdd)
{
	if (pAdd)
	{ //add new events to temp list - this prevents the event list from changing size while its running
		m_vNewEvents.push_front(pAdd);
		pAdd->AddRef();
	}
}

void EventManager::Cleanup()
{
	list<Event *>::iterator		iter;
	Event						*pEvent			= 0;
	for (iter = m_vEvents.begin(); iter != m_vEvents.end(); iter++)
	{
		pEvent = *iter;
		if (pEvent)
		{
			pEvent->Release();
			pEvent = 0;
		}
	}
	m_vEvents.clear();
}

void EventManager::DebugInfoRender(const float &fRatio, const PosQuat &pq)
{

}

void EventManager::DebugScreenRender()
{

}

void EventManager::Init()
{
	
}	

void EventManager::MoveNewEventsToMainList()
{
	list<Event *>::iterator		iter;
	Event						*pEvent				= 0;

	for (iter = m_vNewEvents.begin(); iter != m_vNewEvents.end(); /* no update */ )
	{
		pEvent = *iter;
		if (pEvent)
		{ //move event to the main event list for execution
			m_vEvents.push_front(pEvent);
		}
		
		//remove the event
		iter = m_vNewEvents.erase(iter);
	}
}

void EventManager::Refresh(const float &fDeltaTime)
{
	MoveNewEventsToMainList();

	list<Event *>::iterator		iter;
	Event						*pEvent				= 0;

	for (iter = m_vEvents.begin(); iter != m_vEvents.end(); /* no update */ )
	{
		pEvent = *iter;
		if (pEvent)
		{
			pEvent->Update(fDeltaTime);

			if (pEvent->GetTimeTillExecution() <= 0)
			{ //it has been executed, so remove it from the list
				pEvent->Release();
				iter = m_vEvents.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		else
		{ // null element so remove
			iter = m_vEvents.erase(iter);
		}
	}	
}

void EventManager::SelectNext()
{

}

void EventManager::SelectPrevious()
{
	
}