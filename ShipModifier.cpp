#include "ShipModifier.h"

#include <iostream>
using namespace std;

ShipModifier::ShipModifier()
{
	Init();
}

ShipModifier::~ShipModifier()
{
	
}

void ShipModifier::AddModifier(ShipModifier *pModifier)
{
	m_fDamagePercent			+= pModifier->m_fDamagePercent;
	m_fPhysicalDamageFlat		+= pModifier->m_fPhysicalDamageFlat;
	m_fPhysicalDamagePercent	+= pModifier->m_fPhysicalDamagePercent;
	m_fEnergyDamageFlat			+= pModifier->m_fEnergyDamageFlat;
	m_fEnergyDamagePercent		+= pModifier->m_fEnergyDamagePercent;
	m_fEnergy					+= pModifier->m_fEnergy;
	m_fEnergyRecharge			+= pModifier->m_fEnergyRecharge;
	m_fShields					+= pModifier->m_fShields;
	m_fShieldEfficiency			+= pModifier->m_fShieldEfficiency;
	m_fShieldRecharge			+= pModifier->m_fShieldRecharge;
	m_fStructure				+= pModifier->m_fStructure;
	m_fExperienceModifier		+= pModifier->m_fExperienceModifier;
	m_fArmor					+= pModifier->m_fArmor;
	m_fRateOfFire				+= pModifier->m_fRateOfFire;
	m_fMovement					+= pModifier->m_fMovement;
	m_fReflection				+= pModifier->m_fReflection;
	m_fTurretRotateAngle		+= pModifier->m_fTurretRotateAngle;
	m_fTurretRotateSpeed		+= pModifier->m_fTurretRotateSpeed;
	m_fProjectileSpeed			+= pModifier->m_fProjectileSpeed;
	m_fProjectileSplitting		+= pModifier->m_fProjectileSplitting;
	m_fCloningChance			+= pModifier->m_fCloningChance;
	m_fPhysicalDamageReduction	+= pModifier->m_fPhysicalDamageReduction;
	m_fEnergyDamageReduction	+= pModifier->m_fEnergyDamageReduction;
	m_fJumpFrequency			+= pModifier->m_fJumpFrequency;
}	

void ShipModifier::FromBlock(Block *pBlock)
{

}

void ShipModifier::Init()
{
	m_fDamagePercent			= 0.0f;
	m_fPhysicalDamageFlat		= 0.0f;
	m_fPhysicalDamagePercent	= 0.0f;
	m_fEnergyDamageFlat			= 0.0f;
	m_fEnergyDamagePercent		= 0.0f;
	m_fEnergy					= 0.0f;
	m_fEnergyRecharge			= 0.0f;
	m_fShields					= 0.0f;
	m_fShieldEfficiency			= 0.0f;
	m_fShieldRecharge			= 0.0f;
	m_fStructure				= 0.0f;
	m_fExperienceModifier		= 0.0f;
	m_fArmor					= 0.0f;
	m_fRateOfFire				= 0.0f;
	m_fMovement					= 0.0f;
	m_fReflection				= 0.0f;
	m_fTurretRotateAngle		= 0.0f;
	m_fTurretRotateSpeed		= 0.0f;
	m_fProjectileSpeed			= 0.0f;
	m_fProjectileSplitting		= 0.0f;
	m_fCloningChance			= 0.0f;
	m_fPhysicalDamageReduction	= 0.0f;
	m_fEnergyDamageReduction	= 0.0f;
	m_fJumpFrequency			= 0.0f;
}

