#include "FreeCamera.h"

#include "Block.h"
#include "Matrix44.h"

FreeCamera::FreeCamera()
{
	Init();
}

FreeCamera::~FreeCamera()
{

}	

void FreeCamera::FromBlock(Block *pBlock)
{

}

void FreeCamera::Init()
{
	SetCameraType(Camera::eCAMERA_FREE);
	m_fCameraMoveSpeed		= 0.05f;
	m_fCameraTurnSpeed		= 0.03f;
	m_PosQuat.pos.z			= 3.0f;
	m_PosQuat.pos.y			= 0.5f;
}

void FreeCamera::LookDown()
{
	Matrix44	rotation = m_PosQuat.quat.ToMatrix();
	Quat		*quat = &(m_PosQuat.quat);
	Quat		rotationToMake;
	VECTOR3		axis, actualAxis;

	axis.x = 1.0f;
	axis.y = 0.0f;
	axis.z = 0.0f;

	actualAxis.x = (rotation.m[0] * axis.x) + (rotation.m[4] * axis.y) + (rotation.m[8] * axis.z);
	actualAxis.y = (rotation.m[1] * axis.x) + (rotation.m[5] * axis.y) + (rotation.m[9] * axis.z);
	actualAxis.z = (rotation.m[2] * axis.x) + (rotation.m[6] * axis.y) + (rotation.m[10] * axis.z);

	rotationToMake.CreateFromRotationRADIANS(-m_fCameraTurnSpeed, actualAxis.x, actualAxis.y, actualAxis.z);
	*quat = rotationToMake * *quat;
}

void FreeCamera::LookUp()
{
	Matrix44	rotation = m_PosQuat.quat.ToMatrix();
	Quat		*quat = &(m_PosQuat.quat);
	Quat		rotationToMake;
	VECTOR3		axis, actualAxis;

	axis.x = 1.0f;
	axis.y = 0.0f;
	axis.z = 0.0f;

	actualAxis.x = (rotation.m[0] * axis.x) + (rotation.m[4] * axis.y) + (rotation.m[8] * axis.z);
	actualAxis.y = (rotation.m[1] * axis.x) + (rotation.m[5] * axis.y) + (rotation.m[9] * axis.z);
	actualAxis.z = (rotation.m[2] * axis.x) + (rotation.m[6] * axis.y) + (rotation.m[10] * axis.z);

	rotationToMake.CreateFromRotationRADIANS(m_fCameraTurnSpeed, actualAxis.x, actualAxis.y, actualAxis.z);
	*quat = rotationToMake * *quat;
}

void FreeCamera::MoveBackwards()
{
	Matrix44 rotation = m_PosQuat.quat.ToMatrix();
	VECTOR3 vTravelDistance;
	vTravelDistance.x = 0.0f;
	vTravelDistance.y = 0.0f;
	vTravelDistance.z = m_fCameraMoveSpeed;  //z coordinate into the screen is negative

	m_PosQuat.pos.x += (vTravelDistance.x * rotation.m[0]) + (vTravelDistance.y * rotation.m[4]) + (vTravelDistance.z * rotation.m[8]);
	m_PosQuat.pos.y += (vTravelDistance.x * rotation.m[1]) + (vTravelDistance.y * rotation.m[5]) + (vTravelDistance.z * rotation.m[9]);
	m_PosQuat.pos.z += (vTravelDistance.x * rotation.m[2]) + (vTravelDistance.y * rotation.m[6]) + (vTravelDistance.z * rotation.m[10]);
}

void FreeCamera::MoveForwards()
{
	Matrix44 rotation = m_PosQuat.quat.ToMatrix();
	VECTOR3 vTravelDistance;
	vTravelDistance.x = 0.0f;
	vTravelDistance.y = 0.0f;
	vTravelDistance.z = -m_fCameraMoveSpeed;  //z coordinate into the screen is negative

	m_PosQuat.pos.x += (vTravelDistance.x * rotation.m[0]) + (vTravelDistance.y * rotation.m[4]) + (vTravelDistance.z * rotation.m[8]);
	m_PosQuat.pos.y += (vTravelDistance.x * rotation.m[1]) + (vTravelDistance.y * rotation.m[5]) + (vTravelDistance.z * rotation.m[9]);
	m_PosQuat.pos.z += (vTravelDistance.x * rotation.m[2]) + (vTravelDistance.y * rotation.m[6]) + (vTravelDistance.z * rotation.m[10]);
}	

void FreeCamera::Refresh(const float &fDeltaTime)
{

}

void FreeCamera::StrafeLeft()
{
	Matrix44 rotation = m_PosQuat.quat.ToMatrix();
	VECTOR3 vTravelDistance;
	vTravelDistance.x = -m_fCameraMoveSpeed;
	vTravelDistance.y = 0.0f;
	vTravelDistance.z = 0.0f;

	m_PosQuat.pos.x += (vTravelDistance.x * rotation.m[0]) + (vTravelDistance.y * rotation.m[4]) + (vTravelDistance.z * rotation.m[8]);
	m_PosQuat.pos.y += (vTravelDistance.x * rotation.m[1]) + (vTravelDistance.y * rotation.m[5]) + (vTravelDistance.z * rotation.m[9]);
	m_PosQuat.pos.z += (vTravelDistance.x * rotation.m[2]) + (vTravelDistance.y * rotation.m[6]) + (vTravelDistance.z * rotation.m[10]);
}

