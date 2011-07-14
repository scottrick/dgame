#include "StandardMovementTask.h"

#include "MovementWaypoint.h"
#include "Node.h"
#include "PhysicalObject.h"
#include "PosQuat.h"

StandardMovementTask::StandardMovementTask()
{
	Init();
}

StandardMovementTask::~StandardMovementTask()
{
	vector<MovementWaypoint *>::iterator iter = m_vMovementWaypoints.begin();
	while (iter != m_vMovementWaypoints.end())
	{
		MovementWaypoint *pWaypoint = *iter;
		if (pWaypoint)
		{
			pWaypoint->Release();
			pWaypoint = 0;
		}

		iter++;
	}

	m_vMovementWaypoints.clear();
}

void StandardMovementTask::AddMovementWaypoint(MovementWaypoint *pWaypoint)
{
	if (pWaypoint)
	{
		pWaypoint->AddRef();
		m_vMovementWaypoints.push_back(pWaypoint);
	}
}

void StandardMovementTask::ApplyTask(PhysicalObject *pObject, float fDeltaTime)
{
	if (pObject)
	{
		MovementWaypoint *pWaypoint = *(m_vMovementWaypoints.begin());

		//calculate new hack distance
		float fX = pWaypoint->vDest.x - pObject->GetNode()->m_PosQuat.pos.x;
		float fY = pWaypoint->vDest.y - pObject->GetNode()->m_PosQuat.pos.y;
		float fZ = pWaypoint->vDest.z - pObject->GetNode()->m_PosQuat.pos.z;
		float fNewDistanceSquared = fX * fX + fY * fY + fZ * fZ;

		if (pWaypoint->fPreviousDistanceSquared < 0.0f) 
		{ //haven't started flying towards the waypoint yet, so start!
			pWaypoint->fPreviousDistanceSquared = fNewDistanceSquared;
		}

		if (fNewDistanceSquared <= pWaypoint->fPreviousDistanceSquared)
		{
			pObject->SetAccelerationDir(pWaypoint->vDest - pObject->GetNode()->m_PosQuat.pos);
			pObject->SetThrottle(1.0f);
			pWaypoint->fPreviousDistanceSquared = fNewDistanceSquared;
		}
		else
		{ //we have reached our destination
			m_vMovementWaypoints.erase(m_vMovementWaypoints.begin());

			if (m_bRepeating)
			{ //reset the distance squared and then add it to the back of the waypoint list!
				pWaypoint->fPreviousDistanceSquared = -1.0f;
				m_vMovementWaypoints.push_back(pWaypoint);
			}
			else
			{ //otherwise just release it...
				pWaypoint->Release();
			}
		}
	
		if (m_vMovementWaypoints.size() <= 0)
		{
			m_bTaskComplete = true;
		}
	}
}

void StandardMovementTask::Init()
{
	m_bRepeating = false;
}

void StandardMovementTask::Print(int indent) const
{
	printf("%*sStandardMovementTask 0x%p (%s)\n", indent, "", this);
}
