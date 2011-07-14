#include "Game.h"

#include "ActionScene.h"
#include "CollisionManager.h"
#include "DrawBuffer.h"
#include "EffectManager.h"
#include "EventManager.h"
#include "FittingScene.h"
#include "GalaxyScene.h"
#include "Node.h"
#include "NothingScene.h"
#include "Object.h"
#include "ObjectManager.h"
#include "Obstacle.h"
#include "ParticleManager.h"
#include "Player.h"
#include "Scene.h"
#include "Sector.h"
#include "ShaderManager.h"
#include "ShaderProgram.h"
#include "SplashScene.h"
#include "TextureManager.h"
#include "UnknownManager.h"
#include "Visual.h"
#include "VisualManager.h"

#include "Profile.h"
#include <GL/glut.h>

Game *gGame()
{
	static Game theGame;
	return &theGame;
}

Game::Game()
{
	Init();
}

Game::~Game()
{
	SetActionScene(0);
	SetCurrentScene(0);
	SetFittingScene(0);
	SetGalaxyScene(0);
	SetNothingScene(0);
	SetPlayer(0);
	SetSplashScene(0);

	vector<Manager *>::iterator iter = m_vManagers.begin();
	while (iter != m_vManagers.end())
	{
		(*iter)->Cleanup();
		iter++;
	}
}

void Game::CurrentManagerSelectNext()
{
	if (*m_SelectedManagerIterator)
	{
		(*m_SelectedManagerIterator)->SelectNext();
	}
}

void Game::CurrentManagerSelectPrevious()
{
	if (*m_SelectedManagerIterator)
	{
		(*m_SelectedManagerIterator)->SelectPrevious();
	}
}

void Game::DebugSetScene(int key)
{
	switch (key)
	{
	case GLUT_KEY_F5:
		SetCurrentScene(m_pSplashScene);
		break;

	case GLUT_KEY_F6:
		SetCurrentScene(m_pGalaxyScene);
		break;

	case GLUT_KEY_F7: //disabled for now, until there's actually something there...
		//SetCurrentScene(m_pFittingScene);
		break;

	case GLUT_KEY_F8:
		SetCurrentScene(m_pActionScene);
		break;

	case GLUT_KEY_F10:
		SetCurrentScene(m_pNothingScene);
		break;

	default:
		//shouldn't happen!!!
		break;
	}
}

void Game::HandleInput(const unsigned char &key, const int &x, const int &y, const unsigned int &dwKeyboardFlags)
{
	if ((key == '-') || (key == '_'))
	{
		CurrentManagerSelectPrevious();
	}
	else if ((key == '=') || (key == '+'))
	{
		CurrentManagerSelectNext();
	}

	if (m_pCurrentScene)
	{
		m_pCurrentScene->HandleInput(key, x, y, dwKeyboardFlags);
	}

	if (dwKeyboardFlags & KEYBOARD_CONTROL_ACTIVE)
	{
		if (key == 2)		//	'B' or 'b'
		{
			if (m_pCurrentScene)
			{
				m_pCurrentScene->ToggleBoundingBoxes();
			}
		}
		else if (key == 3)	//	'C' or 'c'
		{
			//SetManager("CollisionManager");
		}
		else if (key == 4)	//  'D' or 'd'
		{
			ToggleDebugRender();
		}
		else if (key == 20) //  'T' or 't'
		{
			SetManager("TextureManager");
		}
		else if (key == 21) //  'U' or 'u'
		{
			SetManager("UnknownManager");
		}
		else if (key == 15) //  'O' or 'o'
		{
			//SetManager("ObjectManager");
		}
		else if (key  == 22) // 'V' or 'v'
		{
			SetManager("VisualManager");
		}
	}
	else {
		//check for other flags?
	}
}

void Game::HandleMouseMove(const int &dwX, const int &dwY, const int &dwDeltaX, const int &dwDeltaY)
{
	if (m_pCurrentScene)
	{
		m_pCurrentScene->HandleMouseMove(dwX, dwY, dwDeltaX, dwDeltaY);
	}
}

