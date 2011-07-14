#include "TestNode.h"

#include "Matrix44.h"

#include "GL/glut.h"

TestNode::TestNode()
{
	Init();
}	

TestNode::~TestNode()
{
	
}	

void TestNode::Init()
{
	//nothing to initialize !!!
}

void TestNode::Render()
{
	if (m_dwNodeFlags & NODE_DO_NOT_RENDER)
	{		
		return;
	}

	glPushMatrix();

	GLboolean bLightingWasOn = glIsEnabled(GL_LIGHTING);
	if (bLightingWasOn)
	{
		glDisable(GL_LIGHTING);
	}

	//translate to the correct position
	glTranslatef(m_PosQuat.pos.x, m_PosQuat.pos.y, m_PosQuat.pos.z);
	//rotate to correct orientation
	glMultMatrixf(m_PosQuat.quat.ToMatrix().m);
	//scale to correct size
	glScalef(m_vScale.x, m_vScale.y, m_vScale.z);

	//render test thing
	
	//DRAW X ARROW!
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.85f, 0.15f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.85f, -0.15f, 0.0f);

	//DRAW Y ARROW
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.15f, 0.85f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.15f, 0.85f, 0.0f);

	//DRAW Z ARROW
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.15f, 0.0f, 0.85f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.15f, 0.0f, 0.85f);

	glEnd();

	if (bLightingWasOn)
	{
		glEnable(GL_LIGHTING);
	}

	glPopMatrix();
}	