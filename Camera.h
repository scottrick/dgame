#ifndef _CAMERA_
#define _CAMERA_

#include "Object.h"
#include "PosQuat.h"

class Block;

class Camera : public Object
{
public:
	
	enum CameraType {
		eCAMERA_NO_TYPE,
		eCAMERA_FREE,
		eCAMERA_GAME,

		eNUM_CAMERA_TYPES,
	};

	Camera();
	
	void						FromBlock(Block *pBlock);
	unsigned int				GetCameraType() const		{ return m_dwCameraType; }
	virtual const char			*GetClassName() const		{ return "Camera"; }
	PosQuat						GetPosQuat() const			{ return m_PosQuat; }
	void						Refresh(const float &fDeltaTime);
	void						SetPosQuat(PosQuat newPQ)	{ m_PosQuat = newPQ; }
	Block						*ToBlock();

protected:
	virtual						~Camera();

	void						SetCameraType(CameraType dwType);

	PosQuat						m_PosQuat;

private:
	void						Init();

	unsigned int				m_dwCameraType;


};

#endif