#ifndef _PHYSICAL_OBJECT_
#define _PHYSICAL_OBJECT_

#include "def.h"
#include "Object.h"
#include "Weapon.h"
#include <cassert>
using namespace std;

class Controller;
class FiringPlugin;
class FiringTask;
class MovementPlugin;
class MovementTask;
class Shield;
class Shockwave;

//amount of decay per second if the ship is not accelerating in the direction the ship is moving
#define									VELOCITY_DECAY								3.0f
#define									REMOVAL_BOUNDS_DISTANCE_SQUARED				10000.0f //two hundred units away

class PhysicalObject : public Object
{
public:
	
	enum PhysicalObjectType {
		eNO_TYPE,
		ePLAYER_SHIP,
		eCOMPUTER_SHIP,
		ePROJECTILE,
		ePOWER_UP,
		eOBSTACLE,

		eNUM_TYPES,
	};

	PhysicalObject();

	void									AddKills(int dwNumKills);
	void									ApplyDamage(Weapon::Damage damage, PhysicalObject *pCause);
	void									ApplyOnePhysicalDamage(PhysicalObject *pCause);
	void									Clone(PhysicalObject *pClone);
	virtual void							Collide(PhysicalObject *pOtherObject)	= 0;
	virtual void							Destroy();
	virtual PhysicalObject					*Duplicate()							= 0;
	void									FireWeaponOne();
	void									FireWeaponTwo();

	float									GetMaxAcceleration() const				{ return m_fMaxAcceleration; }
	VECTOR3									GetAcceleration() const					{ return m_vAcceleration; }
	virtual const char						*GetClassName()	const					= 0;
	bool									GetCollidable() const					{ return m_bCollidable; }
	virtual VECTOR3							GetDebugColor() const					= 0;
	virtual FiringPlugin					*GetFiringPlugin() const				{ return m_pFiringPlugin; }
	virtual MovementPlugin					*GetMovementPlugin() const				{ return m_pMovementPlugin; }
	int										GetNumKills() const						{ return m_dwNumKills; }
	PhysicalObjectType						GetPhysicalObjectType() const			{ return m_dwPhysicalObjectType; }
	virtual Shockwave						*GetShockwave()							= 0;
	Shield									*GetShield() const						{ return m_pShield; }
	float									GetShields() const						{ return m_fShields; }
	float									GetShieldsMax() const					{ return m_fShieldsMax; }
	float									GetStructure() const					{ return m_fStructure; }
	float									GetStructureMax() const					{ return m_fStructureMax; }
	PhysicalObject							*GetTarget() const						{ return m_pTarget; }
	VECTOR3									GetVelocity() const						{ return m_vVelocity; }
	Weapon									*GetWeaponOne() const					{ return m_pWeaponOne; }
	Weapon									*GetWeaponTwo() const					{ return m_pWeaponTwo; }

	bool									IsAlive() const;
	virtual void							Refresh(const float &fDeltaTime);
	void									Regenerate();
	void									Reset();
	void									SetAccelerationDir(VECTOR3 vAccelerationDir);
	void									SetDestroyer(PhysicalObject *pObject);
	virtual void							SetFiringPlugin(FiringPlugin *pPlugin);
	void									SetFiringTask(FiringTask *pTask);
	void									SetMaxSpeed(float fMax)					{ m_fMaxSpeed = fMax; }
	virtual void							SetMovementPlugin(MovementPlugin *pPlugin);
	void									SetMovementTask(MovementTask *pTask);
	void									SetShield(Shield *pNewShield);
	void									SetTarget(PhysicalObject *pObject);
	void									SetThrottle(float fThrottle);
	void									SetVelocity(VECTOR3 vVelocity);
	void									SetVelocityDecay(bool bDecay)			{ m_bVelocityDecay = bDecay; }
	void									SetWeaponOne(Weapon *pWeapon);
	void									SetWeaponTwo(Weapon *pWeapon);
	
protected:
	virtual ~PhysicalObject();

	virtual void							OnDeath();
	void									SetPhysicalObjectType(PhysicalObjectType dwType);
	virtual void							RefreshPosition(const float &fDeltaTime);
	void									RefreshTilt(const float &fDeltaTime);

	bool									m_bCollidable;
	float									m_fShields;
	float									m_fShieldsMax;
	float									m_fStructure;
	float									m_fStructureMax;
	float									m_fShieldsRechargeRate; //shield pts per second

	PhysicalObject							*m_pTarget;			//the object this thing is targeting!
	PhysicalObject							*m_pDestroyer;		//the last object to deal damage to this thing
	Weapon									*m_pWeaponOne;		//objects first weapon
	Weapon									*m_pWeaponTwo;		//objects second weapon
	Shield									*m_pShield;

	VECTOR3									m_vVelocity;		//both stored as UNITS PER SECOND
	float									m_fMaxAcceleration;	//rate at which this object can accelerate
	float									m_fThrottle;		//percentage of max acceleration this is accelerating
	VECTOR3									m_vAccelerationDir;	//direction this is currently accelerating
	VECTOR3									m_vAcceleration;	//current acceleration!
	float									m_fMaxSpeed;		//max move speed

	float									m_fTiltX;
	float									m_fTiltY;

	bool									m_bVelocityDecay;	//whether or not this is affected by v decay

	FiringPlugin							*m_pFiringPlugin;
	MovementPlugin							*m_pMovementPlugin;

	FiringTask								*m_pFiringTask;
	MovementTask							*m_pMovementTask;

	int										m_dwNumKills;		//number of physical objects this thing has destroyed
	int										m_dwBoundsCheck;	//only check the bounds every now and then

private:
	void									Init();

	PhysicalObjectType						m_dwPhysicalObjectType;

};

#endif