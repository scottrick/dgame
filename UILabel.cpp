#include "UILabel.h"

#include "Texture.h"

#include <iostream>
#include <GL/glut.h>
using namespace std;

UILabel::UILabel()
{
	Init();
}

UILabel::UILabel(int dwTexture)
{
	Init();
	SetLabelTexture(dwTexture);
}

UILabel::~UILabel()
{

}

void UILabel::FromBlock(Block *pBlock)
{

}

void UILabel::Init()
{
	m_dwLabelTextureID			= -1; //indicates no texutre set
}

void UILabel::Print(int indent) const
{
	printf("%*sUILabel 0x%p\n", indent, " ", this);
}

void UILabel::Refresh(const float &fDeltaTime)
{

}

void UILabel::Render()
{
	if (m_bHasFocus)
	{
		glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	}
	else
	{
		glColor4f(0.2f, 0.2f, 0.3f, 1.0f);
	}

	GLboolean bLightingWasOn = glIsEnabled(GL_LIGHTING);
	if (bLightingWasOn)
	{ //turn off lighting
		glDisable(GL_LIGHTING);
	}

	float fBorderSize = 0.05f;

	//draw border
	glBegin(GL_QUADS);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(-1.0f + fBorderSize, -1.0f, 0.0f);
	glVertex3f(-1.0f + fBorderSize, 1.0f, 0.0f);
	
	glVertex3f(-1.0f, -1.0f + fBorderSize, 0.0f);
	glVertex3f(-1.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f + fBorderSize, 0.0f);

	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f - fBorderSize, 1.0f, 0.0f);
	glVertex3f(1.0f - fBorderSize, -1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 0.0f);

	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f - fBorderSize, 0.0f);
	glVertex3f(1.0f, 1.0f - fBorderSize, 0.0f);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	GLboolean bTextureWasOn = glIsEnabled(GL_TEXTURE_2D);
	if (!bTextureWasOn)
	{ //turn textures on if it wasn't already on
		glEnable(GL_TEXTURE_2D);
	}

	if (m_dwLabelTextureID < 0)
	{ //no texture to render, so just stop here
		return;
	}

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, m_dwLabelTextureID);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(-1.0f, -1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(-1.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(1.0f, -1.0f, 0.0f);
	glEnd();

	if (!bTextureWasOn)
	{ //disable textures if it wasn't on initially
		glDisable(GL_TEXTURE_2D);
	}

	if (bLightingWasOn)
	{ //if it was on, turn it back on!
		glEnable(GL_LIGHTING);
	}
}

void UILabel::SetLabelTexture(int dwTexture)
{
	m_dwLabelTextureID = dwTexture;
}	

Block *UILabel::ToBlock()
{
	return 0;
}