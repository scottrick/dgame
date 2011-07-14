#include "ComputerShip.h"

#include "ActionScene.h"
#include "CommandSegment.h"
#include "data.h"
#include "EngineeringSegment.h"
#include "FiringPlugin.h"
#include "FiringTask.h"
#include "Game.h"
#include "HomingMovementPlugin.h"
#include "MovementPlugin.h"
#include "MovementTask.h"
#include "Node.h"
#include "PlayerShip.h"
#include "Projectile.h"
#include "PropulsionSegment.h"
#include "Segment.h"
#include "ShipModifier.h"
#include "Shockwave.h"
#include "utility.h"
#include "Weapon.h"

ComputerShip::ComputerShip()
{
	Init();
}

ComputerShip::~ComputerShip()
{
	SetCommandSegment(0);
	SetEngineeringSegment(0);
	SetPropulsionSegment(0);
}

void ComputerShip::Clone(ComputerShip *pClone)
{
	PhysicalObject::Clone(pClone);

	SetCommandSegment(pClone->GetCommandSegment());
	SetEngineeringSegment(pClone->GetEngineeringSegment());
	SetPropulsionSegment(pClone->GetPropulsionSegment());
	SetShipName(pClone->GetShipName());
}

void ComputerShip::Collide(PhysicalObject *pOtherObject)
{
	switch (pOtherObject->GetPhysicalObjectType())
	{
	case PhysicalObject::ePROJECTILE:
		{
			Projectile *pProj = (Projectile *)pOtherObject;
			ApplyDamage(pProj->GetWeapon()->GetDamage(), pProj->GetCreator());
		}
		break;

	case PhysicalObject::ePLAYER_SHIP:
		ApplyOnePhysicalDamage(pOtherObject);
		break;

	default:
		
		break;

	}
}

PhysicalObject *ComputerShip::Duplicate()
{
	ComputerShip *pDuplicate = new ComputerShip();
	pDuplicate->Clone(this);
	return pDuplicate;	
}

void ComputerShip::FromBlock(Block *pBlock)
{
	
}

void ComputerShip::Generate(int dwLevel)
{
	CommandSegment *pCommand = new CommandSegment();
	pCommand->Generate(dwLevel);
	SetCommandSegment(pCommand);
	pCommand->Release();

	EngineeringSegment *pEngineering = new EngineeringSegment();
	pEngineering->Generate(dwLevel);
	SetEngineeringSegment(pEngineering);
	pEngineering->Release();

	PropulsionSegment *pPropulsion = new PropulsionSegment();

	if (pCommand->GetCommandSegmentType() == CommandSegment::eTYPE_KAMIKAZE)
	{ //force conventional engine if type kamikaze
		pPropulsion->Generate(dwLevel, PropulsionSegment::eTYPE_CONVENTIONAL);		
		pPropulsion->SetMovementPlugin(HomingMovementPlugin::GetPlugin());
	}
	else
	{ //random propulsion segment type
		pPropulsion->Generate(dwLevel);
	}

	SetPropulsionSegment(pPropulsion);
	pPropulsion->Release();

	//create random name
	int partOne = GenerateRandomInt(shipPartOneSize - 1);
	int partTwo = GenerateRandomInt(shipPartTwoSize - 1);

	//assign a default weapon for now
	Weapon *pNewWeaponOne = new Weapon();
	pNewWeaponOne->SetCooldown(1.0f);
	//Weapon *pNewWeaponTwo = new Weapon();
	SetWeaponOne(pNewWeaponOne);
	//SetWeaponTwo(pNewWeaponTwo);
	pNewWeaponOne->SetOwner(this);
	//pNewWeaponTwo->SetOwner(this);
	pNewWeaponOne->Release();
	//pNewWeaponTwo->Release();

	SetShipName(shipPartOne[partOne] + " " + shipPartTwo[partTwo]);

	//set structure/shields
	m_fStructureMax				= (float) (GenerateRandomInt(dwLevel) + dwLevel);
	m_fStructure				= m_fStructureMax;
	m_fShieldsMax				= (float) dwLevel;
	m_fShields					= m_fShieldsMax;
}

FiringPlugin *ComputerShip::GetFiringPlugin() const
{
	if (m_pCommandSegment)
	{
		return m_pCommandSegment->GetFiringPlugin();
	}
	else
	{
		return 0;
	}
}

MovementPlugin *ComputerShip::GetMovementPlugin() const
{
	if (m_pPropulsionSegment)
	{
		return m_pPropulsionSegment->GetMovementPlugin();
	}
	else
	{
		return 0;
	}
}

