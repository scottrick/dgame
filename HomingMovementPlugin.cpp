#include "HomingMovementPlugin.h"

#include "HomingMovementTask.h"
#include "PhysicalObject.h"

#include <iostream>
using namespace std;

HomingMovementPlugin::HomingMovementPlugin()
{
	Init();
}

HomingMovementPlugin::~HomingMovementPlugin()
{

}

void HomingMovementPlugin::AssignMovementTask(PhysicalObject *pObject)
{
	//printf("--------------> assigning homing task.....\n");
	HomingMovementTask *pTask = new HomingMovementTask();
	pTask->SetTarget(pObject->GetTarget());
	pObject->SetMovementTask(pTask);
	pTask->Release();
}

void HomingMovementPlugin::FromBlock(Block *pBlock)
{
	
}

HomingMovementPlugin *HomingMovementPlugin::GetPlugin()
{
	static HomingMovementPlugin *pPlugin = new HomingMovementPlugin();
	return pPlugin;
}

void HomingMovementPlugin::Init()
{
	
}

void HomingMovementPlugin::Print(int indent) const
{
	printf("%*sHomingMovementPlugin 0x%p (%s)\n", indent, "", this);
}

void HomingMovementPlugin::Refresh(const float &fDeltaTime)
{
	
}

void HomingMovementPlugin::SignalRemoved(PhysicalObject *pObject)
{
	
}

Block *HomingMovementPlugin::ToBlock()
{
	return 0;
}	