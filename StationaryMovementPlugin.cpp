#include "StationaryMovementPlugin.h"

#include "Block.h"
#include "Game.h"
#include "MovementWaypoint.h"
#include "PhysicalObject.h"
#include "IdleMovementTask.h"

#include <iostream>
using namespace std;

StationaryMovementPlugin::StationaryMovementPlugin()
{
	Init();
}

StationaryMovementPlugin::~StationaryMovementPlugin()
{
	
}

void StationaryMovementPlugin::AssignMovementTask(PhysicalObject *pObject)
{
	IdleMovementTask *pTask			= new IdleMovementTask();

	pObject->SetVelocity(VECTOR3(0.0f, -0.85f, 0.0f));
	pObject->SetVelocityDecay(false);

	pObject->SetMovementTask(pTask);
	pTask->Release();
}

void StationaryMovementPlugin::FromBlock(Block *pBlock)
{
	
}

StationaryMovementPlugin *StationaryMovementPlugin::GetPlugin()
{
	static StationaryMovementPlugin *pPlugin = new StationaryMovementPlugin();
	return pPlugin;
}

void StationaryMovementPlugin::Init()
{
	
}

void StationaryMovementPlugin::Print(int indent) const
{
	printf("%*sStationaryMovementPlugin 0x%p (%s)\n", indent, "", this);
}

void StationaryMovementPlugin::Refresh(const float &fDeltaTime)
{
	
}

void StationaryMovementPlugin::SignalRemoved(PhysicalObject *pObject)
{
	
}

Block *StationaryMovementPlugin::ToBlock()
{
	return 0;
}	