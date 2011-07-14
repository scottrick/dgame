#include "IdleFiringPlugin.h"

#include "IdleFiringTask.h"
#include "PhysicalObject.h"

#include <iostream>
using namespace std;

IdleFiringPlugin::IdleFiringPlugin()
{
	Init();
}

IdleFiringPlugin::~IdleFiringPlugin()
{

}

void IdleFiringPlugin::AssignFiringTask(PhysicalObject *pObject)
{
	IdleFiringTask *pTask = new IdleFiringTask();
	pObject->SetFiringTask(pTask);
	pTask->Release();
}

void IdleFiringPlugin::FromBlock(Block *pBlock)
{

}

IdleFiringPlugin *IdleFiringPlugin::GetPlugin()
{
	static IdleFiringPlugin *pPlugin = new IdleFiringPlugin();
	return pPlugin;
}

void IdleFiringPlugin::Init()
{
	
}

void IdleFiringPlugin::Print(int indent) const
{
	printf("%*sIdleFiringPlugin 0x%p (%s)\n", indent, "", this);
}

void IdleFiringPlugin::Refresh(const float &fDeltaTime)
{

}

void IdleFiringPlugin::SignalRemoved(PhysicalObject *pObject)
{

}

Block *IdleFiringPlugin::ToBlock()
{
	return 0;
}