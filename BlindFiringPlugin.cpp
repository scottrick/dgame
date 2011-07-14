#include "BlindFiringPlugin.h"

#include "BlindFiringTask.h"
#include "PhysicalObject.h"

#include <iostream>
using namespace std;

BlindFiringPlugin::BlindFiringPlugin()
{
	Init();
}

BlindFiringPlugin::~BlindFiringPlugin()
{

}

void BlindFiringPlugin::AssignFiringTask(PhysicalObject *pObject)
{
	BlindFiringTask *pTask = new BlindFiringTask();
	pObject->SetFiringTask(pTask);
	pTask->Release();
}

void BlindFiringPlugin::FromBlock(Block *pBlock)
{

}

BlindFiringPlugin *BlindFiringPlugin::GetPlugin()
{
	static BlindFiringPlugin *pPlugin = new BlindFiringPlugin();
	return pPlugin;
}

void BlindFiringPlugin::Init()
{
	
}

void BlindFiringPlugin::Print(int indent) const
{
	printf("%*sBlindFiringPlugin 0x%p (%s)\n", indent, "", this);
}

void BlindFiringPlugin::Refresh(const float &fDeltaTime)
{

}

void BlindFiringPlugin::SignalRemoved(PhysicalObject *pObject)
{

}

Block *BlindFiringPlugin::ToBlock()
{
	return 0;
}