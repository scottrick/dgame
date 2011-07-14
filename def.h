#ifndef _DEF_
#define _DEF_				1

#include <cassert>
#include <cmath>
using namespace std;

#define						INSANITY_FACTOR								4
#define						MATH_NORMALIZATION_ALLOWABLE_ERROR			0.000000001f
#define						PI											3.141592654f
#define						GAME_FOV									60.0f

static float				InvSqrt(float x);

struct VECTOR2
{
	VECTOR2()											{ x = 0.0f;		y = 0.0f; }
	VECTOR2(float fX, float fY)							{ x = fX;		y = fY; }
	
	float x, y;
};

struct VECTOR3
{
	VECTOR3()											{ x = 0.0f;		y = 0.0f;		z = 0.0f; }
	VECTOR3(float fX, float fY, float fZ)				{ x = fX;		y = fY;			z = fZ; }

	float x, y, z;

	VECTOR3 operator- (VECTOR3 vSub)					{ VECTOR3 temp; temp.x = x - vSub.x; temp.y = y - vSub.y; temp.z = vSub.z; return temp; }
};

struct VECTOR4
{
	VECTOR4()											{ x = 0.0f;		y = 0.0f;		z = 0.0f;		w = 0.0f; }
	VECTOR4(float fX, float fY, float fZ, float fW)		{ x = fX;		y = fY;			z = fZ;			w = fW; }

	float x, y, z, w;
};

struct VECTOR2_INT
{
	VECTOR2_INT()										{ x = 0;		y = 0; }
	VECTOR2_INT(int dwX, int dwY)						{ x = dwX;		y = dwY; }

	int x, y;
};

static VECTOR3 CrossProduct(const VECTOR3 &vec1, const VECTOR3 &vec2)
{
	VECTOR3 newVec;
	newVec.x = (vec1.y * vec2.z) - (vec1.z * vec2.y);
	newVec.y = -((vec1.x * vec2.z) - (vec1.z * vec2.x));
	newVec.z = (vec1.x * vec2.y) - (vec1.y * vec2.x);

	return newVec;
}

static float MagnitudeSquaredVECTOR3(VECTOR3 &vec)
{
	return (vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}	

static void NormalizeVECTOR2(VECTOR2 &vec)
{
	float magnitude = (vec.x * vec.x) + (vec.y * vec.y);

	if (abs(magnitude - 1.0f) < MATH_NORMALIZATION_ALLOWABLE_ERROR)
	{
		//don't need to normalize, close enough
	}
	else
	{
		//assert(magnitude > MATH_NORMALIZATION_ALLOWABLE_ERROR); //make sure any vector being normalized has some non-zero magnitude

		/*
		//slow way
		magnitude = sq rt(magnitude);

		vec.x = vec.x / magnitude;		
		vec.y = vec.y / magnitude;
		*/

		magnitude = InvSqrt(magnitude);

		vec.x = vec.x * magnitude;
		vec.y = vec.y * magnitude;
	}
}	

static void NormalizeVECTOR3(VECTOR3 &vec)
{
	float magnitude = (vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z);

	if (abs(magnitude - 1.0f) < MATH_NORMALIZATION_ALLOWABLE_ERROR)
	{
		//don't need to normalize, close enough
	}
	else
	{
		//assert(magnitude > MATH_NORMALIZATION_ALLOWABLE_ERROR); //make sure any vector being normalized has some non-zero magnitude

		/*
		//slow way
		magnitude = sq rt(magnitude);

		vec.x = vec.x / magnitude;		
		vec.y = vec.y / magnitude;
		vec.z = vec.z / magnitude;
		*/
		
		magnitude = InvSqrt(magnitude);

		vec.x *= magnitude;		
		vec.y *= magnitude;
		vec.z *= magnitude;
	}
}	

static void NormalizeVECTOR4(VECTOR4 &vec)
{
	float magnitude = (vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z) + (vec.w * vec.w);

	if (abs(magnitude - 1.0f) < MATH_NORMALIZATION_ALLOWABLE_ERROR)
	{
		//don't need to normalize, close enough
	}
	else
	{
		//assert(magnitude > MATH_NORMALIZATION_ALLOWABLE_ERROR); //make sure any vector being normalized has some non-zero magnitude

		/*
		//slow way
		magnitude = sq rt(magnitude);

		vec.x = vec.x / magnitude;		
		vec.y = vec.y / magnitude;
		vec.z = vec.z / magnitude;
		vec.w = vec.w / magnitude;
		*/

		magnitude = InvSqrt(magnitude);

		vec.x *= magnitude;
		vec.y *= magnitude;
		vec.z *= magnitude;
		vec.w *= magnitude;
	}
}	

//Fast approximation of  "1 / sq rt(x)"
//http://www.beyond3d.com/content/articles/8/
static float InvSqrt (float x)
{
    float	xhalf		= 0.5f*x;
    int		i			= *(int*)&x;

    i		= 0x5f3759df - (i>>1);
    x		= *(float*)&i;
    x		= x*(1.5f - xhalf*x*x);

    return x;
}

#endif 