void ShipModifier::Print(int indent) const
{
	if (m_fDamagePercent != 0.0f) {
		printf("%*sDamage increased by %.2f%%\n", indent, " ", m_fDamagePercent * 100.0f);
	}
	if (m_fPhysicalDamageFlat != 0.0f) {
		printf("%*sPhysical damage increased by %.2f\n", indent, " ", m_fPhysicalDamageFlat);
	}
	if (m_fPhysicalDamagePercent != 0.0f) {
		printf("%*sPhysical damage increased by %.2f%%\n", indent, " ", m_fPhysicalDamagePercent * 100.0f);
	}
	if (m_fEnergyDamageFlat != 0.0f) {
		printf("%*sEnergy damage increased by %.2f\n", indent, " ", m_fEnergyDamageFlat);
	}
	if (m_fEnergyDamagePercent != 0.0f) {
		printf("%*sEnergy damage increased by %.2f%%\n", indent, " ", m_fEnergyDamagePercent * 100.0f);
	}
	if (m_fEnergy != 0.0f) {
		printf("%*sEnergy increased by %.2f\n", indent, " ", m_fEnergy);
	}
	if (m_fEnergyRecharge != 0.0f) {
		printf("%*sEnergy recharge rate increased by %f energy/second\n", indent, " ", m_fEnergyRecharge);
	}
	if (m_fShields != 0.0f) {
		printf("%*sShields increased by %.2f\n", indent, " ", m_fShields);
	}
	if (m_fShieldEfficiency != 0.0f) {
		printf("%*sShield efficiency increased by %.2f%%\n", indent, " ", m_fShieldEfficiency * 100.0f);
	}
	if (m_fShieldRecharge != 0.0f) {
		printf("%*sShield recharge increased by %.2f%%\n", indent, " ", m_fShieldRecharge * 100.0f);
	}
	if (m_fStructure != 0.0f) {
		printf("%*sStructure increased by %.2f\n", indent, " ", m_fStructure);
	}
	if (m_fExperienceModifier != 0.0f) {
		printf("%*sExperience gain increased by %.2f%%\n", indent, " ", m_fExperienceModifier * 100.0f);
	}
	if (m_fArmor != 0.0f) {
		printf("%*sArmor increased by %.2f\n", indent, " ", m_fArmor);
	}
	if (m_fRateOfFire != 0.0f) {
		printf("%*sRate of fire increased by %.2f%%\n", indent, " ", m_fRateOfFire * 100.0f);
	}
	if (m_fMovement != 0.0f) {
		printf("%*sMovement increased by %.2f%%\n", indent, " ", m_fMovement * 100.0f);
	}
	if (m_fReflection != 0.0f) {
		printf("%*sChance of reflection increased by %.2f\n", indent, " ", m_fReflection);
	}
	if (m_fTurretRotateAngle != 0.0f) {
		printf("%*sTurret rotation angle increased by %.2f radians\n", indent, " ", m_fTurretRotateAngle);
	}
	if (m_fTurretRotateSpeed != 0.0f) {
		printf("%*sTurret rotation speed increased by %.2f%%\n", indent, " ", m_fTurretRotateSpeed * 100.0f);
	}
	if (m_fProjectileSpeed != 0.0f) {
		printf("%*sProjectile speed increased by %.2f%%\n", indent, " ", m_fProjectileSpeed * 100.0f);
	}
	if (m_fProjectileSplitting != 0.0f) {
		printf("%*sProjectile splitting chance increased by %.2f\n", indent, " ", m_fProjectileSplitting * 100.0f);
	}
	if (m_fCloningChance != 0.0f) {
		printf("%*s%.1f%% chance of cloning\n", indent, " ", m_fCloningChance * 100.0f);
	}
	if (m_fPhysicalDamageReduction != 0.0f) {
		printf("%*sPhysical damage reduced by %.2f\n", indent, " ", m_fPhysicalDamageReduction);
	}
	if (m_fEnergyDamageReduction != 0.0f) {
		printf("%*sEnergy damage reduced by %.2f\n", indent, " ", m_fEnergyDamageReduction);
	}
	if (m_fJumpFrequency != 0.0f) {
		printf("%*s%.2f jump frequency\n", indent, " ", m_fJumpFrequency);
	}
}

void ShipModifier::Refresh(const float &fDeltaTime)
{
	
}	

Block *ShipModifier::ToBlock()
{
	return 0;
}