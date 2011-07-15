#include "SplashScene.h"

#include "Camera.h"
#include "EffectManager.h"
#include "EventManager.h"
#include "ExplosionEffect.h"
#include "Game.h"
#include "Manager.h"
#include "Node.h"
#include "PosQuat.h"
#include "SplashEffect.h"
#include "TestShockwaveThing.h"
#include "Text.h"
#include "TextNode.h"
#include "TextureManager.h"
#include "VisualManager.h"
#include "UIContainer.h"
#include "UnknownManager.h"

#include <GL/glut.h>

SplashScene::SplashScene()
{
	Init();
}

SplashScene::~SplashScene()
{
	
}

void SplashScene::Init()
{
	m_bDateParsed	= false;
	m_dwMonth		= -1;
	m_dwDay			= -1;
	m_dwYear		= -1;
}

void SplashScene::ParseDate()
{
	char month[20];
	char date[] = __DATE__;

	sscanf_s(date, "%s %d %d", month, sizeof(month), &m_dwDay, &m_dwYear);

	m_dwYear = m_dwYear % 1000;

	if (strcmp(month, "Jan") == 0) {
		m_dwMonth = 1;
	} else if (strcmp(month, "Feb") == 0) {
		m_dwMonth = 2;
	} else if (strcmp(month, "Mar") == 0) {
		m_dwMonth = 3;
	} else if (strcmp(month, "Apr") == 0) {
		m_dwMonth = 4;
	} else if (strcmp(month, "May") == 0) {
		m_dwMonth = 5;
	} else if (strcmp(month, "Jun") == 0) {
		m_dwMonth = 6;
	} else if (strcmp(month, "Jul") == 0) {
		m_dwMonth = 7;
	} else if (strcmp(month, "Aug") == 0) {
		m_dwMonth = 8;
	} else if (strcmp(month, "Sep") == 0) {
		m_dwMonth = 9;
	} else if (strcmp(month, "Oct") == 0) {
		m_dwMonth = 10;
	} else if (strcmp(month, "Nov") == 0) {
		m_dwMonth = 11;
	} else if (strcmp(month, "Dec") == 0) {
		m_dwMonth = 12;
	} else {
		assert(0 && "Did not recognize date!  Fix me!");
	}

	m_bDateParsed = true;
}	

void SplashScene::Print(int indent) const
{
	printf("%*sSplashScene 0x%p\n", indent, " ", this);
	Scene::Print(indent);
}

void SplashScene::SceneEnd()
{
	Scene::SceneEnd();
}

