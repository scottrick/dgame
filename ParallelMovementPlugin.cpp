#include "ParallelMovementPlugin.h"

#include "Game.h"
#include "MovementWaypoint.h"
#include "Node.h"
#include "PhysicalObject.h"
#include "PosQuat.h"
#include "StandardMovementTask.h"
#include "utility.h"

#include <iostream>
using namespace std;

ParallelMovementPlugin::ParallelMovementPlugin()
{
	Init();
}

ParallelMovementPlugin::~ParallelMovementPlugin()
{

}

void ParallelMovementPlugin::AssignMovementTask(PhysicalObject *pObject)
{
	StandardMovementTask *pTask		= new StandardMovementTask();
	pTask->SetRepeating(false);

	float fXDest					= pObject->GetTarget()->GetNode()->m_PosQuat.pos.x;
	float fYDest					= pObject->GetTarget()->GetNode()->m_PosQuat.pos.y;
	fYDest							= fYDest + 2.0f + GenerateRandomFloat((GAME_SIZE_Y / 2.0f) - fYDest);
	
	MovementWaypoint *pWaypoint		= new MovementWaypoint();
	pWaypoint->vDest				= VECTOR3(fXDest, fYDest, 0.0f);

	pTask->AddMovementWaypoint(pWaypoint);
	pWaypoint->Release();

	pObject->SetMovementTask(pTask);
	pTask->Release();
}

void ParallelMovementPlugin::FromBlock(Block *pBlock)
{
	
}

ParallelMovementPlugin *ParallelMovementPlugin::GetPlugin()
{
	static ParallelMovementPlugin *pPlugin = new ParallelMovementPlugin();
	return pPlugin;
}

void ParallelMovementPlugin::Init()
{
	
}

void ParallelMovementPlugin::Print(int indent) const
{
	printf("%*sParallelMovementPlugin 0x%p (%s)\n", indent, "", this);
}

void ParallelMovementPlugin::Refresh(const float &fDeltaTime)
{
	
}

void ParallelMovementPlugin::SignalRemoved(PhysicalObject *pObject)
{
	
}

Block *ParallelMovementPlugin::ToBlock()
{
	return 0;
}