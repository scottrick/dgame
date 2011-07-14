#include "Player.h"

#include "Block.h"
#include "PhysicalObject.h"
#include "PlayerShip.h"
#include "Sector.h"
#include "utility.h"

#include <iostream>
#include <list>
using namespace std;

Player::Player()
{
	Init();
}

Player::~Player()
{
	ClearSectors();
	SetPlayerObject(0);
	SetTargetSector(0);
}

void Player::ClearSectors()
{
	list<Sector *>::iterator iter = m_lSectors.begin();
	while (iter != m_lSectors.end())
	{
		Sector *pSector = *iter;
		if (pSector)
		{
			pSector->Release();	
			pSector = 0;
		}
		iter++;
	}
	m_lSectors.clear();
}	

void Player::FromBlock(Block *pBlock)
{
	
}

Sector *Player::GetSector(unsigned int dwIndex) const
{
	list<Sector *>::const_iterator iter = m_lSectors.begin();
	int count = 0;
	while (iter != m_lSectors.end())
	{
		if (dwIndex == count)
		{
			return *iter;
		}
		else
		{
			iter++;
			count++;
		}
	}

	//index was too high!
	return 0;
}

void Player::Init()
{
	int dwNumSectors = GenerateRandomInt(7) + 8;	
	for (int i = 1; i < dwNumSectors; ++i)
	{
		Sector *pNewSector = new Sector(i);
		m_lSectors.push_back(pNewSector);
	}

	m_pPlayerObject = 0;

	//for now just give the player a basic ship
	PlayerShip *pPlayerShip = new PlayerShip();
	SetPlayerObject(pPlayerShip);
	pPlayerShip->Release();

	m_pTargetSector = 0;
	SetTargetSector(*(m_lSectors.begin()));
}

void Player::Print(int indent) const
{
	printf("%*sPlayer 0x%p\n", indent, " ", this);
}

void Player::Refresh(const float &fDeltaTime)
{

}

void Player::SetPlayerObject(PhysicalObject *pObject)
{
	if (m_pPlayerObject != pObject)
	{
		if (m_pPlayerObject)
		{
			m_pPlayerObject->Release();
			m_pPlayerObject = 0;
		}

		if (pObject)
		{
			m_pPlayerObject = pObject;
			m_pPlayerObject->AddRef();
		}
	}
}

void Player::SetTargetSector(Sector *pSector)
{
	if (m_pTargetSector != pSector)
	{
		if (m_pTargetSector)
		{
			m_pTargetSector->Release();
			m_pTargetSector = 0;
		}

		if (pSector)
		{
			m_pTargetSector = pSector;
			m_pTargetSector->AddRef();
		}
	}
}

Block *Player::ToBlock()
{
	return 0;
}