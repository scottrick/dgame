#include "Weapon.h"

#include "EventManager.h"
#include "Matrix44.h"
#include "Node.h"
#include "Projectile.h"
#include "TestFireEvent.h"
#include "utility.h"

Weapon::Weapon()
{
	Init();
}

Weapon::~Weapon()
{

}

void Weapon::Clone(Weapon *pClone)
{
	Component::Clone(pClone);

	SetAcceleration(pClone->GetAcceleration());
	SetCooldown(pClone->GetCooldown());
	SetSpeed(pClone->GetSpeed());
	SetDamage(pClone->GetDamage());
}

void Weapon::Fire()
{
	if (m_fCurrentCooldown <= 0.0f)
	{
		m_fCurrentCooldown = m_fCooldown;

		PhysicalObject *pOwner = GetOwner();
		
		if (pOwner)
		{
			Node			*pSourceNode = pOwner->GetNode();
			
			if (pSourceNode)
			{
				Projectile *pNewProjectile = new Projectile(GetNode());
				VECTOR3 vDirection;

				Matrix44 mat = pSourceNode->m_PosQuat.quat.ToMatrix();

				float yDisp = -(pSourceNode->m_vScale.y / 2.0f + pNewProjectile->GetNode()->m_vScale.y / 2.0f);

				if (pSourceNode->GetNodeFlags() & NODE_RENDER_UPSIDEDOWN)
				{
					yDisp *= -1.0f;
				}

				pNewProjectile->GetNode()->m_PosQuat.pos.x = mat.m[4] * yDisp + pSourceNode->m_PosQuat.pos.x;
				pNewProjectile->GetNode()->m_PosQuat.pos.y = mat.m[5] * yDisp + pSourceNode->m_PosQuat.pos.y;
				pNewProjectile->GetNode()->m_PosQuat.pos.z = mat.m[6] * yDisp + pSourceNode->m_PosQuat.pos.z;

				//calculate actual firing direction based on aim, and weapon spread
				int dwRandom = GenerateRandomInt(9);
				float fSpread = 0.0f;
				if (dwRandom < 7)
				{
					fSpread = GenerateRandomFloat(0.05f);
				}
				else if (dwRandom < 9)
				{
					fSpread = GenerateRandomFloat(0.15f) + 0.05f;
				}
				else
				{
					fSpread = GenerateRandomFloat(0.30f) + 0.2f;
				}
				
				if (GenerateRandomInt(1))
				{
					fSpread = -fSpread;
				}
				
				fSpread = fSpread * m_fFiringSpread;
				float fCos = cos(fSpread);
				float fSin = sin(fSpread);
				
				//vDirection.x = fCos * m_vAim.x + fSin * m_vAim.x;
				//vDirection.y = fCos * m_vAim.y - fSin * m_vAim.y;
				vDirection.x = fCos * m_vAim.x - fSin * m_vAim.y;
				vDirection.y = fSin * m_vAim.x + fCos * m_vAim.y;
				vDirection.z = 0.0f;

				NormalizeVECTOR3(vDirection);

				pNewProjectile->GetNode()->m_PosQuat.pos.z = pSourceNode->m_PosQuat.pos.z;
				pNewProjectile->SetMaxSpeed(m_fSpeed);

				pNewProjectile->SetVelocity(VECTOR3(m_fSpeed * vDirection.x, m_fSpeed * vDirection.y, m_fSpeed * vDirection.z));
				pNewProjectile->SetCreator(pOwner);
				pNewProjectile->SetWeapon(this);

				Quat *quat = &(pNewProjectile->GetNode()->m_PosQuat.quat);
				Quat rotation;
				rotation.CreateFromRotationRADIANS(fSpread, 0.0f, 0.0f, 1.0f);
				*quat = rotation * *quat;

				TestFireEvent *pTestFireEvent = new TestFireEvent();
				pTestFireEvent->SetProjectile(pNewProjectile);
				gEventManager()->AddEvent(pTestFireEvent);
				pTestFireEvent->Release();
				pNewProjectile->Release();	
			}
		}
	}
}

void Weapon::FromBlock(Block *pBlock)
{

}

void Weapon::Init()
{
	m_fAcceleration			= 0.0f;
	float fCooldown			= 45.0f / 1000.0f;
	SetCooldown(fCooldown);
	m_fSpeed				= 17.0f;
	m_fFiringSpread			= PI / 16.0f;

	m_Damage.m_fEnergyDamage	= 0.0f;
	m_Damage.m_fPhysicalDamage	= 1.0f;

	Node *pNode			= new Node();
	switch (GenerateRandomInt(3))
	{
		case 0:
			pNode->SetVisual("data\\visuals\\p_blue.3ds");
			pNode->m_vScale.x	= 0.20f;
			pNode->m_vScale.y	= 0.20f;
			pNode->m_vScale.z	= 0.20f;
			break;
		case 1:
			pNode->SetVisual("data\\visuals\\p_green.3ds");
			pNode->m_vScale.x	= 0.25f;
			pNode->m_vScale.y	= 0.25f;
			pNode->m_vScale.z	= 0.25f;
			break;
		case 2:
			pNode->SetVisual("data\\visuals\\p_orng.3ds");
			pNode->m_vScale.x	= 0.30f;
			pNode->m_vScale.y	= 0.30f;
			pNode->m_vScale.z	= 0.30f;
			break;
		case 3:
			pNode->SetVisual("data\\visuals\\p_red.3ds");
			pNode->m_vScale.x	= 0.15f;
			pNode->m_vScale.y	= 0.15f;
			pNode->m_vScale.z	= 0.15f;
			break;

	}

	SetNode(pNode);
	pNode->Release();
}

bool Weapon::IsReady() const
{
	if (m_fCurrentCooldown <= 0.0f)
	{
		return true;
	}
	else 
	{
		return false;
	}
}

void Weapon::Refresh(const float &fDeltaTime)
{
	if (m_fCurrentCooldown > 0.0f)
	{
		m_fCurrentCooldown -= fDeltaTime;

		if (m_fCurrentCooldown < 0.0f) 
		{
			m_fCurrentCooldown = 0.0f;
		}	
	}
}

void Weapon::SetAim(VECTOR3 vAim)
{
	m_vAim = vAim;
	NormalizeVECTOR3(m_vAim);
}

void Weapon::SetCooldown(float fCooldown)
{
	m_fCooldown = fCooldown;
	m_fCurrentCooldown = GenerateRandomFloat(m_fCooldown);
}

Block *Weapon::ToBlock()
{
	return 0;
}