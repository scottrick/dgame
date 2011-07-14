#include "GalaxyScene.h"

#include "ActionScene.h"
#include "Camera.h"
#include "ComputerShip.h"
#include "EffectManager.h"
#include "EventManager.h"
#include "GalaxyEffect.h"
#include "Game.h"
#include "Manager.h"
#include "Node.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Sector.h"
#include "Text.h"
#include "TextureManager.h"
#include "UIEffect.h"
#include "UIScroller.h"
#include "UnknownManager.h"
#include "VisualManager.h"

#include <GL/glut.h>

GalaxyScene::GalaxyScene()
{
	Init();
}

GalaxyScene::~GalaxyScene()
{
	SetDescriptionText(0);
	SetNameText(0);
	SetOpponentsText(0);
	SetScroller(0);
}

void GalaxyScene::HandleInput(const unsigned char &key, const int &x, const int &y, const unsigned int &dwKeyboardFlags)
{
	if (m_pScroller)
	{
		if ((key == 'a') || (key == 'A') || (key == '1') || (key == '-') || (key == '_'))
		{
			m_pScroller->SelectPrev();
			UpdateSectorText();
		}
		else if ((key == 'd') || (key == 'D') || (key == '2') || (key == '+') || (key == '='))
		{
			m_pScroller->SelectNext();
			UpdateSectorText();
		}
		else if (key == 13) //ENTER
		{	
			gGame()->GetPlayer()->SetTargetSector(gGame()->GetPlayer()->GetSector(m_pScroller->GetSelectedIndex()));
			
			//set the currently selected scene
			gGame()->SetCurrentScene(gGame()->GetActionScene());
		}
	}
}	

void GalaxyScene::Init()
{
	m_pDescriptionText		= 0;
	m_pNameText				= 0;
	m_pOpponentsText		= 0;
	m_pScroller				= 0;
}

void GalaxyScene::Print(int indent) const
{
	printf("%*sGalaxyScene 0x%p\n", indent, " ", this);
	Scene::Print(indent);
}

void GalaxyScene::SceneEnd()
{
	Scene::SceneEnd();

	SetDescriptionText(0);
}

void GalaxyScene::SceneStart()
{
	Scene::SceneStart();

	//setup the camera
	Camera *pCamera = new Camera();
	PosQuat pq;
	pq.pos.z = 10.0f;
	pCamera->SetPosQuat(pq);
	SetCamera(pCamera);
	pCamera->Release();

	//create ui scroller for the sector objects
	UIScroller *pNewScroller = new UIScroller();
	for (unsigned int i = 0; i < gGame()->GetPlayer()->GetSectors()->size(); i++)
	{
		Sector *pSector = gGame()->GetPlayer()->GetSector(i);
		if (pSector)
		{
			UIEffect *pNewUIEffect = new UIEffect();
			pNewUIEffect->SetEffect(pSector->GetGalaxyEffect());
			pNewScroller->AddUIObject(pNewUIEffect);
			pNewUIEffect->Release();
		}
	}

	this->AddUIObject(pNewScroller);
	SetScroller(pNewScroller);
	pNewScroller->SetDimensions(VECTOR3(m_pCamera->GetPosQuat().pos.z * 1.333f, 1.0f, 1.0f));
	pNewScroller->SetRenderBlending(true);
	pNewScroller->SelectNext();
	pNewScroller->Release();

	//Setup menu title
	float	fTextScale						= 2.5f;
	size_t	BUFFER_SIZE						= 50;
	char	*pBuffer						= (char *) malloc(sizeof(char) * BUFFER_SIZE);
	sprintf_s(pBuffer, BUFFER_SIZE, "Sectors");

	Text	*pGalaxyText					= new Text(pBuffer, VECTOR3(0.2f, 0.3f, 0.95f));
	pGalaxyText->GetNode()->m_PosQuat.pos.x	-= pGalaxyText->GetTextNode()->GetTextWidth() / 2.0f * fTextScale;
	pGalaxyText->GetNode()->m_PosQuat.pos.y	+= 2.5f;
	pGalaxyText->GetNode()->m_vScale.x		= fTextScale;
	pGalaxyText->GetNode()->m_vScale.y		= fTextScale;
	AddObject(pGalaxyText);
	pGalaxyText->Release();

	fTextScale								= 0.4f;
	BUFFER_SIZE								= 50;
	pBuffer									= (char *) malloc(sizeof(char) * BUFFER_SIZE);
	sprintf_s(pBuffer, BUFFER_SIZE, "+ and - change selection, ENTER begins");

	Text	*pDescText						= new Text(pBuffer, VECTOR3(0.7f, 0.4f, 0.0f));
	pDescText->GetNode()->m_PosQuat.pos.x	-= pDescText->GetTextNode()->GetTextWidth() / 2.0f * fTextScale;
	pDescText->GetNode()->m_PosQuat.pos.y	+= 2.0f;
	pDescText->GetNode()->m_vScale.x		= fTextScale;
	pDescText->GetNode()->m_vScale.y		= fTextScale;
	AddObject(pDescText);
	pDescText->Release();

	UpdateSectorText();
}