void Game::HandleMouseMovePassive(const int &dwX, const int &dwY, const int &dwDeltaX, const int &dwDeltaY)
{
	if (m_pCurrentScene)
	{
		m_pCurrentScene->HandleMouseMovePassive(dwX, dwY, dwDeltaX, dwDeltaY);
	}
}


void Game::Init()
{
	m_dwInputFlags		= 0;
	m_bDebugRender		= false;

	Player *pNewPlayer	= new Player();
	SetPlayer(pNewPlayer);
	pNewPlayer->Release();

	m_pSceneBuffer		= 0;
	m_pEffectsBuffer	= 0;

	m_pCurrentScene		= 0;
	m_pActionScene		= new ActionScene();
	m_pFittingScene		= new FittingScene();
	m_pGalaxyScene		= new GalaxyScene();
	m_pNothingScene		= new NothingScene();
	m_pSplashScene		= new SplashScene();
	
	SetCurrentScene(m_pSplashScene); //set initial scene

	m_vManagers.push_back(gVisualManager());			
	m_vManagers.push_back(gTextureManager());			
	m_vManagers.push_back(gParticleManager());
	m_vManagers.push_back(gEventManager());
	m_vManagers.push_back(gCollisionManager());
	m_vManagers.push_back(gObjectManager());
//	m_vManagers.push_back(gEffectManager());
	m_vManagers.push_back(gUnknownManager());
	m_vManagers.push_back(gShaderManager());
	
	m_SelectedManagerIterator = m_vManagers.begin();
}

void Game::RaiseInputFlags(unsigned int dwFlags)
{
	m_dwInputFlags |= dwFlags;
}

void Game::Refresh(int dwDeltaTime)
{
	float fDeltaTime = ((float)dwDeltaTime) / 1000.0f;
	
	if (m_pCurrentScene)
	{
		m_pCurrentScene->RefreshAll(fDeltaTime);
		m_pCurrentScene->RefreshManagers(fDeltaTime);
	}
}	

void Game::RemoveInputFlags(unsigned int dwFlags)
{
	dwFlags = ~dwFlags;
	m_dwInputFlags &= dwFlags;
}

void Game::RemoveObject(Object *pObject)
{
	m_pActionScene->RemoveObject(pObject);
	m_pFittingScene->RemoveObject(pObject);
	m_pGalaxyScene->RemoveObject(pObject);
	m_pSplashScene->RemoveObject(pObject);
	m_pNothingScene->RemoveObject(pObject);
}

void Game::Render()
{
	if (m_bDebugRender)
	{
		m_pCurrentScene->Render(*m_SelectedManagerIterator);
	}
	else
	{
		m_pCurrentScene->Render(0);
	}
}

void Game::SelectNextManager()
{
	m_SelectedManagerIterator++;
}

void Game::SelectPreviousManager()
{
	m_SelectedManagerIterator--;
}

void Game::SetActionScene(ActionScene *pNewScene)
{
	if (pNewScene != m_pActionScene)
	{
		if (m_pActionScene)
		{
			m_pActionScene->Release();
			m_pActionScene = 0;
		}

		if (pNewScene)
		{
			m_pActionScene = pNewScene;
			m_pActionScene->AddRef();
		}
	}
}

void Game::SetCurrentScene(Scene *pNewScene)
{
	if (pNewScene != m_pCurrentScene)
	{
		if (m_pCurrentScene)
		{
			m_pCurrentScene->SceneEnd();
			m_pCurrentScene->Release();
			m_pCurrentScene = 0;
		}

		if (pNewScene)
		{
			m_pCurrentScene = pNewScene;
			m_pCurrentScene->AddRef();
			m_pCurrentScene->SceneStart();
		}
	}
}

void Game::SetDebugRender(bool m_bNewDebug)
{
	m_bDebugRender = m_bNewDebug;
}

void Game::SetFittingScene(FittingScene *pNewScene)
{
	if (pNewScene != m_pFittingScene)
	{
		if (m_pFittingScene)
		{
			m_pFittingScene->Release();
			m_pFittingScene = 0;
		}

		if (pNewScene)
		{
			m_pFittingScene = pNewScene;
			m_pFittingScene->AddRef();
		}
	}
}

