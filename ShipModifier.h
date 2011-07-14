#ifndef _SHIP_MODIFIER_
#define _SHIP_MODIFIER_

#include "Object.h"

/*
	A ShipModifier is an object which describs modifications to a ship.  This should encompass
	everything from increased movement speed, to hitpoint bonuses, to bonus attack damage and 
	everything inbetween.  The final stats for a ship will be determined by combining all the 
	ShipModifier objects which its components, segments, and anything else affecting it 
	contain.
*/

class ShipModifier : public Object
{
public:
	ShipModifier();

	void							AddModifier(ShipModifier *pModifier);
	virtual void					FromBlock(Block *pBlock);
	virtual const char				*GetClassName() const				{ return "ShipModifier"; }
	virtual void					Print(int indent) const;
	virtual void					Refresh(const float &fDeltaTime);
	virtual Block					*ToBlock();
		
	//ALL THE MODIFIERS!
	float							m_fArmor;					//base ship armor bonus
	float							m_fCloningChance;			//chance ship will clone on fire (%)
	float							m_fDamagePercent;			//increase physical and energy damage by this %
	float							m_fEnergy;					//base ship energy bonus points
	float							m_fEnergyDamageFlat;		//increase energy damage by flat amount
	float							m_fEnergyDamagePercent;		//increase energy damage by this %
	float							m_fEnergyDamageReduction;	//flat rate at which energy damage is reduced
	float							m_fEnergyRecharge;			//energypoints/second recharge bonus
	float							m_fExperienceModifier;		//experience multiplier
	float							m_fJumpFrequency;			//frequency the ship will jump to a new location
	float							m_fMovement;				//ship movement modifier
	float							m_fPhysicalDamageFlat;		//increase physical damage by flat amount
	float							m_fPhysicalDamagePercent;	//increase physical damage by this %
	float							m_fPhysicalDamageReduction;	//flat rate at which physical damage is reduced
	float							m_fProjectileSpeed;			//increase projectiles speed by this %
	float							m_fProjectileSplitting;		//chance projectile launch will split (%)
	float							m_fRateOfFire;				//weapon rate of fire modifier
	float							m_fReflection;				//weapon reflection chance modifier
	float							m_fShields;					//base shields bonus points
	float							m_fShieldEfficiency;		//bonus to shield efficiency
	float							m_fShieldRecharge;			//shieldpoints/second recharge bonus
	float							m_fStructure;				//base structure bonus points
	float							m_fTurretRotateAngle;		//(radians) bonus to turret rotation angle
	float							m_fTurretRotateSpeed;		//turret rotation speed modifier

protected:
	virtual ~ShipModifier();

private:
	void							Init();

};

#endif