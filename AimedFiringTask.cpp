#include "AimedFiringTask.h"

#include "Node.h"
#include "PhysicalObject.h"
#include "PosQuat.h"

#include <iostream>
using namespace std;

AimedFiringTask::AimedFiringTask()
{
	Init();
}

AimedFiringTask::~AimedFiringTask()
{
	SetTarget(0);
}

void AimedFiringTask::ApplyTask(PhysicalObject *pObject, float fDeltaTime)
{
	if (pObject && m_pTarget && m_pTarget->IsAlive())
	{ //fire the ship's weapons if they aren't on cooldown
		if ((pObject->GetWeaponOne()) && (pObject->GetWeaponOne()->IsReady()))
		{ //fire weapon one!
			//calculate aim
			VECTOR3 vAim;
			vAim.x = m_pTarget->GetNode()->m_PosQuat.pos.x - pObject->GetNode()->m_PosQuat.pos.x;
			vAim.y = m_pTarget->GetNode()->m_PosQuat.pos.y - pObject->GetNode()->m_PosQuat.pos.y;
			vAim.z = m_pTarget->GetNode()->m_PosQuat.pos.z - pObject->GetNode()->m_PosQuat.pos.z;
			pObject->GetWeaponOne()->SetAim(vAim);
			pObject->GetWeaponOne()->SetOwner(pObject);

			//then fire
			pObject->GetWeaponOne()->Fire();
		}

		if ((pObject->GetWeaponTwo()) && (pObject->GetWeaponTwo()->IsReady()))
		{ //fire weapon two!
			//calculate aim
			VECTOR3 vAim;
			vAim.x = m_pTarget->GetNode()->m_PosQuat.pos.x - pObject->GetNode()->m_PosQuat.pos.x;
			vAim.y = m_pTarget->GetNode()->m_PosQuat.pos.y - pObject->GetNode()->m_PosQuat.pos.y;
			vAim.z = m_pTarget->GetNode()->m_PosQuat.pos.z - pObject->GetNode()->m_PosQuat.pos.z;
			pObject->GetWeaponTwo()->SetAim(vAim);
			pObject->GetWeaponTwo()->SetOwner(pObject);

			//then fire!!!!
			pObject->GetWeaponTwo()->Fire();
		}
	}
}

void AimedFiringTask::Init()
{
	m_pTarget = 0;
}

void AimedFiringTask::Print(int indent) const
{
	printf("%*sAimedFiringTask 0x%p (%s)\n", indent, "", this);
}

void AimedFiringTask::SetTarget(PhysicalObject *pTarget)
{
	if (m_pTarget != pTarget)
	{
		if (m_pTarget)
		{
			m_pTarget->Release();
			m_pTarget = 0;
		}

		if (pTarget)
		{
			m_pTarget = pTarget;
			m_pTarget->AddRef();
		}
	}
}