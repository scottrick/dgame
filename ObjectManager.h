#ifndef _OBJECT_MANAGER_
#define _OBJECT_MANAGER_

#include "Manager.h"

#include <list>
using namespace std;

class Object;

/*
	ObjectManager has a list of Objects which are to be refreshed.
*/

class ObjectManager : public Manager 
{
public:
	ObjectManager();
	~ObjectManager();

	void							AddObject(Object *pAdd);
	void							CheckObjectBoundaries();
	void							Cleanup();
	void							DebugInfoRender(const float &fRatio, const PosQuat &pq);
	void							DebugScreenRender();
	const char						*GetManagerName()					{ return "ObjectManager"; }
	unsigned int					GetNumObjects() const				{ return m_vObjects.size(); }
	list<Object *>::const_iterator	GetObjectIteratorBegin() const		{ return m_vObjects.begin(); }
	list<Object *>::const_iterator	GetObjectIteratorEnd() const		{ return m_vObjects.end(); }
	void							Print();
	void							Refresh(const float &fDeltaTime);
	void							RemoveObject(Object *pRemove);
	void							SelectNext();
	void							SelectPrevious();

private:
	
	void							Init();
	
	list<Object *>					m_vObjects;
	
};

extern ObjectManager *gObjectManager();

#endif