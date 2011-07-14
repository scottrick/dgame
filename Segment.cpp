#include "Segment.h"

#include "ActionScene.h"
#include "Block.h"
#include "Game.h"
#include "Projectile.h"
#include "ShipModifier.h"

Segment::Segment()
{
	Init();
}

Segment::~Segment()
{
	SetShipModifier(0);
}

void Segment::Clone(Segment *pClone) 
{
	PhysicalObject::Clone(pClone);

	SetShipModifier(pClone->GetShipModifier());
}

void Segment::Collide(PhysicalObject *pOtherObject)
{
	switch (pOtherObject->GetPhysicalObjectType())
	{
	case PhysicalObject::ePROJECTILE:
		{
			Projectile *pProj = (Projectile *)pOtherObject;

			//only take damage if you aren't the creator
			if (pProj->GetCreator()->GetObjectType() == PhysicalObject::ePLAYER_SHIP)
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

void Segment::Destroy()
{

}

void Segment::FromBlock(Block *pBlock)
{

}

Shockwave *Segment::GetShockwave()
{
	return 0;
}

void Segment::Init()
{
	m_pShipModifier		= 0;

	ShipModifier *pDefaultModifier = new ShipModifier();
	SetShipModifier(pDefaultModifier);
	pDefaultModifier->Release();
}

void Segment::SetShipModifier(ShipModifier *pNewModifier)
{
	if (m_pShipModifier != pNewModifier)
	{
		if (m_pShipModifier)
		{
			m_pShipModifier->Release();
			m_pShipModifier = 0;
		}

		if (pNewModifier)
		{
			m_pShipModifier = pNewModifier;
			m_pShipModifier->AddRef();
		}
	}
}

Block *Segment::ToBlock()
{
	return 0;
}