#include "PlayerShip.h"

#include "ActionScene.h"
#include "AddObjectEvent.h"
#include "ExplosionEffect.h"
#include "EventManager.h"
#include "FireworkEffect.h"
#include "Game.h"
#include "Node.h"
#include "PosQuat.h"
#include "Projectile.h"
#include "Shield.h"
#include "Shockwave.h"
#include "TestExplosionEffect.h"
#include "TestFireEvent.h"
#include "Weapon.h"
#include "utility.h"

#include <iostream>
using namespace std;

PlayerShip::PlayerShip()
{
	Init();
}

PlayerShip::~PlayerShip()
{
	
}

void PlayerShip::Clone(PlayerShip *pClone)
{
	PhysicalObject::Clone(pClone);
}

void PlayerShip::CollectPowerup(PowerUp *pPowerUp)
{
	//collect the powerup's bonus or whatever

	//remove the powerup
	((ActionScene *)(gGame()->GetActionScene()))->RemoveObject((Object *)pPowerUp);
}

void PlayerShip::Collide(PhysicalObject *pOtherObject)
{
	switch (pOtherObject->GetPhysicalObjectType())
	{

	case PhysicalObject::eCOMPUTER_SHIP:
		ApplyOnePhysicalDamage(pOtherObject);
		break;

	case PhysicalObject::eOBSTACLE:
		ApplyOnePhysicalDamage(pOtherObject);
		break;

	case PhysicalObject::ePOWER_UP:
		CollectPowerup((PowerUp *)pOtherObject);
		break;

	case PhysicalObject::ePROJECTILE:
		{
			Projectile *pProj = (Projectile *)pOtherObject;
			//only take damage if you aren't the creator
			if (pProj->GetCreator() != this)
			{
				ApplyOnePhysicalDamage(pProj->GetCreator());
			}
		}
		break;

	default:
		
		break;

	}
}

PhysicalObject *PlayerShip::Duplicate()
{
	PlayerShip *pDuplicate = new PlayerShip();
	pDuplicate->Clone(this);
	return pDuplicate;	
}

void PlayerShip::FromBlock(Block *pBlock)
{

}

Shockwave *PlayerShip::GetShockwave()
{
	return new Shockwave(this, true);
}

void PlayerShip::Init()
{
	SetPhysicalObjectType(PhysicalObject::ePLAYER_SHIP);

	//give player ships some extra starting helth for now and other crap
	m_fStructureMax				= 20.0f;
	m_fStructure				= m_fStructureMax;
	m_fShieldsMax				= 15.0f;
	m_fShields					= m_fShieldsMax;
	m_fShieldsRechargeRate		= 2.5f;

	m_bVelocityDecay			= true;

	Node *pNode = new Node();
	//pNode->SetVisual("data\\visuals\\ship_3.3ds");
	pNode->SetVisual("data\\visuals\\a_stockermans\\spaceship2.3ds");
	SetNode(pNode);
	InitPlayerPosition();
	this->SetVelocity(VECTOR3(0.0f, 6.0f, 0.0f));
	pNode->RaiseNodeFlags(NODE_RENDER_UPSIDEDOWN);
	pNode->Release();

	Shield *pShield = new Shield(pNode);
	pShield->SetColor(VECTOR3(0.6f, 0.6f, 1.0f));
	this->SetShield(pShield);
	pShield->Release();
	
	Weapon *pWeaponOne = new Weapon();
	VECTOR3 vAim(0.0f, 1.0f, 0.0f);
	SetWeaponOne(pWeaponOne);

	Node *pWeaponNode	= new Node();
	pWeaponNode->SetVisual("data\\visuals\\laser.3ds");
	pWeaponNode->m_vScale.x	= 0.42f;
	pWeaponNode->m_vScale.y	= 0.42f;
	pWeaponNode->m_vScale.z	= 0.42f;
	pWeaponOne->SetNode(pWeaponNode);
	pWeaponNode->Release();

	pWeaponOne->SetOwner(this);
	pWeaponOne->SetAim(vAim);
	pWeaponOne->Release();
}

void PlayerShip::InitPlayerPosition()
{
	GetNode()->m_PosQuat.pos.y	= (-GAME_SIZE_Y / 2.0f) + (GAME_SIZE_Y / 15.0f);
	GetNode()->m_PosQuat.pos.x	= 0.0f;
	GetNode()->m_PosQuat.pos.z	= 0.0f;
}

