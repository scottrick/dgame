#include "BackgroundNode.h"

#include "Matrix44.h"

#include <GL/glew.h>
#include <GL/glut.h>

BackgroundNode::BackgroundNode()
{
	Init();
}

BackgroundNode::~BackgroundNode()
{

}

void BackgroundNode::Init()
{

}

void BackgroundNode::Render()
{ //background nodes have a special render, so override base class
	if (m_dwNodeFlags & NODE_DO_NOT_RENDER)
	{	
		return; 
	}

	glLightModelf(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);

	//printf("background pos (%f, %f, %f)\n", m_PosQuat.pos.x, m_PosQuat.pos.y, m_PosQuat.pos.z);
	//need to render twice; once with background in actual position, and then again with background above

	{
		glPushMatrix();
		glTranslatef(m_PosQuat.pos.x, m_PosQuat.pos.y, m_PosQuat.pos.z);
		glMultMatrixf(m_PosQuat.quat.ToMatrix().m);
		glScalef(m_vScale.x, m_vScale.y, m_vScale.z);

		//draw visual
		if (m_pVisual)
		{ 
			m_pVisual->Render();
		}
		glPopMatrix();
	}

	{
		glPushMatrix();
		glTranslatef(m_PosQuat.pos.x, m_PosQuat.pos.y + m_vScale.y, m_PosQuat.pos.z);
		glMultMatrixf(m_PosQuat.quat.ToMatrix().m);
		glScalef(m_vScale.x, m_vScale.y, m_vScale.z);

		//draw visual
		if (m_pVisual)
		{ 
			m_pVisual->Render();
		}
		glPopMatrix();
	}

	if (abs(m_PosQuat.pos.y) > m_vScale.y)
	{
		m_PosQuat.pos.y += m_vScale.y;
	}	

	glLightModelf(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
}