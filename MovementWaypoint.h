#ifndef _MOVE_WAYPOINT_
#define _MOVE_WAYPOINT_

#include "Unknown.h"

#include "def.h"

class MovementWaypoint : public Unknown
{
public:
	MovementWaypoint();

	virtual const char					*GetClassName() const			{ return "MovementWaypoint"; }
	virtual void						Print(int indent) const;

	float								fPreviousDistanceSquared;
	VECTOR3								vDest;

protected:
	virtual ~MovementWaypoint();

private:
	void				Init();

};

#endif