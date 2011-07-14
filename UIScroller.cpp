#include "UIScroller.h"

#include "Block.h"
#include "Effect.h"
#include "TextureManager.h"

#include <GL/glut.h>

UIScroller::UIScroller()
{
	Init();
}

UIScroller::~UIScroller()
{
	RemoveAllUIObjects();
}

void UIScroller::AddUIObject(UIObject *pUIObject)
{
	if (pUIObject)
	{
		m_vContents.push_back(pUIObject);
		pUIObject->AddRef();
	}
}

void UIScroller::FromBlock(Block *pBlock)
{

}

void UIScroller::HandleInput(const unsigned char &key, const int &x, const int &y, const unsigned int &dwKeyboardFlags)
{
	printf("somebody hit something with UISCroller in focus!!!\n");
}

void UIScroller::Init()
{
	m_dwMaxDisplayable		= 7;
	m_dwMinDisplayed		= 0;
	m_bCircular				= false;
	m_bRenderWithBlending	= false;

	m_fTimeLeftToScroll		= 0.0f;
	m_fTimePerScroll		= 0.6f;

	m_dwSelected			= -1;

	m_vDimensions			= VECTOR3(1.0f, 1.0f, 1.0f);
}

void UIScroller::Refresh(const float &fDeltaTime)
{
	vector<UIObject *>::iterator iter = m_vContents.begin();
	while (iter != m_vContents.end())
	{
		UIObject *pObject = *iter;
		if (pObject)
		{
			pObject->Refresh(fDeltaTime);
		}

		iter++;
	}	
}

void UIScroller::RemoveAllUIObjects()
{
	vector<UIObject *>::iterator iter = m_vContents.begin();
	while (iter != m_vContents.end())
	{
		UIObject *pObject = *iter;
		if (pObject)
		{
			pObject->Release();
		}

		iter++;
	}

	m_vContents.clear();
}

void UIScroller::Render()
{ //render inside 1x1x1 cube...
	static int dwCircleTexture = gTextureManager()->GetTextureByFileName("shield.tga");

	if (m_bRenderWithBlending)
	{
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glDisable(GL_LIGHTING);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glNormal3f(0.0f, 0.0f, 1.0f); //normal always towards the camera
	}

	float			fScaleX			= m_vDimensions.x / (float)m_dwMaxDisplayable;
	//float			fScaleY			= m_vDimensions.y;
	float			fScaleY			= fScaleX;	//for now, always scaled to the X dimension...

	if (fScaleX < fScaleY) {
		fScaleY = fScaleX;
	}

	float			fCurrentX		= (float)m_dwMaxDisplayable / -2.0f * fScaleX + fScaleX / 2.0f;
	UIObject		*pObject		= 0;

	glPushMatrix();
//	glScalef(fScaleX, fScaleY, 1.0f);
	glTranslatef(fCurrentX, 0.0f, 0.0f);

	int dwNumRendered = 0;

	int i = m_dwMinDisplayed;//m_vContents.size() - m_dwMaxDisplayable;

	while (m_dwSelected < i)
	{
		--i;
	}
	while (m_dwSelected >= i + m_dwMaxDisplayable)
	{
		++i;
	}

	m_dwMinDisplayed = i;

	for (; i < (int)m_vContents.size() && dwNumRendered < m_dwMaxDisplayable; ++i)
	{
		glPushMatrix();
		glScalef(fScaleX, fScaleY, 0.0f);
		pObject = m_vContents.at(i);
		
		if (pObject)
		{
			pObject->Render();
			dwNumRendered++;

			if (i == m_dwSelected)
			{
				glColor3f(0.2f, 1.0f, 0.2f);
				glBindTexture(GL_TEXTURE_2D, dwCircleTexture);
				glCallList(Effect::m_dwTexturedQuadDisplayList);
			}

			glPopMatrix();
			glTranslatef(fScaleX, 0.0f, 0.0f);
		}
		else
		{
			glPopMatrix();
		}
	}

	glPopMatrix();

	if (m_bRenderWithBlending)
	{
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
	}
}

void UIScroller::SelectNext()
{
	if (m_vContents.size() <= 0)
	{ //nothing to select!
		return;
	}

	//we are definitely scrolling somewhere so set time left for the scroll
	m_fTimeLeftToScroll	= m_fTimePerScroll;

	if (m_dwSelected == -1)
	{
		m_dwSelected		= 0;
		return;
	}

	++m_dwSelected;
	if (m_dwSelected >= (int)m_vContents.size())
	{
		if (m_bCircular)
		{
			m_dwSelected = 0;
		}
		else
		{
			m_dwSelected = m_vContents.size() - 1;
		}
	}
}

void UIScroller::SelectPrev()
{
	if (m_vContents.size() <= 0)
	{ //nothing to select!
		return;
	}

	//we are definitely scrolling somewhere so set time left for the scroll
	m_fTimeLeftToScroll	= m_fTimePerScroll;

	if (m_dwSelected == -1)
	{
		m_dwSelected		= 0;
		return;
	}

	--m_dwSelected;
	if (m_dwSelected < 0)
	{
		if (m_bCircular)
		{
			m_dwSelected = m_vContents.size() - 1;
		}
		else
		{
			m_dwSelected = 0;
		}
	}
}

Block *UIScroller::ToBlock()
{
	return 0;
}