void Game::SetGalaxyScene(GalaxyScene *pNewScene)
{
	if (pNewScene != m_pGalaxyScene)
	{
		if (m_pGalaxyScene)
		{
			m_pGalaxyScene->Release();
			m_pGalaxyScene = 0;
		}

		if (pNewScene)
		{
			m_pGalaxyScene = pNewScene;
			m_pGalaxyScene->AddRef();
		}
	}
}

void Game::SetManager(const char *pManagerName)
{
	if (!pManagerName)
	{
		return;
	}
	
	vector<Manager *>::iterator iter;
	for (iter = m_vManagers.begin(); iter != m_vManagers.end(); iter++)
	{
		if (strcmp((*iter)->GetManagerName(), pManagerName) == 0)
		{
			//found the manager we want!
			m_SelectedManagerIterator = iter;
			break;
		}
	}
}

void Game::SetNewScreenDimensions(int dwWindowX, int dwWindowY)
{
	if (m_vTrueWindowSize.x == dwWindowX && m_vTrueWindowSize.y == dwWindowY)
	{
		return; //dimensions haven't changed, so don't do anything!
	}

	m_vTrueWindowSize.x			= dwWindowX;
	m_vTrueWindowSize.y			= dwWindowY;

	int dwWindowYModified		= dwWindowY * 4 / 3;
	m_vGameWindowStart.x		= 0;
	m_vGameWindowStart.y		= 0;
	m_vGameWindowSize.x			= 0;
	m_vGameWindowSize.y			= 0;

	//We want to calculate the total viewport for a 4:3 aspect ratio.  
	if (dwWindowX >= dwWindowYModified) 
	{
		m_vGameWindowStart.x	= (dwWindowX - dwWindowYModified) / 2;
		m_vGameWindowSize.x		= dwWindowYModified;
		m_vGameWindowSize.y		= dwWindowY;
	}
	else 
	{
		m_vGameWindowStart.y	= (dwWindowY - (dwWindowX * 3 / 4)) / 2;
		m_vGameWindowSize.y		= dwWindowX * 3 / 4;
		m_vGameWindowSize.x		= dwWindowX;
	}

	//update buffers draw buffers!
	if (m_pSceneBuffer)
	{
		m_pSceneBuffer->Release();
		m_pSceneBuffer = 0;
	}		
	if (m_pEffectsBuffer)
	{
		m_pEffectsBuffer->Release();
		m_pSceneBuffer = 0;
	}

	//make new buffers
	m_pSceneBuffer		= new DrawBuffer(m_vGameWindowSize.x, m_vGameWindowSize.y);
	m_pEffectsBuffer	= new DrawBuffer(m_vGameWindowSize.x, m_vGameWindowSize.y);

	printf("Created buffers (%d, %d)\n", m_vGameWindowSize.x, m_vGameWindowSize.y);
}

void Game::SetNothingScene(NothingScene *pNewScene)
{
	if (m_pNothingScene != pNewScene)
	{
		if (m_pNothingScene)
		{
			m_pNothingScene->Release();
			m_pNothingScene = 0;
		}

		if (pNewScene)
		{
			m_pNothingScene = pNewScene;
			m_pNothingScene->AddRef();
		}
	}
}

void Game::SetPlayer(Player *pNewPlayer)
{
	if (m_pPlayer != pNewPlayer)
	{
		if (m_pPlayer)
		{
			m_pPlayer->Release();
			m_pPlayer = 0;
		}

		if (pNewPlayer)
		{
			m_pPlayer = pNewPlayer;
			m_pPlayer->AddRef();
		}
	}
}

void Game::SetSplashScene(SplashScene *pNewScene)
{
	if (pNewScene != m_pSplashScene)
	{
		if (m_pSplashScene)
		{
			m_pSplashScene->Release();
			m_pSplashScene = 0;
		}

		if (pNewScene)
		{
			m_pSplashScene = pNewScene;
			m_pSplashScene->AddRef();
		}
	}
}