void SplashScene::SceneStart()
{
	Scene::SceneStart();

	if (!m_bDateParsed)
	{
		ParseDate();
	}

	float	fTextScale				= 3.0f;
	size_t	BUFFER_SIZE				= 50;
	char	*pBuffer				= (char *) malloc(sizeof(char) * BUFFER_SIZE);
	sprintf_s(pBuffer, BUFFER_SIZE, "dGame");

	Text	*pGameNameText						= new Text(pBuffer, VECTOR3(1.0f, 0.0f, 0.0f));
	pGameNameText->GetNode()->m_PosQuat.pos.x	-= pGameNameText->GetTextNode()->GetTextWidth() / 2.0f * fTextScale;
	pGameNameText->GetNode()->m_PosQuat.pos.y	+= 3.0f;
	pGameNameText->GetNode()->m_vScale.x		= fTextScale;
	pGameNameText->GetNode()->m_vScale.y		= fTextScale;
	//pGameNameText->SetRotate(true); //JUST FOR FUN 
	AddObject(pGameNameText);
	pGameNameText->Release();

	fTextScale									= 0.60f;
	VECTOR3	vColor								= VECTOR3(0.8f, 0.8f, 0.8f);
	sprintf_s(pBuffer, BUFFER_SIZE, "work in progress");
	Text	*pWork								= new Text(pBuffer, vColor);
	pWork->GetNode()->m_PosQuat.pos.x			-= pWork->GetTextNode()->GetTextWidth() / 2.0f * fTextScale;
	pWork->GetNode()->m_PosQuat.pos.y			+= 2.45f;
	pWork->GetNode()->m_vScale.x				= fTextScale;
	pWork->GetNode()->m_vScale.y				= fTextScale;
	AddObject(pWork);
	pWork->Release();

	fTextScale									= 0.38;
	vColor										= VECTOR3(0.2f, 0.2f, 0.2f);
	sprintf_s(pBuffer, BUFFER_SIZE, "version %d.%d.%d.%d.%d", GAME_VERSION, GAME_SUBVERSION, m_dwYear, m_dwMonth, m_dwDay);
	Text	*pLine0								= new Text(pBuffer, vColor);
	pLine0->GetNode()->m_PosQuat.pos.x			-= pLine0->GetTextNode()->GetTextWidth() / 2.0f * fTextScale;
	pLine0->GetNode()->m_PosQuat.pos.y			+= 2.0f;
	pLine0->GetNode()->m_vScale.x				= fTextScale;
	pLine0->GetNode()->m_vScale.y				= fTextScale;
	AddObject(pLine0);
	pLine0->Release();

	fTextScale									= 0.9f;
	vColor										= VECTOR3(0.1f, 0.1f, 1.0f);
	Text	*pLine1								= new Text("Navigation", vColor);
	pLine1->GetNode()->m_PosQuat.pos.x			-= pLine1->GetTextNode()->GetTextWidth() / 2.0f * fTextScale;
	pLine1->GetNode()->m_PosQuat.pos.y			+= 0.3f;
	pLine1->GetNode()->m_vScale.x				= fTextScale;
	pLine1->GetNode()->m_vScale.y				= fTextScale;
	AddObject(pLine1);
	pLine1->Release();

	vColor										= VECTOR3(0.25f, 0.25f, 1.0f);
	Text	*pLine2								= new Text("F5 - Splash", vColor);
	pLine2->GetNode()->m_PosQuat.pos.x			-= pLine2->GetTextNode()->GetTextWidth() / 2.0f * fTextScale;
	pLine2->GetNode()->m_PosQuat.pos.y			-= 0.7f;
	pLine2->GetNode()->m_vScale.x				= fTextScale;
	pLine2->GetNode()->m_vScale.y				= fTextScale;
	AddObject(pLine2);
	pLine2->Release();

	Text	*pLine3								= new Text("F6 - Available Sectors", vColor);
	pLine3->GetNode()->m_PosQuat.pos.x			-= pLine3->GetTextNode()->GetTextWidth() / 2.0f * fTextScale;
	pLine3->GetNode()->m_PosQuat.pos.y			-= 1.7f;
	pLine3->GetNode()->m_vScale.x				= fTextScale;
	pLine3->GetNode()->m_vScale.y				= fTextScale;
	AddObject(pLine3);
	pLine3->Release();

	Text	*pLine4								= new Text("F7 - Ship Fitting (disabled)", vColor);
	pLine4->GetNode()->m_PosQuat.pos.x			-= pLine4->GetTextNode()->GetTextWidth() / 2.0f * fTextScale;
	pLine4->GetNode()->m_PosQuat.pos.y			-= 2.7f;
	pLine4->GetNode()->m_vScale.x				= fTextScale;
	pLine4->GetNode()->m_vScale.y				= fTextScale;
	AddObject(pLine4);
	pLine4->Release();

	Text	*pLine5								= new Text("F8 - Game", vColor);
	pLine5->GetNode()->m_PosQuat.pos.x			-= pLine5->GetTextNode()->GetTextWidth() / 2.0f * fTextScale;
	pLine5->GetNode()->m_PosQuat.pos.y			-= 3.7f;
	pLine5->GetNode()->m_vScale.x				= fTextScale;
	pLine5->GetNode()->m_vScale.y				= fTextScale;
	AddObject(pLine5);
	pLine5->Release();

	Text	*pLine6								= new Text("F10 - Debug", vColor);
	pLine6->GetNode()->m_PosQuat.pos.x			-= pLine6->GetTextNode()->GetTextWidth() / 2.0f * fTextScale;
	pLine6->GetNode()->m_PosQuat.pos.y			-= 4.7f;
	pLine6->GetNode()->m_vScale.x				= fTextScale;
	pLine6->GetNode()->m_vScale.y				= fTextScale;
	AddObject(pLine6);
	pLine6->Release();

	fTextScale									= 0.3f;
	vColor										= VECTOR3(0.75f, 0.75f, 0.75f);
	Text	*pInfo								= new Text("Written by Scott Atkins, 2009  -  atki0090@umn.edu", vColor);
	pInfo->GetNode()->m_PosQuat.pos.x			-= pInfo->GetTextNode()->GetTextWidth() / 2.0f * fTextScale;
	pInfo->GetNode()->m_PosQuat.pos.y			-= 5.5f;
	pInfo->GetNode()->m_vScale.x				= fTextScale;
	pInfo->GetNode()->m_vScale.y				= fTextScale;
	AddObject(pInfo);
	pInfo->Release();

	/*
	TestShockwaveThing *pTestThing = new TestShockwaveThing();
	AddObject(pTestThing);
	pTestThing->Release();
	*/

	SplashEffect *pSplashEffect = new SplashEffect();
	AddObject(pSplashEffect);
	pSplashEffect->Release();

	/*
	Node *pNode = new Node();
	ExplosionEffect *pTestExplosion = new ExplosionEffect(1.0f, pNode);
	pNode->Release();
	AddObject(pTestExplosion);
	pTestExplosion->Release();
	*/

	Camera *pCamera = new Camera();
	PosQuat pq;
	pq.pos.z = 10.0f;
	pCamera->SetPosQuat(pq);
	SetCamera(pCamera);
	pCamera->Release();
}