#include "Matrix44.h"

#include "Quat.h"

#include <cmath>
#include <iostream>
using namespace std;

Matrix44::Matrix44()
{
	Init();
}

Matrix44::~Matrix44()
{
	
}

void Matrix44::Init()
{
	m[0]   = 0.0f;
	m[1]   = 0.0f;
	m[2]   = 0.0f;
	m[3]   = 0.0f;
	m[4]   = 0.0f;
	m[5]   = 0.0f;
	m[6]   = 0.0f;
	m[7]   = 0.0f;
	m[8]   = 0.0f;
	m[9]   = 0.0f;
	m[10]  = 0.0f;
	m[11]  = 0.0f;
	m[12]  = 0.0f;
	m[13]  = 0.0f;
	m[14]  = 0.0f;
	m[15]  = 0.0f;
}

void Matrix44::Print() const
{
	printf("Matrix 0x%p\n", this);
	printf("  [%f, %f, %f, %f]\n", m[0], m[4], m[8], m[12]);
	printf("  [%f, %f, %f, %f]\n", m[1], m[5], m[9], m[13]);
	printf("  [%f, %f, %f, %f]\n", m[2], m[6], m[10], m[14]);
	printf("  [%f, %f, %f, %f]\n", m[3], m[7], m[11], m[15]);
}

Quat Matrix44::ToQuat() const
{
	Quat				newQuat;
	float				tr, s, q[4];
	int					i, j, k;
	int					nxt[3] = {1, 2, 0};

	tr					= m[0] + m[5] + m[10];

	// check the diagonal
	if (tr > 0.0) 
	{
		s				= sqrt(tr + 1.0f);
		newQuat.w		= s / 2.0f;
		s				= 0.5f / s;
		newQuat.x		= (m[9] - m[6]) * s;
		newQuat.y		= (m[2] - m[8]) * s;
		newQuat.z		= (m[4] - m[1]) * s;
	}
	else
	{
		// diagonal is negative
		i = 0;

		if (m[5] > m[0])
		{
			i = 1;
		}

		if (m[10] > m[i+i*4]) 
		{
			i = 2;
		}

		j		= nxt[i];
		k		= nxt[j];
		s		= sqrt ((m[i+i*4] - (m[j+j*4] + m[k+k*4])) + 1.0f);
      	q[i]	= s * 0.5f;
            
		if (s != 0.0f) 
		{
			s = 0.5f / s;
		}

		q[3] = (m[j+k*4] - m[k+j*4]) * s;
		q[j] = (m[i+j*4] + m[j+i*4]) * s;
		q[k] = (m[i+k*4] + m[k+i*4]) * s;

		newQuat.x = q[0];
		newQuat.y = q[1];
		newQuat.z = q[2];
		newQuat.w = q[3];
	}

	return newQuat;
}

Matrix44 Matrix44::ToRotationalInverse() const
{
	Matrix44 RotInv;

	RotInv.m[0]		= m[0];
	RotInv.m[1]		= m[4];
	RotInv.m[2]		= m[8];
	RotInv.m[3]		= m[12];
	RotInv.m[4]		= m[1];
	RotInv.m[5]		= m[5];
	RotInv.m[6]		= m[9];
	RotInv.m[7]		= m[13];
	RotInv.m[8]		= m[2];
	RotInv.m[9]		= m[6];
	RotInv.m[10]	= m[10];
	RotInv.m[11]	= m[14];
	RotInv.m[12]	= m[3];
	RotInv.m[13]	= m[7];
	RotInv.m[14]	= m[11];
	RotInv.m[15]	= m[15];

	return RotInv;
}