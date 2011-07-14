#include "AimedFiringPlugin.h"

#include "AimedFiringTask.h"
#include "PhysicalObject.h"

#include <iostream>
using namespace std;

AimedFiringPlugin::AimedFiringPlugin()
{
	Init();
}

AimedFiringPlugin::~AimedFiringPlugin()
{

}

void AimedFiringPlugin::AssignFiringTask(PhysicalObject *pObject)
{
	AimedFiringTask *pTask = new AimedFiringTask();
	pTask->SetTarget(pObject->GetTarget());
	pObject->SetFiringTask(pTask);
	pTask->Release();
}

void AimedFiringPlugin::FromBlock(Block *pBlock)
{

}

AimedFiringPlugin *AimedFiringPlugin::GetPlugin()
{
	static AimedFiringPlugin *pPlugin = new AimedFiringPlugin();
	return pPlugin;
}

void AimedFiringPlugin::Init()
{
	
}

void AimedFiringPlugin::Print(int indent) const
{
	printf("%*sAimedFiringPlugin 0x%p (%s)\n", indent, "", this);
}

void AimedFiringPlugin::Refresh(const float &fDeltaTime)
{

}

void AimedFiringPlugin::SignalRemoved(PhysicalObject *pObject)
{

}

Block *AimedFiringPlugin::ToBlock()
{
	return 0;
}

