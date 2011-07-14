#include "CommandSegment.h"

#include "AimedFiringPlugin.h"
#include "BlindFiringPlugin.h"
#include "IdleFiringPlugin.h"
#include "ShipModifier.h"
#include "utility.h"

CommandSegment::CommandSegment()
{
	Init();
}

CommandSegment::~CommandSegment()
{
	SetFiringPlugin(0);
}

void CommandSegment::Clone(CommandSegment *pClone) 
{
	Segment::Clone(pClone);

	SetCommandSegmentType(pClone->GetCommandSegmentType());
}

PhysicalObject *CommandSegment::Duplicate()
{
	CommandSegment *pDuplicate = new CommandSegment();
	pDuplicate->Clone(this);
	return pDuplicate;		
}

void CommandSegment::Generate(int dwLevel)
{
	int randomNum				= GenerateRandomInt(TOTAL_COMMAND_ODDS - 1);
	ShipModifier *pModifier		= new ShipModifier();

	if (randomNum < AIMED_ODDS)
	{
		m_Type = eTYPE_AIMED;
	}
	else if (randomNum < (AIMED_ODDS + BLIND_ODDS))
	{
		m_Type = eTYPE_BLIND;
	}
	else if (randomNum < (AIMED_ODDS + BLIND_ODDS + KAMIKAZE_ODDS))
	{
		m_Type = eTYPE_KAMIKAZE;
	}
	else
	{
		m_Type = eTYPE_NO_TYPE; //something is messed up
	}

	MovementPlugin *pPlugin = 0;

	switch (m_Type)
	{
	case eTYPE_AIMED:
		pModifier->m_fProjectileSpeed		= (float)dwLevel * 0.03f;	//3% increase per ship level
		SetFiringPlugin(AimedFiringPlugin::GetPlugin());
		break;
	
	case eTYPE_BLIND:
		pModifier->m_fProjectileSplitting	= (float)dwLevel * 0.03f;	//3% increase per ship level
		SetFiringPlugin(BlindFiringPlugin::GetPlugin());
		break;

	case eTYPE_KAMIKAZE:
		pModifier->m_fMovement				= (float)dwLevel * 0.03f;	//3% increase per ship level
		break;

	default:

		break;
	}

	SetShipModifier(pModifier);
	pModifier->Release();
}

string CommandSegment::GetTypeString() const
{
	string sTypeString;

	switch (m_Type)
	{
	case eTYPE_AIMED:
		sTypeString = "Aimed";
		break;
		
	case eTYPE_BLIND:
		sTypeString = "Blind";
		break;

	case eTYPE_KAMIKAZE:
		sTypeString = "Kamikaze";
		break;

	default:
		sTypeString = "CommandSegment::GetTypeString default case... shuldn't hpn!";
		break;
	}

	return sTypeString;
}

void CommandSegment::Init()
{
	m_Type				= eTYPE_NO_TYPE;	
	m_pFiringPlugin		= 0;
}