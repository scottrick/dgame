#include "Projectile.h"

#include "AddObjectEvent.h"
#include "EventManager.h"
#include "Node.h"
#include "RemoveObjectEvent.h"
#include "SparkEffect.h"
#include "TestExplosionEffect.h"
#include "Weapon.h"

Projectile::Projectile()
{
	Init();
}

Projectile::Projectile(Node *pNode)
{
	Node *pNewNode			= new Node();
	pNewNode->SetVisual(pNode->GetVisual());
	pNewNode->m_vScale.x	= pNode->m_vScale.x;
	pNewNode->m_vScale.y	= pNode->m_vScale.y;
	pNewNode->m_vScale.z	= pNode->m_vScale.z;
	SetNode(pNewNode);
	pNewNode->Release();

	Init();
}

Projectile::~Projectile()
{
	SetCreator(0);
	SetWeapon(0);
}

void Projectile::Clone(Projectile *pClone)
{
	PhysicalObject::Clone(pClone);

	SetCreator(pClone->GetCreator());
	SetWeapon(pClone->GetWeapon());
}

void Projectile::Collide(PhysicalObject *pOtherObject)
{
	switch (pOtherObject->GetPhysicalObjectType())
	{

	case PhysicalObject::ePROJECTILE:  //don't collide with other projectiles originating from the same creator
		if (((Projectile *)pOtherObject)->GetCreator() != GetCreator())
		{
			//ApplyOnePhysicalDamage(((Projectile *)pOtherObject)->GetCreator());
			ApplyOnePhysicalDamage(0);
		}
		break;

	default:
		if (GetCreator() != pOtherObject)
		{
			//ApplyOnePhysicalDamage(pOtherObject);
			ApplyOnePhysicalDamage(0);
		}
		break;

	}
}

PhysicalObject *Projectile::Duplicate()
{
	Projectile *pDuplicate = new Projectile();
	pDuplicate->Clone(this);
	return pDuplicate;	
}

void Projectile::FromBlock(Block *pBlock)
{

}

Shockwave *Projectile::GetShockwave()
{
	return 0;
}

void Projectile::Init()
{
	SetPhysicalObjectType(PhysicalObject::ePROJECTILE);

	m_pCreator			= 0;
	m_pWeapon			= 0;

	if (!GetNode())
	{ //set some default node if none exists...
		Node *pNode			= new Node();
		pNode->SetVisual("data\\visuals\\laser.3ds");
		pNode->m_vScale.x	= 0.5f;
		pNode->m_vScale.y	= 0.5f;
		pNode->m_vScale.z	= 0.5f;
		SetNode(pNode);
		pNode->Release();
	}
}

void Projectile::OnDeath()
{
	SparkEffect *pSparkEffect = new SparkEffect(this->GetNode()->m_vScale.x, this->GetNode(), this->GetVelocity());
	AddObjectEvent *pAddSpark = new AddObjectEvent();
	pAddSpark->SetObject(pSparkEffect, this->GetScene());
	gEventManager()->AddEvent(pAddSpark);
	pAddSpark->Release();
	pSparkEffect->Release();

	////nothing on death here..
	//if (this->GetCreator()->GetPhysicalObjectType() == PhysicalObjectType::ePLAYER_SHIP)
	//{
	//	Node *pNode			= new Node();
	//	pNode->SetVisual("data\\visuals\\p_blue.3ds");
	//	pNode->m_vScale.x	= 0.20f;
	//	pNode->m_vScale.y	= 0.20f;
	//	pNode->m_vScale.z	= 0.20f;

	//	for (int i = 0; i < 7; ++i)
	//	{
	//		Projectile *pNewProjectile = new Projectile(pNode);
	//		pNewProjectile->GetNode()->m_PosQuat.pos = this->GetNode()->m_PosQuat.pos;

	//		pNewProjectile->SetCreator(this->GetCreator());
	//		pNewProjectile->SetWeapon(this->GetWeapon());
	//		pNewProjectile->SetVelocity(VECTOR3(-6.0f + 2.0f * i, -3.0f + 1.0f * i, 0.0f));

	//		AddObjectEvent *pAddProj = new AddObjectEvent();
	//		pAddProj->SetObject(pNewProjectile, this->GetScene());
	//		gEventManager()->AddEvent(pAddProj);
	//		pAddProj->Release();
	//		pNewProjectile->Release();
	//	}

	//	pNode->Release();
	//}
}

void Projectile::Refresh(const float &fDeltaTime)
{
	PhysicalObject::Refresh(fDeltaTime);

	//rotate the projectile based on the direction it is traveling

	//update the rotation for debug visual display
	Quat *quat = &(GetNode()->m_PosQuat.quat);
	Quat rotation;
	rotation.CreateFromRotationRADIANS(fDeltaTime * 0.75f * PI, m_vVelocity.x, m_vVelocity.y, m_vVelocity.z);
	*quat = rotation * *quat;
}

void Projectile::SetCreator(PhysicalObject *pCreator)
{
	if (m_pCreator != pCreator)
	{
		if (m_pCreator)
		{
			m_pCreator->Release();
			m_pCreator = 0;
		}

		if (pCreator)
		{
			m_pCreator = pCreator;
			m_pCreator->AddRef();
		}	
	}
}

void Projectile::SetWeapon(Weapon *pWeapon)
{
	if (m_pWeapon != pWeapon)
	{
		if (m_pWeapon)
		{
			m_pWeapon->Release();
			m_pWeapon = 0;
		}

		if (pWeapon)
		{
			m_pWeapon = pWeapon;
			m_pWeapon->AddRef();
		}
	}
}

Block *Projectile::ToBlock()
{
	return 0;
}