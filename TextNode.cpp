#include "TextNode.h"

#include "Matrix44.h"
#include "TextVisual.h"
#include "VisualManager.h"

#include <GL/glut.h>
using namespace std;

TextNode::TextNode(const char *pText)
{	
	m_pText			= pText;
	m_vTextColor	= VECTOR3(1.0f, 1.0f, 1.0f);
	Init();
}

TextNode::TextNode(const char *pText, VECTOR3 vColor)
{
	m_pText			= pText;
	m_vTextColor	= vColor;
	Init();
}

TextNode::~TextNode()
{
	DestroyDisplayList();
}	

void TextNode::CreateDisplayList()
{
	if (m_dwDisplayList != -1)
	{ //display list already exists!
		return;
	}

	m_dwDisplayList = glGenLists(1);
	glNewList(m_dwDisplayList, GL_COMPILE);
	RenderSlow();
	glEndList();
}

void TextNode::DestroyDisplayList()
{
	if (m_dwDisplayList == -1)
	{ //no display list to destroy!
		return;
	}

	glDeleteLists(m_dwDisplayList, 1);
	m_dwDisplayList = -1;
}

void TextNode::Init()
{
	m_dwDisplayList		= -1;
	m_fTextWidth		= 0.0f;
	CreateDisplayList();
}

void TextNode::Print(int indent) const
{
	printf("%*sTextNode 0x%p\n", indent, " ", this);
	m_PosQuat.Print(indent + 2);
}

void TextNode::Render()
{
	glPushMatrix();
	
	//translate to the correct position
	glTranslatef(m_PosQuat.pos.x, m_PosQuat.pos.y, m_PosQuat.pos.z);

	//rotate to correct orientation
	glMultMatrixf(m_PosQuat.quat.ToMatrix().m);
	
	//scale to correct size
	glScalef(m_vScale.x, m_vScale.y, m_vScale.z);
	
	if (m_dwDisplayList == -1)
	{	
		RenderSlow();
	}
	else 
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		glCallList(m_dwDisplayList);
	}

	glPopMatrix();
}

void TextNode::RenderSlow()
{
	//RENDER ALL THE CHARACTERS in a ROW!
	//if a character doesn't have a 3ds model, then just draw a question mark
	size_t		BUFFER_SIZE				= 8; //buffer large enough for  XXX.3ds  string
	const char	*pCurrent				= m_pText; //pointer to current character we are drawing
	char		*pBuffer				= (char *) malloc(sizeof(char) * BUFFER_SIZE);  
	Visual		*pCurrentVisual			= 0;
	m_fTextWidth						= 0.0f;

	GLfloat mat_ambient[]			= { m_vTextColor.x, m_vTextColor.y, m_vTextColor.z, 1.0f };
	GLfloat mat_diffuse[]			= { m_vTextColor.x, m_vTextColor.y, m_vTextColor.z, 1.0f };
	GLfloat mat_specular[]			= { m_vTextColor.x, m_vTextColor.y, m_vTextColor.z, 1.0f };
	GLfloat mat_shininess[]			= { 1.0f };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

	while (*pCurrent != '\0')
	{
		if (*pCurrent == ' ')
		{ //handle special case for the SPACE character
			glTranslatef(0.3f, 0.0f, 0.0f); //just translate over a little bit
			m_fTextWidth += 0.3f;
		}
		else
		{
			memset(pBuffer, 0, BUFFER_SIZE);
			sprintf_s(pBuffer, BUFFER_SIZE, "%d.3ds", *pCurrent);

			pCurrentVisual = gVisualManager()->GetVisualByFileName(pBuffer);
			if (!pCurrentVisual) //did not find a 3ds model for this character; draw ?
			{
				pCurrentVisual = gVisualManager()->GetVisualByFileName("63.3ds");
			}

			if (pCurrentVisual)
			{
				pCurrentVisual->Render();
				
				glTranslatef((pCurrentVisual->GetMaxExtents().x - pCurrentVisual->GetMinExtents().x) + 0.025f, 0.0f, 0.0f);


				if (pCurrent != m_pText)
				{
					m_fTextWidth += 0.025f;
				}
				m_fTextWidth += (pCurrentVisual->GetMaxExtents().x - pCurrentVisual->GetMinExtents().x);

				//printf("width=%f\n", pCurrentVisual->GetMaxExtents().x - pCurrentVisual->GetMinExtents().x);
			}
		}
		pCurrent++;
	}

	if (pBuffer)
	{
		free(pBuffer);
	}
}