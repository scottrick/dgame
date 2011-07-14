#include "Background.h"

#include "BackgroundNode.h"
#include "GameCamera.h"
#include "Visual.h"

Background::Background()
{
	Init();
}

Background::~Background()
{
	SetCamera(0);
}

void Background::FromBlock(Block *pBlock)
{

}

void Background::Init()
{
	SetObjectType(Object::eTYPE_BACKGROUND);

	m_pGameCamera			= 0;
	m_bRecalculate			= false;
	m_fScrollSpeed			= 5.75f;

	//setup background node
	BackgroundNode *pNode	= new BackgroundNode();
	pNode->SetVisual("data\\visuals\\test_background.3ds");
	SetNode(pNode);
	pNode->Release();
}

/*
	Recalculate the scale based on the current camera settings!
*/
void Background::Recalculate()
{
	if (m_pGameCamera && m_pNode)
	{
		//want to set background 10.0f behind the game
		float fDistance			= BACKGROUND_DISTANCE_FROM_GAME + m_pGameCamera->GetPosQuat().pos.z;
		float fScale			= tan((1.0f / 6.0f) * PI) * fDistance * 2.0f;
	
		//always square map for now, so this is eZ
		VECTOR3					vNewScale;
		vNewScale.x				= fScale;
		vNewScale.y				= fScale;
		vNewScale.z				= fScale;
		
		m_pNode->SetScale(vNewScale);
		m_pNode->m_PosQuat.pos.z = -BACKGROUND_DISTANCE_FROM_GAME;
	}
}

void Background::Refresh(const float &fDeltaTime)
{
	if (m_bRecalculate)
	{
		Recalculate();	
		m_bRecalculate = false;
	}

	if (m_pNode)
	{	
		m_pNode->m_PosQuat.pos.y		-= m_fScrollSpeed * fDeltaTime;
	}
}

void Background::SetCamera(Camera *pNewCamera)
{
	if (m_pGameCamera != pNewCamera)
	{
		if (m_pGameCamera)
		{
			m_pGameCamera->Release();
			m_pGameCamera = 0;
		}

		if ((pNewCamera) && (pNewCamera->GetCameraType() == Camera::eCAMERA_GAME))
		{
			m_pGameCamera = (GameCamera *) pNewCamera;
			m_pGameCamera->AddRef();
			m_bRecalculate = true;
		}
	}
}

Block *Background::ToBlock()
{
	return 0;
}