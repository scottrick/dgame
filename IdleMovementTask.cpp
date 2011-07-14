#include "IdleMovementTask.h"

#include "Node.h"
#include "PhysicalObject.h"

#include <iostream>
using namespace std;

IdleMovementTask::IdleMovementTask()
{
	Init();
}

IdleMovementTask::~IdleMovementTask()
{

}

void IdleMovementTask::ApplyTask(PhysicalObject *pObject, float fDeltaTime)
{
	//DOESNT DO ANYTHING
}

void IdleMovementTask::Init()
{

}

void IdleMovementTask::Print(int indent) const
{
	printf("%*sIdleMovementTask 0x%p (%s)\n", indent, "", this);
}