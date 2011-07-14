#ifndef _MATRIX44_
#define _MATRIX44_				1

#include "def.h"

class Quat;

class Matrix44
{
public:
	Matrix44();
	~Matrix44();

	void						Print() const;
	Quat						ToQuat() const;

	Matrix44					ToRotationalInverse() const;

	float						m[16];
								/*
									MATRIX STORED LIKE THIS (like OpenGL wants them):

									[  m0   m4   m8   m12  ]
									[  m1   m5   m9   m13  ]
									[  m2   m6   m10  m14  ]
									[  m3   m7   m11  m15  ]
								*/

private:
	void						Init();	
	
	
};

#endif