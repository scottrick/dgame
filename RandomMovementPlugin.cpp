#include "RandomMovementPlugin.h"

#include "Block.h"
#include "Game.h"
#include "MovementWaypoint.h"
#include "PhysicalObject.h"
#include "StandardMovementTask.h"
#include "utility.h"

#include <iostream>
using namespace std;

RandomMovementPlugin::RandomMovementPlugin()
{
	Init();
}

RandomMovementPlugin::~RandomMovementPlugin()
{
	
}

void RandomMovementPlugin::AssignMovementTask(PhysicalObject *pObject)
{
	StandardMovementTask *pTask		= new StandardMovementTask();
	pTask->SetRepeating(false);

	float fXDest					= GenerateRandomFloat(GAME_SIZE_X - 2.0f) - ((GAME_SIZE_X - 2.0f) / 2.0f);
	float fYDest					= GenerateRandomFloat(GAME_SIZE_Y - 2.0f) - ((GAME_SIZE_Y - 2.0f) / 2.0f);
	
	MovementWaypoint *pWaypoint		= new MovementWaypoint();
	pWaypoint->vDest				= VECTOR3(fXDest, fYDest, 0.0f);

	pTask->AddMovementWaypoint(pWaypoint);
	pWaypoint->Release();

	pObject->SetMovementTask(pTask);
	pTask->Release();
}

void RandomMovementPlugin::FromBlock(Block *pBlock)
{
	
}

RandomMovementPlugin *RandomMovementPlugin::GetPlugin()
{
	static RandomMovementPlugin *pPlugin = new RandomMovementPlugin();
	return pPlugin;
}

void RandomMovementPlugin::Init()
{

}

void RandomMovementPlugin::Print(int indent) const
{
	printf("%*sRandomMovementPlugin 0x%p (%s)\n", indent, "", this);
}

void RandomMovementPlugin::Refresh(const float &fDeltaTime)
{
	
}

void RandomMovementPlugin::SignalRemoved(PhysicalObject *pObject)
{
	
}

Block *RandomMovementPlugin::ToBlock()
{
	return 0;
}	