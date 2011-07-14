/*
#include "StandardMovementPlugin.h"

#include "Block.h"
#include "Game.h"
#include "MovementWaypoint.h"
#include "Node.h"
#include "PhysicalObject.h"
#include "PosQuat.h"
#include "StandardMovementTask.h"
#include "utility.h"

#include <iostream>
using namespace std;

StandardMovementPlugin::StandardMovementPlugin()
{
	Init();
}

StandardMovementPlugin::~StandardMovementPlugin()
{
	
}

void StandardMovementPlugin::AssignMovementTask(PhysicalObject *pObject)
{

}

void StandardMovementPlugin::FromBlock(Block *pBlock)
{
	
}

StandardMovementPlugin *StandardMovementPlugin::GetPlugin()
{
	static StandardMovementPlugin *pPlugin = new StandardMovementPlugin();
	return pPlugin;
}

void StandardMovementPlugin::Init()
{
	
}

void StandardMovementPlugin::Print(int indent) const
{
	printf("%*sStandardMovementPlugin 0x%p (%s)\n", indent, "", this);
}

void StandardMovementPlugin::Refresh(const float &fDeltaTime)
{
	
}

void StandardMovementPlugin::SignalRemoved(PhysicalObject *pObject)
{
	
}

Block *StandardMovementPlugin::ToBlock()
{
	return 0;
}	
*/