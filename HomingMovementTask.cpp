#include "HomingMovementTask.h"

#include "Node.h"
#include "PhysicalObject.h"
#include "PosQuat.h"

#include <iostream>
using namespace std;

HomingMovementTask::HomingMovementTask()
{
	Init();
}

HomingMovementTask::~HomingMovementTask()
{
	SetTarget(0);
}

void HomingMovementTask::ApplyTask(PhysicalObject *pObject, float fDeltaTime)
{
	//printf("applying homing task...\n");
	if (pObject && m_pTarget)
	{
		VECTOR3 vDir;
		vDir.x = m_pTarget->GetNode()->m_PosQuat.pos.x - pObject->GetNode()->m_PosQuat.pos.x;
		vDir.y = m_pTarget->GetNode()->m_PosQuat.pos.y - pObject->GetNode()->m_PosQuat.pos.y;
		pObject->SetAccelerationDir(vDir);
		pObject->SetThrottle(1.0f);


		//pSegment->dwTimeLeft	-= dwDeltaTime;

		//m_pTarget->GetNode()->m_PosQuat.

		//pObject->SetAcceleration(pSegment->vAcceleration);
		//pObject->SetVelocity(pSegment->vVelocity);

		/*
		if (pSegment->dwTimeLeft <= 0)
		{
			m_vMoveSegments.erase(m_vMoveSegments.begin());
			pSegment->Release();

			if (m_vMoveSegments.size() <= 0) 
			{
				m_bTaskComplete = true;
			}
		}
		*/
	}
}

void HomingMovementTask::Init()
{
	m_pTarget = 0;
}

void HomingMovementTask::Print(int indent) const
{
	printf("%*sHomingMovementTask 0x%p (%s)\n", indent, "", this);
}

void HomingMovementTask::SetTarget(PhysicalObject *pObject)
{
	if (m_pTarget != pObject)
	{
		if (m_pTarget)
		{
			m_pTarget->Release();
			m_pTarget = 0;
		}

		if (pObject)
		{
			m_pTarget = pObject;
			m_pTarget->AddRef();
		}
	}
}