Shockwave *ComputerShip::GetShockwave()
{
	return 0;

	/*
	int dwRandom = GenerateRandomInt(5);
	if (dwRandom) 
	{
		return 0;
	}

	return new Shockwave(this, false);
	*/
}

void ComputerShip::Init()
{
	SetPhysicalObjectType(PhysicalObject::eCOMPUTER_SHIP);

	m_pCommandSegment		= 0;
	m_pEngineeringSegment	= 0;
	m_pPropulsionSegment	= 0;
	m_pFiringTask			= 0;
	m_pMovementTask			= 0;
	m_sShipName				= "NoShipName";

	//just give it a temporary visual for now
	Node *pNode = new Node();
	pNode->SetVisual("data\\visuals\\a_stockermans\\fighter.3ds");
	SetNode(pNode);
	pNode->Release();

	//some random orientation?
	//pNode->m_PosQuat.quat.CreateFromRotationRADIANS(GenerateRandomFloat(2.0f), 0.0f, 0.0f, 1.0f);

	m_bVelocityDecay		= true;
}

void ComputerShip::OnDeath()
{
	PhysicalObject::OnDeath();
}

void ComputerShip::Print(int indent) const
{
	printf("%*sComputerShip (%s, %s, %s) \"%s\"\n", indent, " ", 
		m_pCommandSegment->GetTypeString().c_str(), 
		m_pEngineeringSegment->GetTypeString().c_str(), 
		m_pPropulsionSegment->GetTypeString().c_str(), 
		m_sShipName.c_str());

	ShipModifier *pModifiers = new ShipModifier();
	pModifiers->AddModifier(m_pCommandSegment->GetShipModifier());
	pModifiers->AddModifier(m_pEngineeringSegment->GetShipModifier());
	pModifiers->AddModifier(m_pPropulsionSegment->GetShipModifier());
	pModifiers->Print(indent + 2);
	pModifiers->Release();
}

void ComputerShip::Refresh(const float &fDeltaTime)
{
	PhysicalObject::Refresh(fDeltaTime);

	if (!m_pMovementTask || m_pMovementTask->IsComplete())
	{
		if (m_pPropulsionSegment->GetMovementPlugin())
		{
			m_pPropulsionSegment->GetMovementPlugin()->AssignMovementTask(this);
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

	if (!m_pFiringTask || m_pFiringTask->IsComplete())
	{
		if (m_pCommandSegment->GetFiringPlugin())
		{
			m_pCommandSegment->GetFiringPlugin()->AssignFiringTask(this);
		}
		else
		{
			//assign some idle firing task!?  
		}
	}
	else
	{
		m_pFiringTask->ApplyTask(this, fDeltaTime);
	}
}	

void ComputerShip::SetCommandSegment(CommandSegment *pSegment)
{
	if (m_pCommandSegment != pSegment)
	{
		if (m_pCommandSegment)
		{
			m_pCommandSegment->Release();
			m_pCommandSegment = 0;
		}	

		if (pSegment)
		{
			m_pCommandSegment = pSegment;
			m_pCommandSegment->AddRef();
		}
	}
}

void ComputerShip::SetEngineeringSegment(EngineeringSegment *pSegment)
{
	if (m_pEngineeringSegment != pSegment)
	{
		if (m_pEngineeringSegment)
		{
			m_pEngineeringSegment->Release();
			m_pEngineeringSegment = 0;
		}	

		if (pSegment)
		{
			m_pEngineeringSegment = pSegment;
			m_pEngineeringSegment->AddRef();
		}
	}
}

void ComputerShip::SetFiringPlugin(FiringPlugin *pPlugin)
{
	if (m_pCommandSegment)
	{
		m_pCommandSegment->SetFiringPlugin(pPlugin);
	}
}

/*
void ComputerShip::SetFiringTask(FiringTask *pTask)
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
*/

void ComputerShip::SetMovementPlugin(MovementPlugin *pPlugin)
{
	if (m_pPropulsionSegment)
	{
		m_pPropulsionSegment->SetMovementPlugin(pPlugin);
	}
}

/*
void ComputerShip::SetMovementTask(MovementTask *pTask)
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
*/

void ComputerShip::SetPropulsionSegment(PropulsionSegment *pSegment)
{
	if (m_pPropulsionSegment != pSegment)
	{
		if (m_pPropulsionSegment)
		{
			m_pPropulsionSegment->Release();
			m_pPropulsionSegment = 0;
		}	

		if (pSegment)
		{
			m_pPropulsionSegment = pSegment;
			m_pPropulsionSegment->AddRef();
		}
	}
}

Block *ComputerShip::ToBlock()
{
	return 0;
}