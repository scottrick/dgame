#include "PhysicalObject.h"

#include "AddObjectEvent.h"
#include "EventManager.h"
#include "ExplosionEffect.h"
#include "FireworkEffect.h"
#include "FiringPlugin.h"
#include "FiringTask.h"
#include "Matrix44.h"
#include "MovementPlugin.h"
#include "MovementTask.h"
#include "Node.h"
#include "PosQuat.h"
#include "RemoveObjectEvent.h"
#include "Shield.h"
#include "Shockwave.h"
#include "SparkEffect.h"
#include "TestExplosionEffect.h"
#include "utility.h"

#define	MAX_TILT		40.0f

PhysicalObject::PhysicalObject()
{
	Init();
}

PhysicalObject::~PhysicalObject()
{
	SetFiringPlugin(0);
	SetFiringTask(0);
	SetMovementPlugin(0);
	SetMovementTask(0);
	SetDestroyer(0);
	SetShield(0);
	SetTarget(0);
	SetWeaponOne(0);
	SetWeaponTwo(0);
}

void PhysicalObject::AddKills(int dwNumKills)
{
	if (dwNumKills > 0)
	{
		m_dwNumKills += dwNumKills;
	}
}

void PhysicalObject::ApplyDamage(Weapon::Damage damage, PhysicalObject *pCause)
{
	//for now just treat both damage types the same
	float fDamage = damage.m_fEnergyDamage + damage.m_fPhysicalDamage;

	//printf("applying %f damage to %s\n", fDamage, this->GetClassName());

	if (m_pShield)
	{
		m_pShield->Ping();
	}

	if (fDamage > 0)
	{
		SetDestroyer(pCause);
	}

	if (m_fShields >= fDamage)
	{
		m_fShields -= fDamage;
	}
	else 
	{
		fDamage -= m_fShields;
		m_fShields = 0.0f;

		if (m_fStructure >= fDamage)
		{
			m_fStructure -= fDamage;
		}
		else
		{
			m_fStructure = 0.0f;
		}
	}
}	

//a conveniance function cause im lazy
void PhysicalObject::ApplyOnePhysicalDamage(PhysicalObject *pCause)
{
	Weapon::Damage dmg;
	dmg.m_fEnergyDamage			= 0.0f;
	dmg.m_fPhysicalDamage		= 1.0f;
	ApplyDamage(dmg, pCause);
}

void PhysicalObject::Clone(PhysicalObject *pClone)
{
	Object::Clone(pClone);

	SetPhysicalObjectType(pClone->GetPhysicalObjectType());
	SetFiringPlugin(pClone->GetFiringPlugin());
	SetMovementPlugin(pClone->GetMovementPlugin());
	SetTarget(pClone->GetTarget());

	if (pClone->GetWeaponOne())
	{
		Weapon *pWeaponOne = new Weapon();
		pWeaponOne->Clone(pClone->GetWeaponOne());
		SetWeaponOne(pWeaponOne);
		pWeaponOne->Release();
	}

	if (pClone->GetWeaponTwo())
	{
		Weapon *pWeaponTwo = new Weapon();
		pWeaponTwo->Clone(pClone->GetWeaponTwo());
		SetWeaponTwo(pWeaponTwo);
		pWeaponTwo->Release();
	}
	if (pClone->GetShield())
	{
		Shield *pShield = new Shield(m_pNode);
		pShield->Clone(pClone->GetShield());
		SetShield(pShield);
		pShield->Release();
	}

	m_vVelocity					= pClone->GetVelocity();
	m_fMaxAcceleration			= pClone->GetMaxAcceleration();
	m_vAcceleration				= pClone->GetAcceleration();
	m_bCollidable				= pClone->GetCollidable();
	m_fShields					= pClone->GetShields();
	m_fShieldsMax				= pClone->GetShieldsMax();
	m_fStructure				= pClone->GetStructure();
	m_fStructureMax				= pClone->GetStructureMax();
}

void PhysicalObject::Destroy()
{
	//remove this object
	RemoveObjectEvent *pNewRemoveEvent = new RemoveObjectEvent();
	pNewRemoveEvent->SetObject(this);
	gEventManager()->AddEvent(pNewRemoveEvent);
	pNewRemoveEvent->Release();

	this->OnDeath();

	if (m_pDestroyer)
	{
		m_pDestroyer->AddKills(1);
	}

	//add shockwave
	Shockwave *pShockwave = GetShockwave();
	if (pShockwave)
	{
		AddObjectEvent *pAddPostEvent = new AddObjectEvent();
		pAddPostEvent->SetObject(pShockwave, this->GetScene());
		gEventManager()->AddEvent(pAddPostEvent);
		pAddPostEvent->Release();
		pShockwave->Release();
	}
}

