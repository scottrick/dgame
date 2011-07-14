#include "EngineeringSegment.h"

#include "ShipModifier.h"
#include "utility.h"

EngineeringSegment::EngineeringSegment()
{
	Init();
}

EngineeringSegment::~EngineeringSegment()
{

}

void EngineeringSegment::Clone(EngineeringSegment *pClone) 
{
	Segment::Clone(pClone);

	SetEngineeringSegmentType(pClone->GetEngineeringSegmentType());
}

PhysicalObject *EngineeringSegment::Duplicate()
{
	EngineeringSegment *pDuplicate = new EngineeringSegment();
	pDuplicate->Clone(this);
	return pDuplicate;		
}

void EngineeringSegment::Generate(int dwLevel)
{
	int randomNum				= GenerateRandomInt(TOTAL_ENGINEERING_ODDS - 1);
	ShipModifier *pModifier		= new ShipModifier();

	if (randomNum < FORTIFIED_ODDS)
	{
		m_Type = eTYPE_FORTIFIED;
	}
	else if (randomNum < (FORTIFIED_ODDS + BOOSTED_ODDS))
	{
		m_Type = eTYPE_BOOSTED;
	}
	else if (randomNum < (FORTIFIED_ODDS + BOOSTED_ODDS + CLONING_ODDS))
	{
		m_Type = eTYPE_CLONING;
	}
	else
	{
		m_Type = eTYPE_NO_TYPE; //something is messed up
	}

	switch (m_Type)
	{
	case eTYPE_FORTIFIED:
		pModifier->m_fArmor						= (float)dwLevel;			//1 armor increase per level
		pModifier->m_fPhysicalDamageReduction	= (float)dwLevel;
		break;
	
	case eTYPE_BOOSTED:
		pModifier->m_fShields					= (float)dwLevel * 3.0f;	//3 bonus shields per level
		pModifier->m_fShieldEfficiency			= (float)dwLevel  * 0.03f;	//3% bonus to shields efficiency/level
		pModifier->m_fEnergyDamageReduction		= (float)dwLevel;			
		break;

	case eTYPE_CLONING:
		pModifier->m_fCloningChance				= (float)dwLevel * 0.01f;	//1% chance a level
		break;

	default:

		break;

	}

	SetShipModifier(pModifier);
	pModifier->Release();
}

string EngineeringSegment::GetTypeString() const
{
	string sTypeString;

	switch (m_Type)
	{
	case eTYPE_FORTIFIED:
		sTypeString = "Fortified";
		break;
		
	case eTYPE_BOOSTED:
		sTypeString = "Boosted";
		break;

	case eTYPE_CLONING:
		sTypeString = "Cloning";
		break;

	default:
		sTypeString = "EngineeringSegment::GetTypeString default case... shuldn't hpn!";
		break;
	}

	return sTypeString;
}

void EngineeringSegment::Init()
{
	m_Type = eTYPE_NO_TYPE;
}