#ifndef _FREE_CAMERA_
#define _FREE_CAMERA_

#include "Camera.h"

class Block;

class FreeCamera : public Camera
{
public:
	FreeCamera();
	
	void								FromBlock(Block *pBlock);
	virtual const char					*GetClassName() const			{ return "FreeCamera"; }
	void								LookUp();
	void								LookDown();
	void								MoveBackwards();
	void								MoveForwards();
	virtual void						Refresh(const float &fDeltaTime);
	void								StrafeLeft();  //strafe left
	void								StrafeRight(); //strafe right
	void								TiltLeft();
	void								TiltRight();
	Block								*ToBlock();
	void								TurnLeft();
	void								TurnRight();
	
protected:
	virtual ~FreeCamera();


private:
	void								Init();

	float								m_fCameraMoveSpeed;
	float								m_fCameraTurnSpeed;
};

#endif