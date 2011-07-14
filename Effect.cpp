#include "Effect.h"

#include <GL/glut.h>

int Effect::m_dwTexturedQuadDisplayList = -1;

Effect::Effect()
{
	Init();
}

Effect::~Effect()
{
	
}

void Effect::Clone(Effect *pClone)
{
	Object::Clone(pClone);
}

int Effect::GenerateTexturedQuadDisplayList()
{
	int dwDisplayList = glGenLists(1);
	glNewList(dwDisplayList, GL_COMPILE);
	
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.0f);
	glEnd();

	glEndList();

	return dwDisplayList;
}

void Effect::Init()
{
	SetObjectType(Object::eTYPE_EFFECT);

	//init the quad texture display list
	if (m_dwTexturedQuadDisplayList == -1)
	{
		m_dwTexturedQuadDisplayList = GenerateTexturedQuadDisplayList();
	}
}