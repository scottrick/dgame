#include "FittingScene.h"

#include "Camera.h"
#include "EffectManager.h"
#include "EventManager.h"
#include "Game.h"
#include "Manager.h"
#include "Node.h"
#include "Text.h"
#include "TextureManager.h"
#include "UIContainer.h"
#include "UnknownManager.h"
#include "VisualManager.h"

#include <GL/glut.h>

FittingScene::FittingScene()
{
	Init();
}

FittingScene::~FittingScene()
{
	
}

void FittingScene::Init()
{

}

void FittingScene::Print(int indent) const
{
	printf("%*sFittingScene 0x%p\n", indent, " ", this);
	Scene::Print(indent);
}

void FittingScene::SceneEnd()
{
	Scene::SceneEnd();
}

void FittingScene::SceneStart()
{
	Scene::SceneStart();

	//Setup menu title
	float	fTextScale							= 2.5f;
	size_t	BUFFER_SIZE							= 50;
	char	*pBuffer							= (char *) malloc(sizeof(char) * BUFFER_SIZE);
	sprintf_s(pBuffer, BUFFER_SIZE, "Ship Fitting");

	Text	*pFittingText						= new Text(pBuffer, VECTOR3(1.0f, 0.15f, 0.0f));
	pFittingText->GetNode()->m_PosQuat.pos.x	-= pFittingText->GetTextNode()->GetTextWidth() / 2.0f * fTextScale;
	pFittingText->GetNode()->m_PosQuat.pos.y	+= 1.5f;
	pFittingText->GetNode()->m_vScale.x			= fTextScale;
	pFittingText->GetNode()->m_vScale.y			= fTextScale;
	AddObject(pFittingText);
	pFittingText->Release();

	Camera *pCamera = new Camera();
	PosQuat pq;
	pq.pos.z = 10.0f;
	pCamera->SetPosQuat(pq);
	SetCamera(pCamera);
	pCamera->Release();
}