void GalaxyScene::SetDescriptionText(Text *pText)
{
	if (pText != m_pDescriptionText)
	{
		if (m_pDescriptionText)
		{
			RemoveObject(m_pDescriptionText);
			m_pDescriptionText->Release();
			m_pDescriptionText = 0;
		}

		if (pText)
		{
			m_pDescriptionText = pText;
			AddObject(m_pDescriptionText);
			m_pDescriptionText->AddRef();
		}
	}
}

void GalaxyScene::SetNameText(Text *pText)
{
	if (pText != m_pNameText)
	{
		if (m_pNameText)
		{
			RemoveObject(m_pNameText);
			m_pNameText->Release();
			m_pNameText = 0;
		}

		if (pText)
		{
			m_pNameText = pText;
			AddObject(m_pNameText);
			m_pNameText->AddRef();
		}
	}
}

void GalaxyScene::SetOpponentsText(Text *pText)
{
	if (pText != m_pOpponentsText)
	{
		if (m_pOpponentsText)
		{
			RemoveObject(m_pOpponentsText);
			m_pOpponentsText->Release();			
			m_pOpponentsText = 0;
		}

		if (pText)
		{
			m_pOpponentsText = pText;
			AddObject(m_pOpponentsText);
			m_pOpponentsText->AddRef();
		}
	}
}

void GalaxyScene::SetScroller(UIScroller *pScroller)
{
	if (m_pScroller != pScroller)
	{
		if (m_pScroller)
		{
			m_pScroller->Release();
			m_pScroller = 0;
		}

		if (pScroller)
		{
			m_pScroller = pScroller;
			m_pScroller->AddRef();
		}
	}
}

void GalaxyScene::UpdateSectorText()
{
	if (gGame()->GetPlayer()->GetSectors()->size() > 0)
	{
		Sector	*pSector						= gGame()->GetPlayer()->GetSector(this->m_pScroller->GetSelectedIndex());
		//Setup description text
		float	fTextScale						= 1.2f;
		size_t	BUFFER_SIZE						= 250;
		char	*pBuffer						= (char *) malloc(sizeof(char) * BUFFER_SIZE);
		sprintf_s(pBuffer, BUFFER_SIZE, pSector->GetSectorName());

		Text	*pNewText						= new Text(pBuffer, VECTOR3(0.0f, 0.4f, 0.0f));
		pNewText->GetNode()->m_PosQuat.pos.x	-= pNewText->GetTextNode()->GetTextWidth() / 2.0f * fTextScale;
		pNewText->GetNode()->m_PosQuat.pos.y	-= 2.4f;
		pNewText->GetNode()->m_vScale.x			= fTextScale;
		pNewText->GetNode()->m_vScale.y			= fTextScale;
		SetNameText(pNewText);
		pNewText->Release();

		sprintf_s(pBuffer, BUFFER_SIZE, "Level %d (of %d)", pSector->GetLevel(), gGame()->GetPlayer()->GetSectors()->size());
		fTextScale								= 0.35f;

		Text	*pNewerText						= new Text(pBuffer, VECTOR3(0.7f, 0.7f, 0.7f));
		pNewerText->GetNode()->m_PosQuat.pos.x	= pNewText->GetNode()->m_PosQuat.pos.x;
		pNewerText->GetNode()->m_PosQuat.pos.y	-= 3.0f;
		pNewerText->GetNode()->m_vScale.x		= fTextScale;
		pNewerText->GetNode()->m_vScale.y		= fTextScale;
		SetDescriptionText(pNewerText);
		pNewerText->Release();

		sprintf_s(pBuffer, BUFFER_SIZE, "(1) %s, (2) %s, and (3) %s", pSector->GetShip(0)->GetShipName().c_str(), pSector->GetShip(1)->GetShipName().c_str(), pSector->GetShip(2)->GetShipName().c_str());
		fTextScale								= 0.35f;

		pNewerText								= new Text(pBuffer, VECTOR3(0.7f, 0.7f, 0.7f));
		pNewerText->GetNode()->m_PosQuat.pos.x	= pNewText->GetNode()->m_PosQuat.pos.x;
		pNewerText->GetNode()->m_PosQuat.pos.y	-= 3.5f;
		pNewerText->GetNode()->m_vScale.x		= fTextScale;
		pNewerText->GetNode()->m_vScale.y		= fTextScale;
		SetOpponentsText(pNewerText);
		pNewerText->Release();
	}
}