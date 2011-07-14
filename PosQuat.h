#ifndef _POSQUAT_
#define _POSQUAT_					1

#include "def.h"
#include "Quat.h"

class PosQuat
{
public:
	PosQuat();
	~PosQuat();

	void				Print(int indent = 0) const;

	//DATA
	VECTOR3				pos;
	Quat				quat;

	void operator = (PosQuat pq);

private:

};

#endif
