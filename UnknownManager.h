#ifndef _UNKNOWN_MANAGER_
#define _UNKNOWN_MANAGER_

#include "Manager.h"

#include <deque>
using namespace std;

class Unknown;
class UnknownBucket;

//#define _UNKNOWN_DEBUG  //enable unknown debug stuff!

/*
	UnknownManager has a list of every unknown created.  Used to help keep track of everything.
*/
class UnknownManager : public Manager
{
public:
	UnknownManager();
	~UnknownManager();

	void							AddUnknown(Unknown *pAdd);
	void							Cleanup();
	void							DebugInfoRender(const float &fRatio, const PosQuat &pq);
	void							DebugScreenRender();
	const char						*GetManagerName()							{ return "UnknownManager"; }
	void							Print();
	void							Refresh(const float &fDeltaTime);
	void							RemoveUnknown(Unknown *pRemove);
	void							SelectNext();
	void							SelectPrevious();

private:

	void							AddToBuckets(Unknown *pAdd);
	void							ClearBuckets();
	void							Init();
	void							UpdateBuckets();
	void							RemoveFromBuckets(Unknown *pRemove);
	
	bool							m_bChange;

#ifdef _UNKNOWN_DEBUG
	int								m_dwUpdateCounter;
	deque<UnknownBucket *>			m_vUnknownBuckets;
	deque<Unknown *>				m_vUnsorted; //unknowns which have not been put into their buckets yet
#endif

};

extern UnknownManager *gUnknownManager();

#endif