void PhysicalObject::FireWeaponOne()
{
	if (m_pWeaponOne)
	{
		m_pWeaponOne->Fire();
	}
}

void PhysicalObject::FireWeaponTwo()
{
	if (m_pWeaponTwo)
	{
		m_pWeaponTwo->Fire();
	}
}

void PhysicalObject::Init()
{
	SetObjectType(Object::eTYPE_PHYSICAL_OBJECT);
	SetPhysicalObjectType(PhysicalObject::eNO_TYPE);
	
	m_dwNumKills				= 0;
	m_dwBoundsCheck				= 0;
	m_pDestroyer				= 0;
	m_pTarget					= 0;
	m_pFiringPlugin				= 0;
	m_pFiringTask				= 0;
	m_pMovementPlugin			= 0;
	m_pMovementTask				= 0;
	m_pWeaponOne				= 0;
	m_pWeaponTwo				= 0;
	m_pShield					= 0;

	//Some Defaults!
	m_vVelocity.x				= 0.0f;
	m_vVelocity.y				= 0.0f;
	m_vVelocity.z				= 0.0f;
	m_vAcceleration.x			= 0.0f;
	m_vAcceleration.y			= 0.0f;
	m_vAcceleration.z			= 0.0f;
	m_vAccelerationDir.x		= 0.0f;
	m_vAccelerationDir.y		= 0.0f;
	m_vAccelerationDir.z		= 0.0f;

	m_bCollidable				= true;
	m_bVelocityDecay			= false;
	
	m_fThrottle					= 0.0f;
	m_fMaxAcceleration			= 9.0f;
	m_fMaxSpeed					= 6.0f;
	m_fStructureMax				= 1.0f;
	m_fStructure				= m_fStructureMax;
	m_fShieldsMax				= 0.0f;
	m_fShields					= m_fShieldsMax;
	m_fShieldsRechargeRate		= 0.0f;

	m_fTiltX					= 0.0f;
	m_fTiltY					= 0.0f;
}

