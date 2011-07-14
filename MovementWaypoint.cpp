#include "MovementWaypoint.h"

MovementWaypoint::MovementWaypoint()
{
	Init();
}

MovementWaypoint::~MovementWaypoint()
{
	
}

void MovementWaypoint::Init()
{
	fPreviousDistanceSquared = -1.0f;
}

void MovementWaypoint::Print(int indent) const
{
	printf("%*sMovementWaypoint 0x%p (%s)\n", indent, "", this);
}
