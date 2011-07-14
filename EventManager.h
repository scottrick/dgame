#ifndef _EVENT_MANAGER_
#define _EVENT_MANAGER_

#include "Manager.h"

#include <list>
using namespace std;

class Event;

/*
	EventManager has a list of Events which are scheduled to occur
*/

class EventManager : public Manager 
{
public:
	EventManager();
	~EventManager();

	void					AddEvent(Event *pAdd);
	void					Cleanup();
	void					DebugInfoRender(const float &fRatio, const PosQuat &pq);
	void					DebugScreenRender();
	const char				*GetManagerName()					{ return "EventManager"; }
	void					Print();
	void					Refresh(const float &fDeltaTime);
	void					RemoveEvent(Event *pRemove);
	void					SelectNext();
	void					SelectPrevious();

private:
	
	void					Init();
	void					MoveNewEventsToMainList();
	
	list<Event *>			m_vEvents;
	list<Event *>			m_vNewEvents;	//place to store new events before they are added ot the event vector..	
};

extern EventManager *gEventManager();

#endif