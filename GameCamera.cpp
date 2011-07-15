#include "GameCamera.h"

#include "Game.h"
#include <cmath>
using namespace std;

GameCamera::GameCamera()
{
	Init();
}

GameCamera::~GameCamera()
{

}

void GameCamera::FromBlock(Block *)
{

}

void GameCamera::Init()
{
	SetCameraType(Camera::eCAMERA_GAME);

	vGameSize.x				= GAME_SIZE_X;
	vGameSize.y				= GAME_SIZE_Y;

	//assuming square game area for now
	float fTemp				= tan((1.0f / 6.0f) * PI) * 2.0f; //for now just always asume 60 degree viewing angle
	m_PosQuat.pos.z			= vGameSize.x / fTemp;
}

void GameCamera::Refresh(const float &)
{

}

Block *GameCamera::ToBlock()
{
	return 0;
}