#include "JumpMovementTask.h"

#include "PhysicalObject.h"
#include "PosQuat.h"
#include "MovementWaypoint.h"
#include "Node.h"

JumpMovementTask::JumpMovementTask()
{
	Init();
}

JumpMovementTask::~JumpMovementTask()
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

void JumpMovementTask::AddMovementWaypoint(MovementWaypoint *pWaypoint)
{
	if (pWaypoint)
	{
		pWaypoint->AddRef();
		m_vMovementWaypoints.push_back(pWaypoint);
	}
}

void JumpMovementTask::ApplyTask(PhysicalObject *pObject, float fDeltaTime)
{
	if (pObject)
	{
		if (m_fCurrentDelay > 0.0f)
		{
			m_fCurrentDelay -= fDeltaTime;
		}
		else
		{
			MovementWaypoint *pCurrent = *(m_vMovementWaypoints.begin());

			//automatically move to the destination
			pObject->GetNode()->m_PosQuat.pos = pCurrent->vDest;

			//remove waypoint from list
			m_vMovementWaypoints.erase(m_vMovementWaypoints.begin());

			if (m_bRepeating)
			{ //reset the delay and add waypoint to the end of list
				m_fCurrentDelay = m_fDelay;
				m_vMovementWaypoints.push_back(pCurrent);
			}
			else
			{ //otherwise just release it...
				pCurrent->Release();
			}
		}

		if (m_vMovementWaypoints.size() <= 0)
		{
			m_bTaskComplete = true;
		}
	}
}

void JumpMovementTask::Init()
{
	m_fCurrentDelay	= 0.0f;
	m_fDelay		= 0.0f;
}	

void JumpMovementTask::Print(int indent) const
{
	printf("%*sJumpMovementTask 0x%p (%s)\n", indent, "", this);
}

void JumpMovementTask::SetDelay(float fDelay)
{
	m_fDelay			= fDelay;  
	m_fCurrentDelay		= m_fDelay; 
}