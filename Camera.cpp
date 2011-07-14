#include "Camera.h"

#include "Block.h"

Camera::Camera()
{
	Init();
}

Camera::~Camera()
{

}

void Camera::FromBlock(Block *pBlock)
{

}

void Camera::Init()
{
	SetObjectType(Object::eTYPE_CAMERA);
	m_dwCameraType		= Camera::eCAMERA_NO_TYPE; //no camera type
}

void Camera::Refresh(const float &fDeltaTime)
{

}

void Camera::SetCameraType(CameraType dwType)
{
	m_dwCameraType		= dwType;
}

Block *Camera::ToBlock() 
{
	return 0;
}