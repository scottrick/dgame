#include "HumanController.h"

#include "def.h"
#include "Input.h"
#include "PhysicalObject.h"
#include "PlayerShip.h"

HumanController::HumanController()
{
	Init();
}

HumanController::~HumanController()
{
	
}

void HumanController::FromBlock(Block *pBlock)
{

}

void HumanController::Init()
{
	m_fAcceleration			= 4.0f;
	m_fSpeed				= 4.0f;

	m_dwTempFireCooldown	= 75; //weapon cooldown
	m_dwCurrentCooldown		= 0;
}

void HumanController::Print(int indent) const
{
	printf("%*sHumanShipController 0x%p (%s)\n", indent, "", this);
}

void HumanController::Refresh(int dwDeltaTime)
{
	if (m_vControlledObjects.size() > 0)
	{
		int			dwAlphaInput			= gInput()->GetKeyboardAlphaFlags();

		VECTOR3 vVelocity;
		
		if (dwAlphaInput & INPUT_CHAR_W)
		{
			vVelocity.y				+= 1.0f;
		}

		if (dwAlphaInput & INPUT_CHAR_S)
		{
			vVelocity.y				-= 1.0f;
		}

		if (dwAlphaInput & INPUT_CHAR_D)
		{
			vVelocity.x				+= 1.0f;
		}

		if (dwAlphaInput & INPUT_CHAR_A)
		{
			vVelocity.x				-= 1.0f;
		}

		float	fMagnitudeSquared	= MagnitudeSquaredVECTOR3(vVelocity);
		bool	bNoChange			= false;

		if (fMagnitudeSquared > 0.0f)
		{
			NormalizeVECTOR3(vVelocity);

			vVelocity.x		*= m_fSpeed;
			vVelocity.y		*= m_fSpeed;
			vVelocity.z		*= m_fSpeed;
		}
		else if (fMagnitudeSquared == 0.0f)
		{
			bNoChange		= true;	
		}

		vector<PhysicalObject *>::iterator			iter;
		PhysicalObject								*pPhysObject			= 0;
		for (iter = m_vControlledObjects.begin(); iter != m_vControlledObjects.end(); iter++)
		{
			pPhysObject = *iter;
			if (pPhysObject)
			{
				if (bNoChange)
				{
					vVelocity = pPhysObject->GetVelocity();
					vVelocity.x *= 0.8f;
					vVelocity.y *= 0.8f;
					vVelocity.z *= 0.8f;
				}
				
				pPhysObject->SetVelocity(vVelocity);
			}
		}
	
		/*
		//OLD WAY OF HANDLING MOVEMENT
		bool		bAccelerationChange		= false;

		VECTOR3 vAcceleration; 
		vAcceleration.x = 0.0f; 
		vAcceleration.y = 0.0f; 
		vAcceleration.z = 0.0f;

		if (dwAlphaInput & INPUT_CHAR_W)
		{
			vAcceleration.y			+= 1.0f;
			bAccelerationChange		= true;
		}
		if (dwAlphaInput & INPUT_CHAR_S)
		{
			vAcceleration.y			-= 1.0f;
			bAccelerationChange		= true;
		}
		if (dwAlphaInput & INPUT_CHAR_A)
		{
			vAcceleration.x			-= 1.0f;
			bAccelerationChange		= true;
		}
		if (dwAlphaInput & INPUT_CHAR_D)
		{
			vAcceleration.x			+= 1.0f;
			bAccelerationChange		= true;
		}

		if (bAccelerationChange && (MagnitudeSquaredVECTOR3(vAcceleration) > 0.0f))
		{
			NormalizeVECTOR3(vAcceleration);

			//printf("(%f, %f, %f)\n", vAcceleration.x, vAcceleration.y, vAcceleration.z);

			vector<PhysicalObject *>::iterator	iter;
			PhysicalObject						*pObject		= 0;
			for (iter = m_vControlledObjects.begin(); iter != m_vControlledObjects.end(); iter++)
			{
				pObject = (*iter);
				if (pObject)
				{
					pObject->m_vAcceleration.x += vAcceleration.x * m_fAcceleration;
					pObject->m_vAcceleration.y += vAcceleration.y * m_fAcceleration;
					pObject->m_vAcceleration.z += vAcceleration.z * m_fAcceleration;
				}
			}
		}
		*/

		//HANDLE FIRING
		if ((dwAlphaInput & INPUT_SPACE) && (m_dwCurrentCooldown <= 0))
		{
			m_dwCurrentCooldown = m_dwTempFireCooldown;

			vector<PhysicalObject *>::iterator	iter;
			PhysicalObject						*pObject		= 0;
			for (iter = m_vControlledObjects.begin(); iter != m_vControlledObjects.end(); iter++)
			{
				pObject = (*iter);
				if ((pObject) && (pObject->GetPhysicalObjectType() == PhysicalObject::ePLAYER_SHIP))
				{
					//printf("FIRING");
					((PlayerShip *)pObject)->TestFire();
				}
			}			
		}
		
		if (m_dwCurrentCooldown > 0)
		{
			m_dwCurrentCooldown -= dwDeltaTime;
		}
	}
}

Block *HumanController::ToBlock()
{
	return 0;
}