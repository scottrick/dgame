#include "BlindFiringTask.h"

#include "Game.h"
#include "Matrix44.h"
#include "Node.h"
#include "PhysicalObject.h"
#include "Player.h"
#include "PosQuat.h"

#include <iostream>
using namespace std;

BlindFiringTask::BlindFiringTask()
{
	Init();
}

BlindFiringTask::~BlindFiringTask()
{

}

void BlindFiringTask::ApplyTask(PhysicalObject *pObject, float fDeltaTime)
{
	if (pObject && gGame()->GetPlayer()->GetPlayerObject()->IsAlive())
	{
		if ((pObject->GetWeaponOne()) && (pObject->GetWeaponOne()->IsReady()))
		{ //fire weapon one!
			//calculate aim
			Matrix44 mat = pObject->GetNode()->m_PosQuat.quat.ToMatrix();
			VECTOR3 vAim(mat.m[4] * -1.0f, mat.m[5] * -1.0f, mat.m[6] * -1.0f);
			
			pObject->GetWeaponOne()->SetAim(vAim);
			pObject->GetWeaponOne()->SetOwner(pObject);

			//then fire
			pObject->GetWeaponOne()->Fire();
		}

		if ((pObject->GetWeaponTwo()) && (pObject->GetWeaponTwo()->IsReady()))
		{ //fire weapon two!

		}
	}
	/*
	if (pObject && m_pTarget)
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
	*/
}

void BlindFiringTask::Init()
{

}

void BlindFiringTask::Print(int indent) const
{
	printf("%*sBlindFiringTask 0x%p (%s)\n", indent, "", this);
}