void PlayerShip::OnDeath()
{
	for (int i = 0; i < 5; i++)
	{
		VECTOR3 vOrigin = this->GetNode()->m_PosQuat.pos;
		float fScale = this->GetNode()->GetScale().x;
		vOrigin.x += GenerateRandomFloat(fScale) - fScale / 2.0f;
		vOrigin.y += GenerateRandomFloat(fScale) - fScale / 2.0f;
		FireworkEffect *pFireworkEffect = new FireworkEffect(fScale + GenerateRandomFloat(fScale), vOrigin);
		AddObjectEvent *pAddExplosion = new AddObjectEvent();
		pAddExplosion->SetObject(pFireworkEffect, this->GetScene());
		gEventManager()->AddEvent(pAddExplosion);
		pAddExplosion->Release();
		pFireworkEffect->Release();
	}

	ExplosionEffect *pExplosionEffect = new ExplosionEffect(this->GetNode()->m_vScale.x, this->GetNode());
	AddObjectEvent *pAddExplosion = new AddObjectEvent();
	pAddExplosion->SetObject(pExplosionEffect, this->GetScene());
	gEventManager()->AddEvent(pAddExplosion);
	pAddExplosion->Release();
	pExplosionEffect->Release();
}

void PlayerShip::RefreshPosition(const float &fDeltaTime)
{ //Player ship's custom refresh position, so it can't move off the game board area
	if (m_pNode)
	{
		float fPotentialX			= m_pNode->m_PosQuat.pos.x + (fDeltaTime * m_vVelocity.x);
		float fPotentialY			= m_pNode->m_PosQuat.pos.y + (fDeltaTime * m_vVelocity.y);

		//Don't let the player ship move off of the game area
		if (fPotentialX > (GAME_SIZE_X - m_pNode->m_vScale.x) / 2.0f)
		{
			fPotentialX = (GAME_SIZE_X - m_pNode->m_vScale.x) / 2.0f;
		}
		else if (fPotentialX < (GAME_SIZE_X - m_pNode->m_vScale.x) / -2.0f)
		{
			fPotentialX = (GAME_SIZE_X - m_pNode->m_vScale.x) / -2.0f;
		}

		if (fPotentialY > (GAME_SIZE_Y - m_pNode->m_vScale.y) / 2.0f)
		{
			fPotentialY = (GAME_SIZE_Y - m_pNode->m_vScale.y) / 2.0f;
		}
		else if (fPotentialY < (GAME_SIZE_Y - m_pNode->m_vScale.y) / -2.0f)
		{
			fPotentialY = (GAME_SIZE_Y - m_pNode->m_vScale.y) / -2.0f;
		}

		//update position
		m_pNode->m_PosQuat.pos.x	= fPotentialX;
		m_pNode->m_PosQuat.pos.y	= fPotentialY;
		m_pNode->m_PosQuat.pos.z	+= (fDeltaTime * m_vVelocity.z);

		//set acceleration equal to velocity, so the ship is tilted properly
		m_vAcceleration.x = m_vVelocity.x;
		m_vAcceleration.y = m_vVelocity.y;
		m_vAcceleration.z = m_vVelocity.z;
	}
}

void PlayerShip::TestFire()
{
	//test fire function
	Projectile *pNewProjectile = new Projectile();

	if (m_pNode)
	{
		pNewProjectile->GetNode()->m_PosQuat.pos.x = m_pNode->m_PosQuat.pos.x;
		pNewProjectile->GetNode()->m_PosQuat.pos.y = m_pNode->m_PosQuat.pos.y 
			+ (m_pNode->m_vScale.y / 2.0f) 
			+ (pNewProjectile->GetNode()->m_vScale.y / 2.0f);
		pNewProjectile->GetNode()->m_PosQuat.pos.z = m_pNode->m_PosQuat.pos.z;
		pNewProjectile->SetVelocity(VECTOR3(0.0f, 10.0f, 0.0f));
		pNewProjectile->SetCreator(this);

		TestFireEvent *pTestFireEvent = new TestFireEvent();
		pTestFireEvent->SetProjectile(pNewProjectile);
		gEventManager()->AddEvent(pTestFireEvent);
		pTestFireEvent->Release();
	}

	pNewProjectile->Release();
}

Block *PlayerShip::ToBlock()
{
	return 0;
}