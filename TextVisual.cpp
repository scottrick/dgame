#include "TextVisual.h"

#include "GeometryDesc.h"

#include <iostream>
#include <GL/glut.h>
using namespace std;

TextVisual::TextVisual()
{
	Init();
}

TextVisual::~TextVisual()
{

}

void TextVisual::Init()
{
	m_bReversedNormals	= true;	
}

void TextVisual::Print(int indent) const
{
	printf("%*sTextVisual 0x%p\n", indent, " ", this);
}

void TextVisual::RenderSlow() //renders without display lists!
{
	glPushMatrix();

	//SCALE SO IT ISNT SO BIG
	//glScalef(m_fSCALE, m_fSCALE, m_fSCALE);

	//TRANSLATE THIS THING SO IT IS CENTERED LAWL
	//glTranslatef(m_vTRANS.x, m_vTRANS.y, m_vTRANS.z);

	vector<GeometryDesc *>::iterator iter;
	GeometryDesc *pDesc = 0;
	for (iter = m_vGeometryDescriptions.begin(); iter != m_vGeometryDescriptions.end(); iter++)
	{
		pDesc = *iter;
		if (pDesc)
		{	
			pDesc->Render();
		}
	}

	glPopMatrix();
}