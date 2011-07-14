#include "TestTextureNode.h"

#include "Matrix44.h"
#include "TextureManager.h"

#include "GL/glut.h"


TestTextureNode::TestTextureNode()
{
	Init();
}

TestTextureNode::~TestTextureNode()
{

}

void TestTextureNode::Init()
{	
	m_dwTexture = gTextureManager()->GetTextureByFileName("data\\textures\\Planets\\neptunemap.tga");
	//m_dwTexture = gTextureManager()->GetTextureByFileName("data\\textures\\craig.tga");
}

void TestTextureNode::Render()
{
	if (m_dwNodeFlags & NODE_DO_NOT_RENDER)
	{		
		return;
	}

	GLboolean bLightingWasOn = glIsEnabled(GL_LIGHTING);
	GLboolean bTextureWasOn = glIsEnabled(GL_TEXTURE_2D);

	if (bLightingWasOn)
	{
		glDisable(GL_LIGHTING);
	}
	if (!bTextureWasOn)
	{
		glEnable(GL_TEXTURE_2D);
	}

	glPushMatrix();
	
	//translate to the correct position
	glTranslatef(m_PosQuat.pos.x, m_PosQuat.pos.y, m_PosQuat.pos.z);
	//rotate to correct orientation
	glMultMatrixf(m_PosQuat.quat.ToMatrix().m);
	//scale to correct size
	glScalef(m_vScale.x, m_vScale.y, m_vScale.z);
	
	if (m_dwTexture >= 0)
	{
		glBindTexture(GL_TEXTURE_2D, m_dwTexture);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}

	glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	
	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, 1.0f);			glVertex3f(-0.5f, 1.0f, 0.5f);
	glTexCoord2f(0.0f, 0.5f);			glVertex3f(-1.0f, 0.0f, 1.0f);
	glTexCoord2f(0.25f, 0.5f);			glVertex3f(1.0f, 0.0f, 1.0f);
	glTexCoord2f(0.25f, 1.0f);			glVertex3f(0.5f, 1.0f, 0.5f);
	
	glTexCoord2f(0.25f, 1.0f);			glVertex3f(0.5f, 1.0f, 0.5f);
	glTexCoord2f(0.25f, 0.5f);			glVertex3f(1.0f, 0.0f, 1.0f);
	glTexCoord2f(0.5f, 0.5f);			glVertex3f(1.0f, 0.0f, -1.0f);
	glTexCoord2f(0.5f, 1.0f);			glVertex3f(0.5f, 1.0f, -0.5f);

	glTexCoord2f(0.5f, 1.0f);			glVertex3f(0.5f, 1.0f, -0.5f);
	glTexCoord2f(0.5f, 0.5f);			glVertex3f(1.0f, 0.0f, -1.0f);
	glTexCoord2f(0.75f, 0.5f);			glVertex3f(-1.0f, 0.0f, -1.0f);
	glTexCoord2f(0.75f, 1.0f);			glVertex3f(-0.5f, 1.0f, -0.5f);
	
	glTexCoord2f(0.75f, 1.0f);			glVertex3f(-0.5f, 1.0f, -0.5f);
	glTexCoord2f(0.75f, 0.5f);			glVertex3f(-1.0f, 0.0f, -1.0f);
	glTexCoord2f(1.0f, 0.5f);			glVertex3f(-1.0f, 0.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);			glVertex3f(-0.5f, 1.0f, 0.5f);

	//bottom half
	glTexCoord2f(0.0f, 0.5f);			glVertex3f(-1.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);			glVertex3f(-0.5f, -1.0f, 0.5f);
	glTexCoord2f(0.25f, 0.0f);			glVertex3f(0.5f, -1.0f, 0.5f);
	glTexCoord2f(0.25f, 0.5f);			glVertex3f(1.0f, 0.0f, 1.0f);

	glTexCoord2f(0.25f, 0.5f);			glVertex3f(1.0f, 0.0f, 1.0f);
	glTexCoord2f(0.25f, 0.0f);			glVertex3f(0.5f, -1.0f, 0.5f);
	glTexCoord2f(0.5f, 0.0f);			glVertex3f(0.5f, -1.0f, -0.5f);
	glTexCoord2f(0.5f, 0.5f);			glVertex3f(1.0f, 0.0f, -1.0f);

	glTexCoord2f(0.5f, 0.5f);			glVertex3f(1.0f, 0.0f, -1.0f);
	glTexCoord2f(0.5f, 0.0f);			glVertex3f(0.5f, -1.0f, -0.5f);
	glTexCoord2f(0.75f, 0.0f);			glVertex3f(-0.5f, -1.0f, -0.5f);
	glTexCoord2f(0.75f, 0.5f);			glVertex3f(-1.0f, 0.0f, -1.0f);

	glTexCoord2f(0.75f, 0.5f);			glVertex3f(-1.0f, 0.0f, -1.0f);
	glTexCoord2f(0.75f, 0.0f);			glVertex3f(-0.5f, -1.0f, -0.5f);
	glTexCoord2f(1.0f, 0.0f);			glVertex3f(-0.5f, -1.0f, 0.5f);
	glTexCoord2f(1.0f, 0.5f);			glVertex3f(-1.0f, 0.0f, 1.0f);
	
	glEnd();



	//DRAW X ARROW!
	/*
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.85f, 0.15f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.85f, -0.15f, 0.0f);

	glEnd();
	*/

	if (bLightingWasOn)
	{
		glEnable(GL_LIGHTING);
	}
	if (!bTextureWasOn)
	{
		glDisable(GL_TEXTURE_2D);
	}
	
	glPopMatrix();	
}