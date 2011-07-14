#include "Node.h"

#include "Matrix44.h"
#include "VisualManager.h"

#include <iostream>
#include <GL/glut.h>
using namespace std;

Node::Node()
{
	Init();	
}

Node::~Node()
{
	if (m_pVisual)
	{
		m_pVisual->Release();
	}
}

void Node::Init()
{
	m_vScale.x = 1.0f;  m_vScale.y = 1.0f;  m_vScale.z = 1.0f;
	m_dwNodeFlags	= 0;
	m_pVisual		= 0;
	m_dwDebugX		= 0;
	m_dwDebugY		= 0;
	m_dwBucket		= 0;
}

void Node::Print(int indent) const
{
	printf("%*sNode 0x%p\n", indent, " ", this);
	m_PosQuat.Print(indent + 2);
}

void Node::RaiseNodeFlags(unsigned int dwFlags)
{
	m_dwNodeFlags |= dwFlags;
}

void Node::RemoveNodeFlags(unsigned int dwFlags)
{
	dwFlags = ~dwFlags;
	m_dwNodeFlags &= dwFlags;
}

void Node::Render()
{
	if (m_dwNodeFlags & NODE_DO_NOT_RENDER)
	{	
		return; 
	}

	if (m_pVisual)
	{
		glPushMatrix();
		
		//translate to the correct position
		glTranslatef(m_PosQuat.pos.x, m_PosQuat.pos.y, m_PosQuat.pos.z);

		//rotate to correct orientation
		glMultMatrixf(m_PosQuat.quat.ToMatrix().m);

		if (NODE_RENDER_UPSIDEDOWN & m_dwNodeFlags)
		{
			glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
		}
		
		//scale to correct size
		glScalef(m_vScale.x, m_vScale.y, m_vScale.z);

		//draw the visual
		if (m_pVisual)
		{
			m_pVisual->Render();
		}

		glPopMatrix();
	}

	//draw bounding box?
	if (m_pVisual && (m_dwNodeFlags & NODE_RENDER_COLLISION_DEBUG))
	{
		glPushMatrix();
		
		//translate to the correct position
		glTranslatef(m_PosQuat.pos.x, m_PosQuat.pos.y, m_PosQuat.pos.z);

		//scale to correct size
		glScalef(m_vScale.x, m_vScale.y, m_vScale.z);

		GLboolean bTextureWasOn = glIsEnabled(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_2D);
		GLboolean bLightingWasOn = glIsEnabled(GL_LIGHTING);
		glDisable(GL_LIGHTING);
		
		glColor3f(0.0f, 1.0f, 0.0f);
		float			fRadians		= 0.0f;
		float			fX				= 0.0f;
		float			fY				= 0.0f;
		unsigned int	dwSegments		= 12;

		glBegin(GL_LINE_STRIP);
		for (unsigned int i = 0; i <= 16; ++i)
		{
			fRadians	= 2.0f * PI / (float)dwSegments * (float)i;
			fX			= cos(fRadians) * 0.5f;
			fY			= sin(fRadians) * 0.5f;
			glVertex3f(fX, fY, 0.0f);
		}
		glEnd();

		/*
		size_t size = 50;
		char *pDetails = (char *)malloc(sizeof(char) * size);
		memset(pDetails, 0, size);
		sprintf_s(pDetails, size, "--- %d:%d:%d", m_dwDebugX, m_dwDebugY, m_dwBucket);

		glRasterPos2f(0.0f, 0.0f);
		for (const char *c = pDetails; *c != '\0'; c++)
		{
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
		}

		free(pDetails);
		*/

		if (bTextureWasOn)
		{
			glEnable(GL_TEXTURE_2D);
		}
		if (bLightingWasOn)
		{
			glEnable(GL_LIGHTING);
		}
		glPopMatrix();
	}

	/*
	if (pVisual)
	{
		pVisual->Render();
		//pVisual->RenderSlow();

		GLboolean bTextureWasOn = glIsEnabled(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_2D);
		GLboolean bLightingWasOn = glIsEnabled(GL_LIGHTING);
		glDisable(GL_LIGHTING);
		glColor3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_LINE_STRIP);
		VECTOR3 vBounds = pVisual->GetBoundaries();
		glVertex3f(-vBounds.x, vBounds.y, 0.0f);
		glVertex3f(vBounds.x, vBounds.y, 0.0f);
		glVertex3f(vBounds.x, -vBounds.y, 0.0f);
		glVertex3f(-vBounds.x, -vBounds.y, 0.0f);
		glVertex3f(-vBounds.x, vBounds.y, 0.0f);
		glEnd();
		if (bTextureWasOn)
		{
			glEnable(GL_TEXTURE_2D);
		}
		if (bLightingWasOn)
		{
			glEnable(GL_LIGHTING);
		}
	}
	*/

	//glutWireCube(1.0);
	//glutWireTeapot(1.0);
	//glutSolidTeapot(1.0);
	//glutWireDodecahedron();
	
	//render any children here?
}

void Node::SetScale(VECTOR3 vScale)
{
	m_vScale = vScale;
}

bool Node::SetVisual(const char *pFile)
{
	bool bResult			= false;

	if (pFile)
	{
		Visual *pVisual = gVisualManager()->GetVisualByFileName(pFile);
		if (pVisual)
		{
			SetVisual(pVisual);
			bResult = true;
		}
	}

	return bResult;
}
	
void Node::SetVisual(Visual *pVisual)
{
	if (m_pVisual != pVisual)
	{
		if (m_pVisual)
		{
			m_pVisual->Release();
			m_pVisual = 0;
		}

		if (pVisual)
		{
			m_pVisual = pVisual;
			m_pVisual->AddRef();
		}
	}
}