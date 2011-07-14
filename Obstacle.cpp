#include "Obstacle.h"

#include "AddObjectEvent.h"
#include "DriftingMovementPlugin.h"
#include "EventManager.h"
#include "FireworkEffect.h"
#include "Node.h"
#include "MovementTask.h"
#include "Projectile.h"
#include "Shield.h"
#include "Shockwave.h"
#include "StationaryMovementPlugin.h"
#include "utility.h"

#include <iostream>
using namespace std;

Obstacle::Obstacle()
{
	m_ObstacleType = (ObstacleType)GenerateRandomInt(Obstacle::eNUM_TYPES - 1);
	Init();
}

Obstacle::Obstacle(ObstacleType type)
{
	m_ObstacleType = type;
	Init();
}

Obstacle::~Obstacle()
{

}

void Obstacle::Clone(Obstacle *pClone)
{
	PhysicalObject::Clone(pClone);

	SetObstacleType(pClone->GetObstacleType());
	m_dwLevel = pClone->GetLevel();

	GenerateStats();
}

void Obstacle::Collide(PhysicalObject *pOtherObject)
{
	switch (pOtherObject->GetPhysicalObjectType())
	{
	case PhysicalObject::ePROJECTILE:
		{
			Projectile *pProj = (Projectile *)pOtherObject;
			
			//only take damage if you aren't the creator
			if (pProj->GetCreator() != this)
			{
				ApplyDamage(pProj->GetWeapon()->GetDamage(), pProj->GetCreator());
			}
		}
		break;

	case PhysicalObject::ePLAYER_SHIP:
		ApplyOnePhysicalDamage(pOtherObject);
		break;

	default:
		
		break;

	}
}

PhysicalObject *Obstacle::Duplicate() 
{
	Obstacle *pDuplicate = new Obstacle();
	pDuplicate->Clone(this);
	return pDuplicate;	
}

void Obstacle::FromBlock(Block *pBlock)
{

}

void Obstacle::Generate(int dwLevel)
{
	m_dwLevel			= dwLevel;
	Node	*pNode		= new Node();

	Shield *pShield = new Shield(0);
	this->SetShield(pShield);
	pShield->Release();

	switch (m_ObstacleType)
	{
	case eDRIFTING:
		//pNode->SetVisual("data\\visuals\\asteroid1.3ds");
		pNode->SetVisual("data\\visuals\\a_stockermans\\satellite.3ds");
		SetMovementPlugin(DriftingMovementPlugin::GetPlugin());
		pShield->SetColor(VECTOR3(1.0f, 0.6f, 0.6f));
		break;

	case eSTATIONARY:
		pNode->SetVisual("data\\visuals\\a_stockermans\\satellite.3ds");
		SetMovementPlugin(StationaryMovementPlugin::GetPlugin());
		pShield->SetColor(VECTOR3(0.6f, 1.0f, 0.6f));
		break;

	default:
		printf("Obstacle::Generate default case!  -----> SHOULD NOT HIT THIS\n");
		break;
	}

	SetNode(pNode);
	pNode->Release();

	GenerateStats();
}

void Obstacle::GenerateStats()
{
	float fScale	= 0.0f;

	switch (m_ObstacleType)
	{
	case eDRIFTING:
		m_vRotationAxis.x		= GenerateRandomFloat(1.0f);
		m_vRotationAxis.y		= GenerateRandomFloat(1.0f);
		m_vRotationAxis.z		= GenerateRandomFloat(1.0f);
		m_fSecondsPerRotation	= GenerateRandomFloat(10.0f) + 4.0f;
		fScale					= GenerateRandomFloat(0.5f) + 1.4f;
		break;

	case eSTATIONARY:
		m_vRotationAxis.x		= GenerateRandomFloat(1.0f);
		m_vRotationAxis.y		= GenerateRandomFloat(1.0f);
		m_vRotationAxis.z		= GenerateRandomFloat(1.0f);
		m_fSecondsPerRotation	= GenerateRandomFloat(10.0f) + 4.0f;
		fScale					= GenerateRandomFloat(0.9f) + 1.0f;
		break;

	default:
		printf("Obstacle::GenerateStats default case!  -----> SHOULD NOT HIT THIS\n");
		break;
	}

	NormalizeVECTOR3(m_vRotationAxis);
	m_pNode->m_vScale.x		= fScale;
	m_pNode->m_vScale.y		= fScale;
	m_pNode->m_vScale.z		= fScale;

	m_fStructureMax			= fScale * ((GenerateRandomFloat(10.0f) * (float)m_dwLevel) + (float)m_dwLevel * 20.0f) + 12.0f;
	m_fStructure			= m_fStructureMax;
}

Shockwave *Obstacle::GetShockwave()
{
	return new Shockwave(this, false);
}

void Obstacle::Init()
{
	SetPhysicalObjectType(PhysicalObject::eOBSTACLE);
}

void Obstacle::OnDeath()
{
	FireworkEffect *pFireworkEffect = new FireworkEffect(this->GetNode()->m_vScale.x, this->GetNode()->m_PosQuat.pos);
	AddObjectEvent *pAddExplosion = new AddObjectEvent();
	pAddExplosion->SetObject(pFireworkEffect, this->GetScene());
	gEventManager()->AddEvent(pAddExplosion);
	pAddExplosion->Release();
	pFireworkEffect->Release();
}

void Obstacle::Refresh(const float &fDeltaTime)
{
	PhysicalObject::Refresh(fDeltaTime);

	if (m_fSecondsPerRotation != 0.0f)
	{
		float			fPercentOfFullRotation	= fDeltaTime / m_fSecondsPerRotation;
		Quat			*quat					= &(m_pNode->m_PosQuat.quat);
		Quat			rotation;
		
		rotation.CreateFromRotationRADIANS(2.0f * PI * fPercentOfFullRotation, m_vRotationAxis.x, m_vRotationAxis.y, m_vRotationAxis.z);
		*quat = rotation * *quat;		
	}

	if (!m_pMovementTask || m_pMovementTask->IsComplete())
	{
		if (GetMovementPlugin())
		{
			GetMovementPlugin()->AssignMovementTask(this);
		}
		else
		{
			//assign some idle task?  might not even need to do that!
		}
	}
	else
	{
		m_pMovementTask->ApplyTask(this, fDeltaTime);
	}
}

void Obstacle::SetObstacleType(ObstacleType newType)
{
	m_ObstacleType = newType;
}

Block *Obstacle::ToBlock()
{
	return 0;
}