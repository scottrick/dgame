#include "JumpMovementPlugin.h"

#include "Block.h"
#include "Game.h"
#include "JumpMovementTask.h"
#include "MovementWaypoint.h"
#include "PhysicalObject.h"
#include "utility.h"

#include <iostream>
using namespace std;

JumpMovementPlugin::JumpMovementPlugin()
{
	Init();
}

JumpMovementPlugin::~JumpMovementPlugin()
{
	
}

void JumpMovementPlugin::AssignMovementTask(PhysicalObject *pObject)
{
	JumpMovementTask *pTask			= new JumpMovementTask();
	pTask->SetRepeating(false);

	pTask->SetDelay(0.5f + GenerateRandomFloat(2.5f));

	float fXDest					= GenerateRandomFloat(GAME_SIZE_X - 2.0f) - (GAME_SIZE_X / 2.0f);
	float fYDest					= GenerateRandomFloat(GAME_SIZE_Y - 2.0f) - (GAME_SIZE_Y / 2.0f);
	
	MovementWaypoint *pWaypoint		= new MovementWaypoint();
	pWaypoint->vDest				= VECTOR3(fXDest, fYDest, 0.0f);

	pTask->AddMovementWaypoint(pWaypoint);
	pWaypoint->Release();

	pObject->SetMovementTask(pTask);
	pTask->Release();
}

void JumpMovementPlugin::FromBlock(Block *pBlock)
{
	
}

JumpMovementPlugin *JumpMovementPlugin::GetPlugin()
{
	static JumpMovementPlugin *pPlugin = new JumpMovementPlugin();
	return pPlugin;
}

void JumpMovementPlugin::Init()
{

}

void JumpMovementPlugin::Print(int indent) const
{
	printf("%*sJumpMovementPlugin 0x%p (%s)\n", indent, "", this);
}

void JumpMovementPlugin::Refresh(const float &fDeltaTime)
{
	
}

void JumpMovementPlugin::SignalRemoved(PhysicalObject *pObject)
{
	
}

Block *JumpMovementPlugin::ToBlock()
{
	return 0;
}	