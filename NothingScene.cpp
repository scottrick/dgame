#include "NothingScene.h"

#include "EffectManager.h"
#include "EventManager.h"
#include "Game.h"
#include "Manager.h"
#include "Matrix44.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "UnknownManager.h"
#include "utility.h"
#include "VisualManager.h"

#include <GL/glut.h>

NothingScene::NothingScene()
{
	Init();
}

NothingScene::~NothingScene()
{
	
}

void NothingScene::Init()
{
	m_fScale = 2.0f;
}

void NothingScene::HandleInput(const unsigned char &key, const int &x, const int &y, const unsigned int &dwKeyboardFlags)
{
	if (key == '[' || key == '{')
	{
		m_fScale *= 0.91f;
		
		if (m_fScale < 0.1f)
		{
			m_fScale = 0.1f;
		}
	}
	else if (key == ']' || key == '}')
	{
		m_fScale *= 1.1f;
	}	
	else if (key == 'r' || key == 'R')
	{
		//gVisualManager()->ToggleAutoRotate();
		m_fScale = 2.0f;
		m_PosQuat.quat.CreateFromRotationDEGREES(0.0f, 1.0f, 0.0f, 0.0f);
	}
}

void NothingScene::HandleMouseMove(const int &dwX, const int &dwY, const int &dwDeltaX, const int &dwDeltaY)
{	
	//update the rotation for debug display based on the mouse movement!
	Quat *quat = &(m_PosQuat.quat);
	Quat rotation;
	VECTOR3 axis; 	
	axis.x = (float) dwDeltaY;	
	axis.y = (float) dwDeltaX;	

	rotation.CreateFromRotationRADIANS(PI / 360.0f * (float)(abs(dwDeltaX) + abs(dwDeltaY)), axis.x, axis.y, axis.z);
	*quat = rotation * *quat;
}

void NothingScene::Print(int indent) const
{
	printf("%*sNothingScene 0x%p\n", indent, " ", this);
	Scene::Print(indent);
}

void NothingScene::RenderDebug(Manager *pManager)
{
	const VECTOR2_INT *pGameWindowSize		= gGame()->GetGameWindowSize();
	const VECTOR2_INT *pGameWindowStart		= gGame()->GetGameWindowStart();
	bool bPostProcessing					= (GetNumPostRenderEffects() > 0) && (gShaderManager()->IsShadersSupported());

	//use the whole window on debug for the NothingScene
	if (bPostProcessing) {
		glViewport(0, 0, pGameWindowSize->x, pGameWindowSize->y);
	}
	else {
		glViewport(pGameWindowStart->x, pGameWindowStart->y, pGameWindowSize->x, pGameWindowSize->y);
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, 1.33333333, 0.1, 50.0); //update this so we dont get crappy distortion
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 2.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glPushMatrix();

	GLfloat fLightPos[] = { 0.0f, 0.0f, 10.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, fLightPos);

	glScalef(m_fScale, m_fScale, m_fScale);
	pManager->DebugInfoRender(1.33333333f, m_PosQuat);

	glPopMatrix();
}

void NothingScene::SceneEnd()
{
	Scene::SceneEnd();

	gVisualManager()->SetAutoRotate(true);
	gGame()->SetDebugRender(false);
}

void NothingScene::SceneStart()
{
	Scene::SceneStart();

	PosQuat pq;
	gVisualManager()->SetAutoRotate(false);
	gVisualManager()->SetPosQuat(pq);
	m_PosQuat = pq;
	gGame()->SetDebugRender(true);
}