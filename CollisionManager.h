#ifndef _COLLISION_MANAGER_
#define _COLLISION_MANAGER_

#include "Manager.h"

#include <list>
#include <vector>
using namespace std;

class PhysicalObject;

#define		COLLISION_CHECKS_PER_SECOND			25.0f
#define		SECONDS_PER_COLLISION_CHECK			1.0f / COLLISION_CHECKS_PER_SECOND

class Collision
{
public:
	Collision()						{ m_pObjectOne = 0; m_pObjectTwo = 0; }
	~Collision()					{}

	PhysicalObject					*m_pObjectOne;
	PhysicalObject					*m_pObjectTwo;
};

class CollisionBucket
{
public: 
	CollisionBucket();
	~CollisionBucket();

	void							ClearContents();

	list<PhysicalObject *>			m_vContents;

	float							m_dwX;
	float							m_dwY;
};

class CollisionManager : public Manager
{
public:

	CollisionManager();
	~CollisionManager();
		
	void								Cleanup();
	void								DebugInfoRender(const float &fRatio, const PosQuat &pq);
	void								DebugScreenRender();
	const char							*GetManagerName()				{ return "CollisionManager"; }
	void								Refresh(const float &fDeltaTime);
	void								SelectNext();
	void								SelectPrevious();
	
protected:

	bool								AddPhysicalObjectToBucket(PhysicalObject *pObject, CollisionBucket *pBucket);
	CollisionBucket						*GetBucket(int x, int y);	

private:

	void								ClearBuckets();
	void								FindCollisions(CollisionBucket *pBucket);
	void								Init();
	void								ResolveCollisions();
	void								SortIntoBuckets();

	vector<CollisionBucket *>			m_vBuckets;

	float								m_fBucketSize;
	float								m_fTimeTillNextCheck;

};

extern CollisionManager *gCollisionManager();

#endif