#include "DriftingMovementPlugin.h"

#include "Block.h"
#include "Game.h"
#include "MovementWaypoint.h"
#include "Node.h"
#include "PhysicalObject.h"
#include "PosQuat.h"
#include "IdleMovementTask.h"
#include "utility.h"

#include <iostream>
using namespace std;

DriftingMovementPlugin::DriftingMovementPlugin()
{
	Init();
}

DriftingMovementPlugin::~DriftingMovementPlugin()
{
	
}

void DriftingMovementPlugin::AssignMovementTask(PhysicalObject *pObject)
{
	IdleMovementTask *pTask			= new IdleMovementTask();
	pObject->SetMovementTask(pTask);
	pTask->Release();

	float xDest		= GenerateRandomFloat(GAME_SIZE_X) - (GAME_SIZE_X / 2.0f);
	float yDest		= -GenerateRandomFloat(GAME_SIZE_Y / 2.0f);
	float fSpeed	= 0.75f;

	//set the object moving in a random direction towards the bottom
	VECTOR3 vDir;
	vDir.x			= xDest - pObject->GetNode()->m_PosQuat.pos.x;
	vDir.y			= yDest - pObject->GetNode()->m_PosQuat.pos.y;
	vDir.z			= 0.0f;

	NormalizeVECTOR3(vDir);

	vDir.x			*= fSpeed;
	vDir.y			*= fSpeed;
	vDir.z			*= fSpeed;

	pObject->SetVelocity(vDir);
	pObject->SetVelocityDecay(false);
}

void DriftingMovementPlugin::FromBlock(Block *pBlock)
{
	
}

DriftingMovementPlugin *DriftingMovementPlugin::GetPlugin()
{
	static DriftingMovementPlugin *pPlugin = new DriftingMovementPlugin();
	return pPlugin;
}

void DriftingMovementPlugin::Init()
{
	
}

void DriftingMovementPlugin::Print(int indent) const
{
	printf("%*sDriftingMovementPlugin 0x%p (%s)\n", indent, "", this);
}

void DriftingMovementPlugin::Refresh(const float &fDeltaTime)
{
	
}

void DriftingMovementPlugin::SignalRemoved(PhysicalObject *pObject)
{
	
}

Block *DriftingMovementPlugin::ToBlock()
{
	return 0;
}	