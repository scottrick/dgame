#ifndef _BACKGROUND_
#define _BACKGROUND_

#include "Object.h"

class Camera;
class GameCamera;

#define										BACKGROUND_DISTANCE_FROM_GAME				30.0f

class Background : public Object
{
public:
	Background();

	virtual void							FromBlock(Block *pBlock);
	virtual const char						*GetClassName() const						{ return "Background"; }

	void									Refresh(const float &fDeltaTime);
	void									SetCamera(Camera *pNewCamera);
	virtual Block							*ToBlock();

protected:
	virtual	~Background();

	GameCamera								*m_pGameCamera;		//pointer to current game camera

private:
	void									Init();
	void									Recalculate();

	bool									m_bRecalculate;
	float									m_fScrollSpeed;		//units per second

};

#endif