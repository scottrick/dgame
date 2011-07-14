#ifndef _QUAT_
#define _QUAT_					1

#include "def.h"

class Matrix44;

class Quat
{
public:
	Quat();
	~Quat();

	Quat						Conjugate() const;
	void						CreateFromRotationDEGREES(const float &angle, const float &xAxis, const float &yAxis, const float &zAxis);
	void						CreateFromRotationRADIANS(const float &angle, const float &xAxis, const float &yAxis, const float &zAxis);
	void						Normalize();
	void						Print() const;

	Matrix44					ToMatrix() const;

	float						x, y, z, w;

	Quat operator + (Quat q);
	Quat operator * (Quat q);
	void operator = (Quat q);

private:

};

#endif