bool PhysicalObject::IsAlive() const
{
	if (m_fStructure > 0.0f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void PhysicalObject::OnDeath()
{ //default death stuff; just a small explosion effect
	ExplosionEffect *pExplosionEffect = new ExplosionEffect(this->GetNode()->m_vScale.x * 0.5f, this->GetNode());
	AddObjectEvent *pAddExplosion = new AddObjectEvent();
	pAddExplosion->SetObject(pExplosionEffect, this->GetScene());
	gEventManager()->AddEvent(pAddExplosion);
	pAddExplosion->Release();
	pExplosionEffect->Release();
}

void PhysicalObject::Refresh(const float &fDeltaTime)
{
	if (m_fStructure <= 0.0f)
	{
		Destroy();
		return;
	}

	if (m_dwBoundsCheck % 50 == 0)
	{
		//check to see if this is too far off the board.  if so, remove it from the game!
		VECTOR3 *pPos = &(GetNode()->m_PosQuat.pos);
		float fDistanceSquared = pPos->x * pPos->x + pPos->y * pPos->y + pPos->z * pPos->z;
	
		if ((fDistanceSquared > REMOVAL_BOUNDS_DISTANCE_SQUARED) || (fDistanceSquared < -REMOVAL_BOUNDS_DISTANCE_SQUARED))
		{ //outside play area, so remove!
			RemoveObjectEvent *pNewRemoveEvent = new RemoveObjectEvent();
			pNewRemoveEvent->SetObject(this);
			gEventManager()->AddEvent(pNewRemoveEvent);
			pNewRemoveEvent->Release();
			return;
		}
	}

	m_dwBoundsCheck++;

	if (m_pWeaponOne)
	{
		m_pWeaponOne->Refresh(fDeltaTime);
	}

	if (m_pWeaponTwo)
	{
		m_pWeaponTwo->Refresh(fDeltaTime);
	}

	RefreshPosition(fDeltaTime);
	RefreshTilt(fDeltaTime);

	if (m_fShields < m_fShieldsMax)
	{
		m_fShields += fDeltaTime * m_fShieldsRechargeRate;

		if (m_fShields >= m_fShieldsMax)
		{
			m_fShields = m_fShieldsMax;
		}
	}
}

void PhysicalObject::RefreshPosition(const float &fDeltaTime)
{ //NOTE:  IF YOU CHANGE HOW PHYSICAL OBJECTS MOVE, THEN BE SURE TO UPDATE THE PLAYER OBJECTS REFRESHPOS FUNC AS WELL
	if (m_pNode)
	{
		//update position
		m_pNode->m_PosQuat.pos.x	+= (fDeltaTime * m_vVelocity.x);
		m_pNode->m_PosQuat.pos.y	+= (fDeltaTime * m_vVelocity.y);
		m_pNode->m_PosQuat.pos.z	+= (fDeltaTime * m_vVelocity.z);

		if (m_bVelocityDecay)
		{
			float fDecay = 1.0f - (VELOCITY_DECAY * fDeltaTime);
			if ((m_vVelocity.x * m_vAcceleration.x) <= 0.0f)
			{
				m_vVelocity.x *= fDecay;
			}
			if ((m_vVelocity.y * m_vAcceleration.y) <= 0.0f)
			{
				m_vVelocity.y *= fDecay;
			}
			if ((m_vVelocity.z * m_vAcceleration.z) <= 0.0f)
			{
				m_vVelocity.z *= fDecay;
			}
		}

		//update velocity
		m_vVelocity.x				+= fDeltaTime * m_vAcceleration.x;
		m_vVelocity.y				+= fDeltaTime * m_vAcceleration.y;
		m_vVelocity.z				+= fDeltaTime * m_vAcceleration.z;

		if (MagnitudeSquaredVECTOR3(m_vVelocity) > (m_fMaxSpeed * m_fMaxSpeed))
		{
			NormalizeVECTOR3(m_vVelocity);
			m_vVelocity.x *= m_fMaxSpeed;
			m_vVelocity.y *= m_fMaxSpeed;
			m_vVelocity.z *= m_fMaxSpeed;
		}
	}	
}

void PhysicalObject::RefreshTilt(const float &fDeltaTime)
{
	if (this->GetPhysicalObjectType() == PhysicalObject::eOBSTACLE)
	{ //obstacles don't tilt
		return;
	}

	//update X and Y tilt based on current acceleration
	static VECTOR3 vAccVector;
	vAccVector = m_vAcceleration;
	NormalizeVECTOR3(vAccVector);

	float fDecay = 1.0f - fDeltaTime;
	if (fDecay < 0.0f)
	{
		fDecay = 0.0f;
	}

	//first decay old tilt values.  only decay if not accelerating in that axis
	if (vAccVector.x == 0.0f)
	{
		if (m_fTiltX > 0.0f)
		{
			m_fTiltX -= fDeltaTime * MAX_TILT;
			if (m_fTiltX < 0.0f)
			{
				m_fTiltX = 0.0f;
			}
		}
		else if (m_fTiltX < 0.0f)
		{
			m_fTiltX += fDeltaTime * MAX_TILT;
			if (m_fTiltX > 0.0f)
			{
				m_fTiltX = 0.0f;
			}
		}
	}

	if (vAccVector.y == 0.0f)
	{
		if (m_fTiltY > 0.0f)
		{
			m_fTiltY -= fDeltaTime * MAX_TILT;
			if (m_fTiltY < 0.0f)
			{
				m_fTiltY = 0.0f;
			}
		}
		else if (m_fTiltY < 0.0f)
		{
			m_fTiltY += fDeltaTime * MAX_TILT;
			if (m_fTiltY > 0.0f)
			{
				m_fTiltY = 0.0f;
			}
		}
	}

	m_fTiltX += vAccVector.x * (1.0f - fDecay) * MAX_TILT * 4.0f;
	m_fTiltY += vAccVector.y * (1.0f - fDecay) * MAX_TILT * 4.0f;

	if (m_fTiltX > MAX_TILT)
	{
		m_fTiltX = MAX_TILT;
	}
	else if (m_fTiltX < -MAX_TILT)
	{
		m_fTiltX = -MAX_TILT;
	}

	if (m_fTiltY > MAX_TILT)
	{
		m_fTiltY = MAX_TILT;
	}
	else if (m_fTiltY < -MAX_TILT)
	{
		m_fTiltY = -MAX_TILT;
	}

	static Quat qRotX;
	qRotX.CreateFromRotationDEGREES(m_fTiltX, 0.0f, 1.0f, 0.0f);

	static Quat qRotY;
	qRotY.CreateFromRotationDEGREES(m_fTiltY, -1.0f, 0.0f, 0.0f);

	//set the objects rotation
	m_pNode->m_PosQuat.quat = qRotX * qRotY;
	m_pNode->m_PosQuat.quat.Normalize();
}

void PhysicalObject::Regenerate()
{
	m_fShields		= m_fShieldsMax;
	m_fStructure	= m_fStructureMax;
}

void PhysicalObject::Reset()
{
	m_dwNumKills = 0;
}

void PhysicalObject::SetAccelerationDir(VECTOR3 vAccelerationDir)
{
	NormalizeVECTOR3(vAccelerationDir);
	m_vAccelerationDir = vAccelerationDir;

	m_vAcceleration.x = m_vAccelerationDir.x * m_fMaxAcceleration * m_fThrottle;
	m_vAcceleration.y = m_vAccelerationDir.y * m_fMaxAcceleration * m_fThrottle;
	m_vAcceleration.z = m_vAccelerationDir.z * m_fMaxAcceleration * m_fThrottle;
}

void PhysicalObject::SetDestroyer(PhysicalObject *pObject)
{
	if (m_pDestroyer != pObject)
	{
		if (m_pDestroyer)
		{
			m_pDestroyer->Release();
			m_pDestroyer = 0;
		}

		if (pObject)
		{
			m_pDestroyer = pObject;
			m_pDestroyer->AddRef();
		}
	}
}

void PhysicalObject::SetFiringPlugin(FiringPlugin *pPlugin)
{
	if (pPlugin != m_pFiringPlugin)
	{
		if (m_pFiringPlugin)
		{
			m_pFiringPlugin->SignalRemoved(this);
			m_pFiringPlugin->Release();
			m_pFiringPlugin = 0;
		}

		if (pPlugin)
		{
			m_pFiringPlugin = pPlugin;
			m_pFiringPlugin->AddRef();
		}
	}
}

void PhysicalObject::SetFiringTask(FiringTask *pTask)
{
	if (m_pFiringTask != pTask)
	{
		if (m_pFiringTask)
		{
			m_pFiringTask->Release();
			m_pFiringTask = 0;
		}

		if (pTask)
		{
			m_pFiringTask = pTask;
			m_pFiringTask->AddRef();
		}
	}
}

void PhysicalObject::SetMovementPlugin(MovementPlugin *pPlugin)
{
	if (pPlugin != m_pMovementPlugin)
	{
		if (m_pMovementPlugin)
		{
			m_pMovementPlugin->SignalRemoved(this);
			m_pMovementPlugin->Release();
			m_pMovementPlugin = 0;
		}

		if (pPlugin)
		{
			m_pMovementPlugin = pPlugin;
			m_pMovementPlugin->AddRef();
		}
	}
}

void PhysicalObject::SetMovementTask(MovementTask *pTask)
{
	if (m_pMovementTask != pTask)
	{
		if (m_pMovementTask)
		{
			m_pMovementTask->Release();
			m_pMovementTask = 0;
		}

		if (pTask)
		{
			m_pMovementTask = pTask;
			m_pMovementTask->AddRef();
		}
	}
}

void PhysicalObject::SetVelocity(VECTOR3 vVelocity)
{
	m_vVelocity = vVelocity;
}

void PhysicalObject::SetPhysicalObjectType(PhysicalObject::PhysicalObjectType dwType)
{
	m_dwPhysicalObjectType = dwType;
}

void PhysicalObject::SetShield(Shield *pNewShield)
{
	if (m_pShield != pNewShield)
	{
		if (m_pShield)
		{
			RemoveObjectEvent *pNewRemoveEvent = new RemoveObjectEvent();
			pNewRemoveEvent->SetObject(m_pShield);
			gEventManager()->AddEvent(pNewRemoveEvent);
			pNewRemoveEvent->Release();

			m_pShield->Release();
			m_pShield = 0;
		}

		if (pNewShield)
		{
			m_pShield = pNewShield;
			m_pShield->AddRef();

			AddObjectEvent *pAddShield = new AddObjectEvent();
			pAddShield->SetObject(m_pShield, this->GetScene());
			gEventManager()->AddEvent(pAddShield);
			pAddShield->Release();
		}
	}
}

void PhysicalObject::SetTarget(PhysicalObject *pObject)
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

void PhysicalObject::SetThrottle(float fThrottle)
{
	if (fThrottle > 1.0f)
	{
		fThrottle = 1.0f;
	}
	else if (fThrottle < 0.0f)
	{
		fThrottle = 0.0f;
	}

	m_fThrottle = fThrottle;

	m_vAcceleration.x = m_vAccelerationDir.x * m_fMaxAcceleration * m_fThrottle;
	m_vAcceleration.y = m_vAccelerationDir.y * m_fMaxAcceleration * m_fThrottle;
	m_vAcceleration.z = m_vAccelerationDir.z * m_fMaxAcceleration * m_fThrottle;
}

void PhysicalObject::SetWeaponOne(Weapon *pWeapon)
{
	if (m_pWeaponOne != pWeapon)
	{
		if (m_pWeaponOne)
		{
			m_pWeaponOne->Release();
			m_pWeaponOne = 0;
		}

		if (pWeapon)
		{
			m_pWeaponOne = pWeapon;
			m_pWeaponOne->AddRef();
		}
	}
}

void PhysicalObject::SetWeaponTwo(Weapon *pWeapon)
{
	if (m_pWeaponTwo != pWeapon)
	{
		if (m_pWeaponTwo)
		{
			m_pWeaponTwo->Release();
			m_pWeaponTwo = 0;
		}

		if (pWeapon)
		{
			m_pWeaponTwo = pWeapon;
			m_pWeaponTwo->AddRef();
		}
	}
}