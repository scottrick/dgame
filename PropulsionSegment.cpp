#include "PropulsionSegment.h"

#include "EvasiveMovementPlugin.h"
#include "IdleMovementPlugin.h"
#include "JumpMovementPlugin.h"
#include "ParallelMovementPlugin.h"
#include "PatternMovementPlugin.h"
#include "RandomMovementPlugin.h"
#include "ShipModifier.h"
#include "StationaryMovementPlugin.h"
#include "utility.h"

PropulsionSegment::PropulsionSegment()
{
	Init();
}

PropulsionSegment::~PropulsionSegment()
{
	SetMovementPlugin(0);
}

void PropulsionSegment::Clone(PropulsionSegment *pClone) 
{
	Segment::Clone(pClone);

	SetPropulsionSegmentType(pClone->GetPropulsionSegmentType());
}

PhysicalObject *PropulsionSegment::Duplicate()
{
	PropulsionSegment *pDuplicate = new PropulsionSegment();
	pDuplicate->Clone(this);
	return pDuplicate;		
}

void PropulsionSegment::Generate(int dwLevel)
{
	int randomNum				= GenerateRandomInt(TOTAL_PROPULSION_ODDS - 1);

	if (randomNum < CONVENTIONAL_ODDS)
	{
		m_Type = eTYPE_CONVENTIONAL;
	}
	else if (randomNum < (CONVENTIONAL_ODDS + JUMP_ODDS))
	{
		m_Type = eTYPE_JUMP;
	}
	else if (randomNum < (CONVENTIONAL_ODDS + JUMP_ODDS + STATIONARY_ODDS))
	{
		m_Type = eTYPE_STATIONARY;
	}
	else
	{
		m_Type = eTYPE_NO_TYPE; //something is messed up
	}

	GenerateShipModifiers(dwLevel);
}	

void PropulsionSegment::Generate(int dwLevel, PropulsionSegmentType type)
{
	m_Type = type;
	GenerateShipModifiers(dwLevel);
}

void PropulsionSegment::GenerateShipModifiers(int dwLevel)
{
	ShipModifier *pModifier		= new ShipModifier();

	switch (m_Type)
	{
	case eTYPE_CONVENTIONAL:
		{
			pModifier->m_fMovement				= (float)dwLevel * 0.03f;	//3% increase per ship level
			
			int dwType = GenerateRandomInt(4);

			switch (dwType)
			{
			case 0:
				SetMovementPlugin(RandomMovementPlugin::GetPlugin());
				break;
			case 1:
				SetMovementPlugin(ParallelMovementPlugin::GetPlugin());
				break;
			case 2:
			case 3:
				SetMovementPlugin(PatternMovementPlugin::GetPlugin());
				break;
			case 4:
				SetMovementPlugin(EvasiveMovementPlugin::GetPlugin());
				break;
			default:
				printf("PropulsionSegment GenerateShipModifiers DEFAULT CASE shouldn't happen...\n");
				break;
			}
		}

		break;
	
	case eTYPE_JUMP:
		pModifier->m_fJumpFrequency			= (float)dwLevel;
		pModifier->m_fRateOfFire			= (float)dwLevel * 0.02f;
		SetMovementPlugin(JumpMovementPlugin::GetPlugin());
		break;

	case eTYPE_STATIONARY:
		pModifier->m_fStructure				= (float)dwLevel * 10.0f;
		pModifier->m_fArmor					= (float)dwLevel * 3.0f;
		SetMovementPlugin(StationaryMovementPlugin::GetPlugin());
		break;

	default:

		break;

	}

	SetShipModifier(pModifier);
	pModifier->Release();
}

string PropulsionSegment::GetTypeString() const
{
	string sTypeString;

	switch (m_Type)
	{
	case eTYPE_CONVENTIONAL:
		sTypeString = "Conventional";
		break;
		
	case eTYPE_JUMP:
		sTypeString = "Jump";
		break;

	case eTYPE_STATIONARY:
		sTypeString = "Stationary";
		break;

	default:
		sTypeString = "PropulsionSegment::GetTypeString default case... shuldn't hpn!";
		break;
	}

	return sTypeString;}

void PropulsionSegment::Init()
{
	m_Type					= eTYPE_NO_TYPE;
	m_pMovementPlugin		= 0;
}