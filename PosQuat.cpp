#include "PosQuat.h"

#include <iostream>
using namespace std;

PosQuat::PosQuat()
: pos(0.0f, 0.0f, 0.0f)
{

}

PosQuat::~PosQuat()
{

}

void PosQuat::Print(int indent) const
{
	printf("%*sPosQuat 0x%p\n", indent, " ", this);
	printf("%*s  pos  (%f, %f, %f)\n", indent, " ", pos.x, pos.y, pos.z);
	printf("%*s  quat (%f, %f, %f, %f)\n", indent, " ", quat.x, quat.y, quat.z, quat.w);
}

void PosQuat::operator = (PosQuat pq)
{
	pos		= pq.pos;
	quat	= pq.quat;
}