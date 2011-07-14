#include "Spawner.h"

#include "Game.h"
#include "Node.h"
#include "PhysicalObject.h"
#include "Player.h"
#include "Scene.h"
#include "Sector.h"
#include "utility.h"

Spawner::Spawner() 
{
	Init();
}

Spawner::Spawner(Sector *pSector, PhysicalObject *pObject) 
{
	Init();

	SetObjectToSpawn(pObject);
	SetSector(pSector);
}

Spawner::~Spawner()
{
	SetObjectToSpawn(0);
	SetSector(0);
}

void Spawner::FromBlock(Block *pBlock) 
{

}	

void Spawner::Init() 
{
	m_bRepeating		= true;
	m_fAverageDelay		= 1.035f; //default delay
	m_pObjectToSpawn	= 0;
	m_pSector			= 0;
}

void Spawner::Randomize()
{
	m_dwNumRemaining	= (GenerateRandomInt(3) + 1) * 5;
	//m_fCurrentDelay		= GenerateRandomFloat(1.6f) + 1.0f;
	m_fCurrentDelay		= GenerateRandomFloat(m_fAverageDelay * 0.4f) + (m_fAverageDelay * 0.8f);
	m_fDelay			= GenerateRandomFloat(m_fAverageDelay * 0.4f) + (m_fAverageDelay * 0.8f);
	m_fXLoc				= GenerateRandomFloat(GAME_SIZE_X * 0.8f) - (GAME_SIZE_X * 0.8f) / 2.0f;
	m_fXChange			= GenerateRandomFloat(1.5f) - 0.75f;
}

void Spawner::Refresh(const float &fDeltaTime) 
{
	if (m_pSector->GetScene()->GetComputerShipCount() >= MAX_NUMBER_OF_COMPUTER_SHIPS && m_pObjectToSpawn->GetPhysicalObjectType() == PhysicalObject::eCOMPUTER_SHIP)
	{ //don't spawn if there are too many computer ships and the object to spawn in a computer ship
		return;
	}

	//do spawning here, if the time is right
	m_fCurrentDelay -= fDeltaTime;
	
	if (m_fCurrentDelay <= 0.0f && gGame()->GetPlayer()->GetPlayerObject()->IsAlive())
	{ //TIME TO SPAWN something...
		if (m_dwNumRemaining > 0) 
		{
			PhysicalObject *pNewObject = m_pObjectToSpawn->Duplicate();
			pNewObject->SetTarget(gGame()->GetPlayer()->GetPlayerObject());
			pNewObject->GetNode()->m_PosQuat.pos.y	= GAME_SIZE_Y / 2.0f + pNewObject->GetNode()->GetScale().x + 0.1f;
			pNewObject->GetNode()->m_PosQuat.pos.x	= m_fXLoc;
			m_pSector->GetScene()->AddObject(pNewObject);
			pNewObject->Release();

			m_fCurrentDelay = m_fDelay;
			m_fXLoc += m_fXChange;
			m_dwNumRemaining--;
		}
		else
		{ //everything has been spawned, so set a larger delay and reset for next batch
			Randomize();
		}
	} 
	else 
	{ //not time to spawn yet...
		
	}
}

void Spawner::SetAverageDelay(float fAverageDelay)
{
	m_fAverageDelay = fAverageDelay;
}

void Spawner::SetObjectToSpawn(PhysicalObject *pObject)
{
	if (m_pObjectToSpawn != pObject)
	{
		if (m_pObjectToSpawn)
		{
			m_pObjectToSpawn->Release();
			m_pObjectToSpawn = 0;
		}

		if (pObject) 
		{
			m_pObjectToSpawn = pObject;
			m_pObjectToSpawn->AddRef();
		}
	}
}

void Spawner::SetRepeating(bool bRepeating)
{
	m_bRepeating = bRepeating;
}

void Spawner::SetSector(Sector *pSector)
{
	if (m_pSector != pSector)
	{
		if (m_pSector)
		{
			m_pSector->Release();
			m_pSector = 0;
		}

		if (pSector) 
		{
			m_pSector = pSector;
			m_pSector->AddRef();
		}
	}
}

Block *Spawner::ToBlock()
{
	return 0;
}