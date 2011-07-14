#include "PatternMovementPlugin.h"

#include "Game.h"
#include "MovementWaypoint.h"
#include "PhysicalObject.h"
#include "StandardMovementTask.h"
#include "utility.h"

#include <iostream>
using namespace std;

PatternMovementPlugin::PatternMovementPlugin()
{
	Init();
}

PatternMovementPlugin::~PatternMovementPlugin()
{
	ClearPattern();
}

void PatternMovementPlugin::AssignMovementTask(PhysicalObject *pObject)
{
	if ((m_dwPatternAssignmentCount % NUMBER_SHIPS_PER_PATTERN) == 0)
	{ //need a new pattern!
		GeneratePattern();
		m_dwPatternAssignmentCount = 1;
	}
	else
	{
		m_dwPatternAssignmentCount++;
	}

	StandardMovementTask *pTask		= new StandardMovementTask();
	pTask->SetRepeating(true);

	vector<MovementWaypoint *>::iterator iter = m_vPattern.begin();
	for (; iter != m_vPattern.end(); iter++)
	{
		MovementWaypoint *pWaypoint = new MovementWaypoint();
		pWaypoint->vDest = (*iter)->vDest;
		pTask->AddMovementWaypoint(pWaypoint);
		pWaypoint->Release();
	}

	pObject->SetMovementTask(pTask);
	pTask->Release();
}

void PatternMovementPlugin::ClearPattern()
{
	MovementWaypoint *pWaypoint = 0;
	vector<MovementWaypoint *>::iterator iter = m_vPattern.begin();
	while (iter != m_vPattern.end())
	{
		pWaypoint = *iter;

		if (pWaypoint)
		{
			pWaypoint->Release();
			pWaypoint = 0;
		}
		
		iter++;
	}

	m_vPattern.clear();
}

void PatternMovementPlugin::FromBlock(Block *pBlock)
{
	
}

void PatternMovementPlugin::GeneratePattern()
{
	ClearPattern(); //remove any existing pattern

	int		dwRandom	= GenerateRandomInt(1);
	float	fXDest;
	float	fYDest;

	switch (dwRandom)
	{
	case 0: //generate random 3 waypoint pattern
		{
			MovementWaypoint *pPointOne		= new MovementWaypoint();
			MovementWaypoint *pPointTwo		= new MovementWaypoint();
			MovementWaypoint *pPointThree	= new MovementWaypoint();

			//create point in the middle top region
			fXDest				= GenerateRandomFloat(GAME_SIZE_X / 2.0f) - (GAME_SIZE_X / 4.0f);
			fYDest				= (GAME_SIZE_Y * 7.0f / 16.0f) - GenerateRandomFloat(GAME_SIZE_Y / 3.0f);
			pPointOne->vDest	= VECTOR3(fXDest, fYDest, 0.0f);
			
			//create point in the lower right region
			fXDest				= (GAME_SIZE_X * 7.0f / 16.0f) - GenerateRandomFloat(GAME_SIZE_X / 3.0f);
			fYDest				= (GAME_SIZE_Y / 16.0f) - GenerateRandomFloat(GAME_SIZE_Y / 2.0f);
			pPointTwo->vDest	= VECTOR3(fXDest, fYDest, 0.0f);

			//create point in the lower left region
			fXDest				= (GAME_SIZE_X * 7.0f / -16.0f) + GenerateRandomFloat(GAME_SIZE_X / 3.0f);
			fYDest				= (GAME_SIZE_Y / 16.0f) - GenerateRandomFloat(GAME_SIZE_Y / 2.0f);
			pPointThree->vDest	= VECTOR3(fXDest, fYDest, 0.0f);

			if (GenerateRandomInt(1))
			{ //clockwise pattern
				m_vPattern.push_back(pPointTwo);
				m_vPattern.push_back(pPointThree);
				m_vPattern.push_back(pPointOne);
			}
			else
			{ //counter clockwise
				m_vPattern.push_back(pPointThree);
				m_vPattern.push_back(pPointTwo);
				m_vPattern.push_back(pPointOne);
			}
		}
		break;

	case 1: //generate 4 waypoint pattern
		{
			MovementWaypoint *pPointOne		= new MovementWaypoint();
			MovementWaypoint *pPointTwo		= new MovementWaypoint();
			MovementWaypoint *pPointThree	= new MovementWaypoint();
			MovementWaypoint *pPointFour	= new MovementWaypoint();

			//create point in the top left region
			fXDest				= GenerateRandomFloat(GAME_SIZE_X * 3.0f / 8.0f) - (GAME_SIZE_X * 7.0f / 16.0f);
			fYDest				= GenerateRandomFloat(GAME_SIZE_Y * 3.0f / 8.0f) + (GAME_SIZE_Y / 16.0f);
			pPointOne->vDest	= VECTOR3(fXDest, fYDest, 0.0f);
			
			//create point in the top right region
			fXDest				= GenerateRandomFloat(GAME_SIZE_X * 3.0f / 8.0f) + (GAME_SIZE_X / 16.0f);
			fYDest				= GenerateRandomFloat(GAME_SIZE_Y * 3.0f / 8.0f) + (GAME_SIZE_Y / 16.0f);
			pPointTwo->vDest	= VECTOR3(fXDest, fYDest, 0.0f);

			//create point in the bottom right region
			fXDest				= GenerateRandomFloat(GAME_SIZE_X * 3.0f / 8.0f) + (GAME_SIZE_X / 16.0f);
			fYDest				= GenerateRandomFloat(GAME_SIZE_Y * 3.0f / 8.0f) - (GAME_SIZE_Y * 7.0f / 16.0f);
			pPointThree->vDest	= VECTOR3(fXDest, fYDest, 0.0f);

			//create point in the bottom left region
			fXDest				= GenerateRandomFloat(GAME_SIZE_X * 3.0f / 8.0f) - (GAME_SIZE_Y * 7.0f / 16.0f);
			fYDest				= GenerateRandomFloat(GAME_SIZE_Y * 3.0f / 8.0f) - (GAME_SIZE_Y * 7.0f / 16.0f);
			pPointFour->vDest	= VECTOR3(fXDest, fYDest, 0.0f);

			if (GenerateRandomInt(1))
			{ //clockwise pattern
				m_vPattern.push_back(pPointThree);
				m_vPattern.push_back(pPointFour);
				m_vPattern.push_back(pPointOne);
				m_vPattern.push_back(pPointTwo);
			}
			else
			{ //counter clockwise
				m_vPattern.push_back(pPointFour);
				m_vPattern.push_back(pPointThree);
				m_vPattern.push_back(pPointTwo);
				m_vPattern.push_back(pPointOne);
			}
		}
		break;

	default:
		//shouldn't hit this...
		printf("PatternMovementPlugin::GeneratePattern ---> SHOULD NOT HIT DEFAULT!\n");
		break;
	}
}

PatternMovementPlugin *PatternMovementPlugin::GetPlugin()
{
	static PatternMovementPlugin *pPlugin = new PatternMovementPlugin();
	return pPlugin;
}

void PatternMovementPlugin::Init()
{
	m_dwPatternAssignmentCount = 0;
}

void PatternMovementPlugin::Print(int indent) const
{
	printf("%*sPatternMovementPlugin 0x%p (%s)\n", indent, "", this);
}

void PatternMovementPlugin::Refresh(const float &fDeltaTime)
{
	
}

void PatternMovementPlugin::SignalRemoved(PhysicalObject *pObject)
{
	
}

Block *PatternMovementPlugin::ToBlock()
{
	return 0;
}