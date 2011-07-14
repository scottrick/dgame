#include "Sector.h"

#include "ComputerShip.h"
#include "data.h"
#include "GalaxyEffect.h"
#include "Node.h"
#include "Obstacle.h"
#include "Player.h"
#include "PlayerShip.h"
#include "Scene.h"
#include "Spawner.h"
#include "utility.h"

Sector::Sector()
{
	Init();
	Generate(1); //level 1 sector is default, if non specified
}	

Sector::Sector(int dwLevel)
{
	Init();
	Generate(dwLevel);
}

Sector::~Sector()
{
	Clear();

	SetGalaxyEffect(0);
	SetScene(0);
}	

void Sector::Clear()
{	
	m_dwLevel			= 1; //default level
	m_sSectorName		= "NoName";

	ComputerShip						*pShip	= 0;
	for (int i = 0; i < NUM_SHIP_TYPES_PER_SECTOR; ++i)
	{
		pShip = m_ComputerShips[i];
		if (pShip)
		{
			pShip->Release();
			m_ComputerShips[i] = 0;
		}

		pShip = 0;
	}

	/*
	list<BossShip *>::iterator		iter	= m_vBossShips.begin();
	BossShip						*pShip	= 0;
	while (iter != m_vBossShips.end())
	{
		pShip = *iter;
		if (pShip)
		{
			pShip->Release();
			pShip = 0;
		}
	}
	m_vBossShips.clear();
	*/
}

void Sector::FromBlock(Block *pBlock)
{

}

void Sector::Generate(int dwLevel)
{
	Clear();

	m_dwLevel	= dwLevel;
	int partOne = GenerateRandomInt(sectorPartOneSize - 1);
	int partTwo = GenerateRandomInt(sectorPartTwoSize - 1);

	SetSectorName(sectorPartOne[partOne] + " " + sectorPartTwo[partTwo]);

	//populate the sector with three random computer ships
	for (int i = 0; i < NUM_SHIP_TYPES_PER_SECTOR; ++i)
	{
		ComputerShip *pNewShip = new ComputerShip();
		pNewShip->Generate(dwLevel);
		m_ComputerShips[i] = pNewShip;
	}

	GalaxyEffect *pNewGalaxyEffect = new GalaxyEffect();
	SetGalaxyEffect(pNewGalaxyEffect);
	pNewGalaxyEffect->Release();

	//Print(0);
}

ComputerShip *Sector::GetShip(unsigned int dwIndex)
{
	if (dwIndex < NUM_SHIP_TYPES_PER_SECTOR)
	{
		return m_ComputerShips[dwIndex];		
	}

	return 0;
}

void Sector::Init()
{
	m_dwLevel			= 1; //default level
	m_sSectorName		= "NoName";
	m_pGalaxyEffect		= 0;
	m_pScene			= 0;

	for (int i = 0; i < NUM_SHIP_TYPES_PER_SECTOR; ++i)
	{
		m_ComputerShips[i] = 0;
	}
}	

void Sector::Pause()
{
	m_bPlaying = false;
}

void Sector::Play()
{
	for (int i = 0; i < NUM_SHIP_TYPES_PER_SECTOR; ++i) 
	{
		Spawner *pSpawner = new Spawner(this, GetShip(i));
		//pSpawner->SetAverageDelay(10.0f);
		pSpawner->Randomize();
		m_pScene->AddObject(pSpawner);
		pSpawner->Release();
	}
	
	//if (GenerateRandomInt(1)) { //50% chance for obstacles
	if (true) { //always have obstacles
		Obstacle *pNewObstacle = new Obstacle();
		pNewObstacle->Generate(this->GetLevel());
		Spawner *pSpawner = new Spawner(this, pNewObstacle);
		pSpawner->SetAverageDelay(8.0f);
		pSpawner->Randomize();
		m_pScene->AddObject(pSpawner);
		pSpawner->Release();
		pNewObstacle->Release();
	}

	m_bPlaying = true;
}
	
void Sector::Print(int indent) const
{
	printf("%*sSector 0x%p \"%s\"\n", indent, " ", this, m_sSectorName.c_str());

	ComputerShip								*pShip		= 0;

	for (int i = 0; i < NUM_SHIP_TYPES_PER_SECTOR; ++i)
	{
		pShip = m_ComputerShips[i];
		if (pShip)
		{
			pShip->Print(indent + 2);
			pShip = 0;
		}
	}
}

void Sector::Refresh(const float &fDeltaTime)
{

}

void Sector::SetGalaxyEffect(GalaxyEffect *pGalaxyEffect)
{
	if (m_pGalaxyEffect != pGalaxyEffect)
	{
		if (m_pGalaxyEffect)
		{
			m_pGalaxyEffect->Release();
			m_pGalaxyEffect = 0;
		}

		if (pGalaxyEffect)
		{
			m_pGalaxyEffect = pGalaxyEffect;
			m_pGalaxyEffect->AddRef();
		}
	}
}

void Sector::SetScene(Scene *pScene)
{
	if (m_pScene != pScene)
	{
		if (m_pScene)
		{
			m_pScene->Release();
			m_pScene = 0;
		}

		if (pScene)
		{
			m_pScene = pScene;
			m_pScene->AddRef();
		}
	}
}

/*
void Sector::SetSectorName(char *pNewName)
{
	if (pNewName != m_pSectorName)
	{
		if (m_pSectorName)
		{
	//		free(m_pSectorName);
			m_pSectorName = 0;
		}

		if (pNewName)
		{
			m_pSectorName = pNewName;
		}
	}
}
*/

Block *Sector::ToBlock()
{
	return 0;
}