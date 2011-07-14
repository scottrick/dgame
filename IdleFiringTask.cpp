#include "IdleFiringTask.h"

#include "PhysicalObject.h"

#include <iostream>
using namespace std;

IdleFiringTask::IdleFiringTask()
{
	Init();
}

IdleFiringTask::~IdleFiringTask()
{

}

void IdleFiringTask::ApplyTask(PhysicalObject *pObject, float fDeltaTime)
{
	//DOESNT DO ANYTHING!!!!
}

void IdleFiringTask::Init()
{

}

void IdleFiringTask::Print(int indent) const
{
	printf("%*sIdleFiringTask 0x%p (%s)\n", indent, "", this);
}