#include "IdleMovementPlugin.h"

#include "Block.h"
#include "Game.h"
#include "MovementWaypoint.h"
#include "PhysicalObject.h"
#include "IdleMovementTask.h"

#include <iostream>
using namespace std;

IdleMovementPlugin::IdleMovementPlugin()
{
	Init();
}

IdleMovementPlugin::~IdleMovementPlugin()
{
	
}

void IdleMovementPlugin::AssignMovementTask(PhysicalObject *pObject)
{
	IdleMovementTask *pTask			= new IdleMovementTask();

	pObject->SetMovementTask(pTask);
	pTask->Release();
}

void IdleMovementPlugin::FromBlock(Block *pBlock)
{
	
}

IdleMovementPlugin *IdleMovementPlugin::GetPlugin()
{
	static IdleMovementPlugin *pPlugin = new IdleMovementPlugin();
	return pPlugin;
}

void IdleMovementPlugin::Init()
{
	
}

void IdleMovementPlugin::Print(int indent) const
{
	printf("%*sIdleMovementPlugin 0x%p (%s)\n", indent, "", this);
}

void IdleMovementPlugin::Refresh(const float &fDeltaTime)
{
	
}

void IdleMovementPlugin::SignalRemoved(PhysicalObject *pObject)
{
	
}

Block *IdleMovementPlugin::ToBlock()
{
	return 0;
}	