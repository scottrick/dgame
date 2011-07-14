#include "Quat.h"

#include "Matrix44.h"

#include <cmath>
#include <iostream>
using namespace std;

Quat::Quat()
: x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{

}

Quat::~Quat()
{
	
}

Quat Quat::Conjugate() const
{
	Quat conjugate;
	conjugate.x = -x;
	conjugate.y = -y;
	conjugate.z = -z;

	return conjugate;
}

void Quat::CreateFromRotationDEGREES(const float &angle, const float &xAxis, const float &yAxis, const float &zAxis)
{	
	float fRadians = angle / 180.0f * PI;
	CreateFromRotationRADIANS(fRadians, xAxis, yAxis, zAxis);
}	

void Quat::CreateFromRotationRADIANS(const float &angle, const float &xAxis, const float &yAxis, const float &zAxis)
{
	VECTOR3 vec3;
	vec3.x = xAxis;
	vec3.y = yAxis;
	vec3.z = zAxis;
	NormalizeVECTOR3(vec3);

	w = cos(angle / 2.0f);
	x = vec3.x * sin(angle / 2.0f);
	y = vec3.y * sin(angle / 2.0f);
	z = vec3.z * sin(angle / 2.0f);

	//Dont think I need to normalize here, since the input is always normalized, it will always be a normalized result
	//Normalize();
}

void Quat::Normalize()
{	
	float magnitude = (w * w) + (x * x) + (y * y) + (z * z);

	if (abs(magnitude - 1.0f) <= MATH_NORMALIZATION_ALLOWABLE_ERROR)
	{
		//don't need to normalize;  close enough
	}
	else
	{
		/*
		//slow way
		magnitude = sq rt(magnitude);

		w = w / magnitude;
		x = x / magnitude;
		y = y / magnitude;
		z = z / magnitude;
		*/

		magnitude	= InvSqrt(magnitude);
		w			= w * magnitude;
		x			= x * magnitude;
		y			= y * magnitude;
		z			= z * magnitude;
	}
}	

Quat Quat::operator + (Quat q)
{
	Quat result;

	result.x = q.x + x;
	result.y = q.y + y;
	result.z = q.z + z;
	result.w = q.w + w;

	return result;
}

Quat Quat::operator * (Quat q)
{
	Quat result;

	result.w = (w * q.w) - (x * q.x) - (y * q.y) - (z * q.z);
	result.x = (w * q.x) + (x * q.w) + (y * q.z) - (z * q.y);
	result.y = (w * q.y) + (y * q.w) + (z * q.x) - (x * q.z);
	result.z = (w * q.z) + (z * q.w) + (x * q.y) - (y * q.x);
	
	/*
	w = w1w2 - x1x2 - y1y2 - z1z2
	x = w1x2 + x1w2 + y1z2 - z1y2
	y = w1y2 + y1w2 + z1x2 - x1z2
	z = w1z2 + z1w2 + x1y2 - y1x2
	*/

	return result;
}

void Quat::operator = (Quat q)
{
	w = q.w;
	x = q.x;
	y = q.y;
	z = q.z;
}

void Quat::Print() const
{
	printf("Quaternion 0x%p\n", this);
	printf("  (%f, %f, %f, %f)\n", x, y, z, w);
}

Matrix44 Quat::ToMatrix() const
{	
	Matrix44 newMatrix;

	/*
	newMatrix.m[0]		= 1.0f - 2.0f * (y * y + z * z);
	newMatrix.m[1]		= 2.0f * (x * y + w * z);
	newMatrix.m[2]		= 2.0f * (x * z - w * y);
	newMatrix.m[3]		= 0;
	newMatrix.m[4]		= 2.0f * (x * y - w * z);
	newMatrix.m[5]		= 1.0f - 2.0f * (x * x + z * z);
	newMatrix.m[6]		= 2.0f * (y * z + w * x);
	newMatrix.m[7]		= 0;
	newMatrix.m[8]		= 2.0f * (w * y + x * z);
	newMatrix.m[9]		= 2.0f * (y * z - w * x);
	newMatrix.m[10]		= 1.0f - 2.0f * (x * x + y * y);
	newMatrix.m[11]		= 0.0f;
	newMatrix.m[12]		= 0.0f;
	newMatrix.m[13]		= 0.0f;
	newMatrix.m[14]		= 0.0f;
	newMatrix.m[15]		= 1.0f;
	*/

	newMatrix.m[0]		= 1.0f - 2.0f * (y * y + z * z);
	newMatrix.m[1]		= 2.0f * (x * y + w * z);
	newMatrix.m[2]		= 2.0f * (x * z - w * y);
	newMatrix.m[3]		= 0;
	newMatrix.m[4]		= 2.0f * (x * y - w * z);
	newMatrix.m[5]		= 1.0f - 2.0f * (x * x + z * z);
	newMatrix.m[6]		= 2.0f * (y * z + w * x);
	newMatrix.m[7]		= 0;
	newMatrix.m[8]		= 2.0f * (w * y + x * z);
	newMatrix.m[9]		= 2.0f * (y * z - w * x);
	newMatrix.m[10]		= 1.0f - 2.0f * (x * x + y * y);
	newMatrix.m[11]		= 0.0f;
	newMatrix.m[12]		= 0.0f;
	newMatrix.m[13]		= 0.0f;
	newMatrix.m[14]		= 0.0f;
	newMatrix.m[15]		= 1.0f;


	return newMatrix;
}