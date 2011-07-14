#include "EvasiveMovementPlugin.h"

#include "def.h"
#include "Game.h"
#include "MovementWaypoint.h"
#include "Node.h"
#include "PhysicalObject.h"
#include "PosQuat.h"
#include "RandomMovementPlugin.h"
#include "StandardMovementTask.h"
#include "utility.h"

#include <iostream>
using namespace std;

float EvasiveMovementPlugin::m_fEvasiveDistanceSquared = (GAME_SIZE_X / 3.5f) * (GAME_SIZE_X / 3.5f);

EvasiveMovementPlugin::EvasiveMovementPlugin()
{
	Init();
}

EvasiveMovementPlugin::~EvasiveMovementPlugin()
{

}

void EvasiveMovementPlugin::AssignMovementTask(PhysicalObject *pObject)
{
	//first check to see if this object is even on the board.  if it isn't, move to some random location
	if (
		(pObject->GetNode()->m_PosQuat.pos.x < GAME_SIZE_X / -2.0f) ||
		(pObject->GetNode()->m_PosQuat.pos.x > GAME_SIZE_X / 2.0f) ||
		(pObject->GetNode()->m_PosQuat.pos.y < GAME_SIZE_Y / -2.0f) ||
		(pObject->GetNode()->m_PosQuat.pos.y > GAME_SIZE_Y / 2.0f))
	{
		RandomMovementPlugin::GetPlugin()->AssignMovementTask(pObject);
		return;
	}

	VECTOR3 vCurrentDistance = VECTOR3(
		pObject->GetNode()->m_PosQuat.pos.x - pObject->GetTarget()->GetNode()->m_PosQuat.pos.x,
		pObject->GetNode()->m_PosQuat.pos.y - pObject->GetTarget()->GetNode()->m_PosQuat.pos.y,
		pObject->GetNode()->m_PosQuat.pos.z - pObject->GetTarget()->GetNode()->m_PosQuat.pos.z);
	
	float fCurrrentDistance = vCurrentDistance.x * vCurrentDistance.x + vCurrentDistance.y * vCurrentDistance.y + vCurrentDistance.z * vCurrentDistance.z;

	if (fCurrrentDistance > m_fEvasiveDistanceSquared)
	{ //Not close enough to try to evade, so just throttle down...
		pObject->SetThrottle(0.0f);
		return;
	}

	//TOO CLOSE!  So create a random movement direction to evade
	float		fRandomX		= GenerateRandomFloat(3.0f) - 1.5f;
	float		fRandomY		= GenerateRandomFloat(3.0f) - 1.5f;
	VECTOR3		vMoveDir		= VECTOR3(fRandomX, fRandomY, 0.0f);
	NormalizeVECTOR3(vMoveDir);

	VECTOR3		vNewDistance	= VECTOR3(vMoveDir.x + vCurrentDistance.x, vMoveDir.y + vCurrentDistance.y, vMoveDir.z + vCurrentDistance.z);
	float		fNewDistance	= vNewDistance.x * vNewDistance.x + vNewDistance.y * vNewDistance.y + vNewDistance.z * vNewDistance.z;
	
	if (fNewDistance < fCurrrentDistance)
	{ //new direction is not moving away, so reverse x value
		vMoveDir.x = -vMoveDir.x;

		vNewDistance.x = vMoveDir.x + vCurrentDistance.x;
		fNewDistance = vNewDistance.x * vNewDistance.x + vNewDistance.y * vNewDistance.y + vNewDistance.z * vNewDistance.z;

		if (fNewDistance < fCurrrentDistance)
		{ //still not closer, so reverse the y value
			vMoveDir.y = -vMoveDir.y;

			vNewDistance.y = vMoveDir.y + vCurrentDistance.y;
			fNewDistance = vNewDistance.x * vNewDistance.x + vNewDistance.y * vNewDistance.y + vNewDistance.z * vNewDistance.z;

			if (fNewDistance < fCurrrentDistance)
			{
				printf("EvasiveMovementPlugin::Assign ----> unable to create a direction further away... this is bad\n");
			}
		}
	}

	float xDest		= pObject->GetNode()->m_PosQuat.pos.x + vMoveDir.x;
	float yDest		= pObject->GetNode()->m_PosQuat.pos.y + vMoveDir.y;
	float xSizeMod	= (GAME_SIZE_X - 2.5f) / 2.0f;
	float ySizeMod	= (GAME_SIZE_Y - 2.5f) / 2.0f;

	//make sure this location is on the board, if not just give it a randomly generated move
	if ((xDest > xSizeMod) || (xDest < -xSizeMod) || (yDest > ySizeMod) || (yDest < -ySizeMod))
	{
		RandomMovementPlugin::GetPlugin()->AssignMovementTask(pObject);
		return;
	}

	//create waypoint for this move
	MovementWaypoint *pWaypoint = new MovementWaypoint();
	VECTOR3 vDest = VECTOR3(xDest, yDest, pObject->GetNode()->m_PosQuat.pos.z + vMoveDir.z);
	pWaypoint->vDest = vDest;

	StandardMovementTask *pTask = new StandardMovementTask();
	pTask->SetRepeating(false);
	
	//set waypoint
	pTask->AddMovementWaypoint(pWaypoint);
	pWaypoint->Release();
	
	pObject->SetMovementTask(pTask);
	pTask->Release();
}

void EvasiveMovementPlugin::FromBlock(Block *pBlock)
{
	
}

EvasiveMovementPlugin *EvasiveMovementPlugin::GetPlugin()
{
	static EvasiveMovementPlugin *pPlugin = new EvasiveMovementPlugin();
	return pPlugin;
}

void EvasiveMovementPlugin::Init()
{

}

void EvasiveMovementPlugin::Print(int indent) const
{
	printf("%*sEvasiveMovementPlugin 0x%p (%s)\n", indent, "", this);
}

void EvasiveMovementPlugin::Refresh(const float &fDeltaTime)
{
	
}

void EvasiveMovementPlugin::SignalRemoved(PhysicalObject *pObject)
{
	
}

Block *EvasiveMovementPlugin::ToBlock()
{
	return 0;
}	