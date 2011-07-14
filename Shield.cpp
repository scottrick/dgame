#include "Shield.h"

#include "Block.h"
#include "Node.h"
#include "TextureManager.h"

#include <GL/glut.h>

Shield::Shield(Node *pNode)
{
	Init();

	SetNode(pNode);
}

Shield::~Shield()
{
	SetNode(0);
}

void Shield::Clone(Shield *pClone)
{
	Effect::Clone(pClone);

	SetColor(pClone->GetColor());
}

void Shield::FromBlock(Block *pBlock)
{

}

void Shield::Init()
{
	m_fCurrentFade		= 0.0f;
	m_fFadeTime			= 0.2f;
	m_vColor			= VECTOR3(1.0f, 1.0f, 1.0f); //default white
	m_pNode				= 0;
	m_bActive			= false;
}

void Shield::Ping()
{
	m_fCurrentFade		= m_fFadeTime;
	m_bActive			= true;
}

void Shield::Refresh(const float &fDeltaTime)
{
	if (!m_bActive)
	{
		return;
	}

	m_fCurrentFade -= fDeltaTime;
	if (m_fCurrentFade < 0.0f)
	{
		m_fCurrentFade = 0.0f;
		m_bActive = false;
	}
}

void Shield::Render()
{
	if (!m_bActive || !m_pNode)
	{ //must be active and
		return; //can't render without something to shield...
	}

	static int dwShieldTexture	= gTextureManager()->GetTextureByFileName("shield.tga");

	float fColorScale = m_fCurrentFade / m_fFadeTime;
	if (fColorScale > 0.5f)
	{
		fColorScale = 1.0f;
	}
	else
	{
		fColorScale *= 2.0f;
	}

	glColor3f(m_vColor.x * m_fCurrentFade, m_vColor.y * m_fCurrentFade, m_vColor.z * m_fCurrentFade);
	glBindTexture(GL_TEXTURE_2D, dwShieldTexture);

	glPushMatrix();

	//translate to correct position
	glTranslatef(m_pNode->m_PosQuat.pos.x, m_pNode->m_PosQuat.pos.y, m_pNode->m_PosQuat.pos.z);

	VECTOR3 vScale = m_pNode->GetScale();
	glScalef(vScale.x * 1.3f, vScale.y * 1.3f, vScale.z * 1.3f);

	glCallList(m_dwTexturedQuadDisplayList);

	glPopMatrix();
}

void Shield::SetColor(const VECTOR3 &vColor)
{
	m_vColor = vColor;	
}

void Shield::SetNode(Node *pNode)
{
	if (m_pNode != pNode)
	{
		if (m_pNode)
		{
			m_pNode->Release();
			m_pNode = 0;
		}

		if (pNode)
		{
			m_pNode = pNode;
			m_pNode->AddRef();
		}
	}
}

Block *Shield::ToBlock()
{
	return 0;
}