void FreeCamera::StrafeRight()
{
	Matrix44 rotation = m_PosQuat.quat.ToMatrix();
	VECTOR3 vTravelDistance;
	vTravelDistance.x = m_fCameraMoveSpeed;
	vTravelDistance.y = 0.0f;
	vTravelDistance.z = 0.0f;

	m_PosQuat.pos.x += (vTravelDistance.x * rotation.m[0]) + (vTravelDistance.y * rotation.m[4]) + (vTravelDistance.z * rotation.m[8]);
	m_PosQuat.pos.y += (vTravelDistance.x * rotation.m[1]) + (vTravelDistance.y * rotation.m[5]) + (vTravelDistance.z * rotation.m[9]);
	m_PosQuat.pos.z += (vTravelDistance.x * rotation.m[2]) + (vTravelDistance.y * rotation.m[6]) + (vTravelDistance.z * rotation.m[10]);
}

void FreeCamera::TiltLeft()
{
	Matrix44	rotation = m_PosQuat.quat.ToMatrix();
	Quat		*quat = &(m_PosQuat.quat);
	Quat		rotationToMake;
	VECTOR3		axis, actualAxis;

	axis.x = 0.0f;
	axis.y = 0.0f;
	axis.z = 1.0f;

	actualAxis.x = (rotation.m[0] * axis.x) + (rotation.m[4] * axis.y) + (rotation.m[8] * axis.z);
	actualAxis.y = (rotation.m[1] * axis.x) + (rotation.m[5] * axis.y) + (rotation.m[9] * axis.z);
	actualAxis.z = (rotation.m[2] * axis.x) + (rotation.m[6] * axis.y) + (rotation.m[10] * axis.z);

	rotationToMake.CreateFromRotationRADIANS(m_fCameraTurnSpeed, actualAxis.x, actualAxis.y, actualAxis.z);
	*quat = rotationToMake * *quat;
}

void FreeCamera::TiltRight()
{
	Matrix44	rotation = m_PosQuat.quat.ToMatrix();
	Quat		*quat = &(m_PosQuat.quat);
	Quat		rotationToMake;
	VECTOR3		axis, actualAxis;

	axis.x = 0.0f;
	axis.y = 0.0f;
	axis.z = 1.0f;

	actualAxis.x = (rotation.m[0] * axis.x) + (rotation.m[4] * axis.y) + (rotation.m[8] * axis.z);
	actualAxis.y = (rotation.m[1] * axis.x) + (rotation.m[5] * axis.y) + (rotation.m[9] * axis.z);
	actualAxis.z = (rotation.m[2] * axis.x) + (rotation.m[6] * axis.y) + (rotation.m[10] * axis.z);

	rotationToMake.CreateFromRotationRADIANS(-m_fCameraTurnSpeed, actualAxis.x, actualAxis.y, actualAxis.z);
	*quat = rotationToMake * *quat;
}

Block *FreeCamera::ToBlock()
{
	return 0;
}

void FreeCamera::TurnLeft()
{
	Matrix44	rotation = m_PosQuat.quat.ToMatrix();
	Quat		*quat = &(m_PosQuat.quat);
	Quat		rotationToMake;
	VECTOR3		axis, actualAxis;

	axis.x = 0.0f;
	axis.y = 1.0f;
	axis.z = 0.0f;

	actualAxis.x = (rotation.m[0] * axis.x) + (rotation.m[4] * axis.y) + (rotation.m[8] * axis.z);
	actualAxis.y = (rotation.m[1] * axis.x) + (rotation.m[5] * axis.y) + (rotation.m[9] * axis.z);
	actualAxis.z = (rotation.m[2] * axis.x) + (rotation.m[6] * axis.y) + (rotation.m[10] * axis.z);

	rotationToMake.CreateFromRotationRADIANS(m_fCameraTurnSpeed, actualAxis.x, actualAxis.y, actualAxis.z);
	*quat = rotationToMake * *quat;
}

void FreeCamera::TurnRight()
{
	Matrix44	rotation = m_PosQuat.quat.ToMatrix();
	Quat		*quat = &(m_PosQuat.quat);
	Quat		rotationToMake;
	VECTOR3		axis, actualAxis;

	axis.x = 0.0f;
	axis.y = 1.0f;
	axis.z = 0.0f;

	actualAxis.x = (rotation.m[0] * axis.x) + (rotation.m[4] * axis.y) + (rotation.m[8] * axis.z);
	actualAxis.y = (rotation.m[1] * axis.x) + (rotation.m[5] * axis.y) + (rotation.m[9] * axis.z);
	actualAxis.z = (rotation.m[2] * axis.x) + (rotation.m[6] * axis.y) + (rotation.m[10] * axis.z);

	rotationToMake.CreateFromRotationRADIANS(-m_fCameraTurnSpeed, actualAxis.x, actualAxis.y, actualAxis.z);
	*quat = rotationToMake * *quat;
}