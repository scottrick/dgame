#ifndef _GAME_CAMERA_
#define _GAME_CAMERA_

#include "Camera.h"

/*	
	The GameCamera is the standard camera used when playing the game.
*/	
class GameCamera : public Camera
{
public:
	GameCamera();
	
	virtual void							FromBlock(Block *pBlock);
	virtual const char						*GetClassName() const		{ return "GameCamera"; }
	VECTOR2									GetGameSize() const			{ return vGameSize; }
	virtual void							Refresh(const float &fDeltaTime);
	virtual Block							*ToBlock();

protected:	
	virtual ~GameCamera();

	VECTOR2									vGameSize;

private:
	void									Init();

};

#endif