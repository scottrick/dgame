#ifndef _GAME_
#define _GAME_

class ActionScene;
class DrawBuffer;
class FittingScene;
class GalaxyScene;
class Manager;
class Node;
class NothingScene;
class Object;
class PhysicalObject;	
class Player;
class Scene;
class ShaderProgram;
class SplashScene;
class Visual;

#include "def.h"
#include <vector>
using namespace std;

//DEFINE KEYBOARD FLAGS
#define								KEYBOARD_CONTROL_ACTIVE			(1 << 1)
#define								KEYBOARD_SHIFT_ACTIVE			(1 << 2)
#define								KEYBOARD_ALT_ACTIVE				(1 << 3)

//DEFINE INPUT FLAGS
#define								INPUT_STRAFE_LEFT				(1 << 1)
#define								INPUT_STRAFE_RIGHT				(1 << 2)
#define								INPUT_MOVE_FORWARDS				(1 << 3)
#define								INPUT_MOVE_BACKWARDS			(1 << 4)
#define								INPUT_TURN_RIGHT				(1 << 5)	//clockwise around Y axis
#define								INPUT_TURN_LEFT					(1 << 6)	//counter clockwise around Y axis
#define								INPUT_LOOK_UP					(1 << 7)	//clockwise around X axis
#define								INPUT_LOOK_DOWN					(1 << 8)	//counter clockwise around X axis
#define								INPUT_TILT_LEFT					(1 << 9)	//clockwise around Z axis
#define								INPUT_TILT_RIGHT				(1 << 10)	//counter clockwise around Z axis

//DEFINE GAME CONSTANTS
#define								GAME_SIZE_X						18.0f
#define								GAME_SIZE_Y						18.0f
#define								NUM_SHIP_TYPES_PER_SECTOR		3

const int							GAME_VERSION					= 0;
const int							GAME_SUBVERSION					= 1;

/*
	The Game
*/
class Game {
public:
	Game();
	~Game();

	void							CurrentManagerSelectNext();
	void							CurrentManagerSelectPrevious();
	void							DebugSetScene(int key);
	ActionScene						*GetActionScene()			{ return m_pActionScene; }
	Scene							*GetCurrentScene()			{ return m_pCurrentScene; }
	DrawBuffer						*GetEffectsBuffer() const	{ return m_pEffectsBuffer; }
	Player							*GetPlayer()				{ return m_pPlayer; }
	DrawBuffer						*GetSceneBuffer() const		{ return m_pSceneBuffer; }
	const VECTOR2_INT				*GetGameWindowSize() const	{ return &m_vGameWindowSize; }
	const VECTOR2_INT				*GetGameWindowStart() const	{ return &m_vGameWindowStart; }
	const VECTOR2_INT				*GetTrueWindowSize() const	{ return &m_vTrueWindowSize; }
	void							HandleInput(const unsigned char &key, const int &x, const int &y, const unsigned int &dwKeyboardFlags);
	void							HandleMouseMove(const int &dwX, const int &dwY, const int &dwDeltaX, const int &dwDeltaY);
	void							HandleMouseMovePassive(const int &dwX, const int &dwY, const int &dwDeltaX, const int &dwDeltaY);
	void							Print();
	void							RaiseInputFlags(unsigned int dwFlags);
	void							Refresh(int dwDeltaTime);
	void							RemoveInputFlags(unsigned int dwFlags);
	void							RemoveObject(Object *pObject);
	void							Render();
	void							SelectNextManager();
	void							SelectPreviousManager();
	void							SetActionScene(ActionScene *pNewScene);
	void							SetCurrentScene(Scene *pNewScene);
	void							SetDebugRender(bool m_bNewDebug);
	void							SetFittingScene(FittingScene *pNewScene);
	void							SetGalaxyScene(GalaxyScene *pNewScene);
	void							SetManager(const char *pManagerName);
	void							SetNewScreenDimensions(int dwWindowX, int dwWindowY);
	void							SetNothingScene(NothingScene *pNewScene);
	void							SetPlayer(Player *pNewPlayer);
	void							SetSplashScene(SplashScene *pNewScene);

private:
	
	void							Init();
	void							ToggleDebugRender()			{ m_bDebugRender = !m_bDebugRender; }
	
	bool							m_bDebugRender;

	unsigned int					m_dwInputFlags;				// Current input flags - STRAFE, ROTATE, etc.  
																// This way multiple inputs can be used at once

	DrawBuffer						*m_pSceneBuffer;			//scene rendered to this
	DrawBuffer						*m_pEffectsBuffer;			//post process effects rendered to this
	
	Scene							*m_pCurrentScene;			// Current scene
	ActionScene						*m_pActionScene;			
	FittingScene					*m_pFittingScene;
	GalaxyScene						*m_pGalaxyScene;
	NothingScene					*m_pNothingScene;
	SplashScene						*m_pSplashScene;

	vector<Manager *>				m_vManagers;
	vector<Manager *>::iterator		m_SelectedManagerIterator;

	Player							*m_pPlayer;					//current player!

	VECTOR2_INT						m_vGameWindowStart;
	VECTOR2_INT						m_vGameWindowSize;
	VECTOR2_INT						m_vTrueWindowSize;
};

extern Game *gGame();

#endif