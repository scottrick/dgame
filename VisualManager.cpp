#include "VisualManager.h"

#include "File.h"
#include "GeometryData.h"
#include "GeometryDesc.h"
#include "Matrix44.h"
#include "TextVisual.h"
#include "Vertex.h"
#include "Visual.h"

#include <fstream>
#include <GL/glut.h>
using namespace std;

//Fonts taken from c:\\windows\\fonts\\cour.ttf
//www.asciitable.com

VisualManager *gVisualManager()
{
	static VisualManager theVisualManager;
	return &theVisualManager;
}

VisualManager::VisualManager()
{
	Init();
}

VisualManager::~VisualManager()
{
	Cleanup();
}

void VisualManager::AddVisual(Visual *pAdd)
{
	if (pAdd)
	{	
		pAdd->AddRef();
		m_vVisuals.push_back(pAdd);
	}
}

void VisualManager::Cleanup()
{
	ClearVisuals();
}

void VisualManager::ClearVisuals()
{
	vector<Visual *>::iterator		iter;
	Visual							*pVisual = 0;

	for (iter = m_vVisuals.begin(); iter != m_vVisuals.end(); iter++)
	{
		pVisual = *iter;
		pVisual->Release();
	}

	m_vVisuals.clear();
}

void VisualManager::DebugInfoRender(const float &fRatio, const PosQuat &pq)
{
	Visual *pCurrent = 0;

	//render currently selected visual for debug display
	if (m_dwSelectedIndex < m_vVisuals.size())
	{
		pCurrent = m_vVisuals.at(m_dwSelectedIndex);
	}

	if (!pCurrent)
	{
		return;
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glPushMatrix();

	glTranslatef(m_PosQuat.pos.x, m_PosQuat.pos.y, m_PosQuat.pos.z - 1.0f);
	glMultMatrixf(pq.quat.ToMatrix().m);
	glMultMatrixf(m_PosQuat.quat.ToMatrix().m);

	pCurrent->Render();
	glPopMatrix();

	GLboolean bTextureWasOn = glIsEnabled(GL_TEXTURE_2D);
	GLboolean bLightingWasOn = glIsEnabled(GL_LIGHTING);
	
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D); //disable textures when rendering the text

	//reset matrices so text is always in the correct spot
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, fRatio, 0.1, 50.0); //update this so we dont get crappy distortion
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0, 0.0, 1.75, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	size_t size = 50 + strlen(pCurrent->GetName());
	char *pDetails = (char *)malloc(sizeof(char) * size);
	memset(pDetails, 0, size);
	sprintf_s(pDetails, size, "%d Vertices, %d Faces, %s", pCurrent->GetVertCount(), pCurrent->GetFaceCount(), pCurrent->GetName());

	glRasterPos2f(-fRatio, 0.97f);
	for (const char *c = pDetails; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
	}

	free(pDetails);

	this->RenderHelpText(fRatio);

	if (bTextureWasOn)
	{
		glEnable(GL_TEXTURE_2D);
	}
	if (bLightingWasOn)
	{
		glEnable(GL_LIGHTING);
	}
}

void VisualManager::DebugScreenRender()
{

}

/*
	Returns the visual, NULL if it isn't found
*/
Visual *VisualManager::GetVisualByFileName(const char *pFileName)
{
	if (!pFileName)
	{
		return 0; //bad filename, return not found
	}

	vector<Visual *>::iterator iter;

	//First check based on full file path and name
	for (iter = m_vVisuals.begin(); iter != m_vVisuals.end(); iter++)
	{
		if (strcmp(pFileName, (*iter)->GetName()) == 0)
		{
			return *iter;
		}
	}

	//if nothing matches based on the full path, then search by just the file name
	for (iter = m_vVisuals.begin(); iter != m_vVisuals.end(); iter++)
	{
		if (strcmp(pFileName, (*iter)->GetShortName()) == 0)
		{
			return *iter;
		}
	}

	return 0; //not found!
}

void VisualManager::Init()
{	
	m_bAutoRotate		= true;
	m_bVisualsLoaded	= false;
}

void VisualManager::Load3DS(const char *pFileName, bool bFontDirectory)
{
	Visual *pNewVisual = 0;

	if (bFontDirectory)
	{
		pNewVisual = new TextVisual();
	}
	else
	{
		pNewVisual = new Visual();
	}

	GLboolean result = pNewVisual->LoadFrom3DS(pFileName);

	if (result)
	{
		AddVisual(pNewVisual);
	}
	
	pNewVisual->Release();
}

void VisualManager::LoadAllVisuals(File *pDirectory, bool bRecursive, bool bFontDirectory)
{
	if (pDirectory && pDirectory->IsDirectory())
	{
		File *pFile = 0;
		for (unsigned int i = 0; i < pDirectory->GetNumChildren(); ++i)
		{
			pFile = pDirectory->GetChild(i);
			if (pFile)
			{
				if (pFile->IsDirectory() && bRecursive)
				{
					//recursive is set to true, so load this directory as well
					LoadAllVisuals(pFile, bRecursive, bFontDirectory);
				}
				else if (!pFile->IsDirectory())
				{	//if its not a directory, check to see if the extension is TGA.  If so, load the texture!
					if ((strcmp(pFile->GetExtension().c_str(), "3ds") == 0) || (strcmp(pFile->GetExtension().c_str(), "3DS") == 0))
					{
						Load3DS(pFile->GetName().c_str(), bFontDirectory);
					}
				}
			}
		}
	}	
}

void VisualManager::LoadVisuals()
{
	if (!m_bVisualsLoaded)
	{
		//load all visuals in a_stockermans directory (created by Adam Stockermans - adamstockermans@hotmail.com)
		File *pAdamDirectory = new File("data\\visuals\\a_stockermans");
		pAdamDirectory->EnumerateContents(false);
		LoadAllVisuals(pAdamDirectory, true, false);
		pAdamDirectory->Release();

		//load all standard visuals
		File *pVisualDirectory = new File("data\\visuals");
		pVisualDirectory->EnumerateContents(false);
		LoadAllVisuals(pVisualDirectory, true, false);
		pVisualDirectory->Release();

		//load all font visuals (where we have to reverse the normals...)
		File *pFontDirectory = new File("data\\visuals\\font");
		pFontDirectory->EnumerateContents(false);
		LoadAllVisuals(pFontDirectory, true, true);
		pFontDirectory->Release();

		m_bVisualsLoaded = true;
	}
}

void VisualManager::Refresh(const float &fDeltaTime)
{
	if (m_bAutoRotate)
	{
		//update the rotation for debug visual display
		Quat *quat = &(m_PosQuat.quat);
		Quat rotation;
		VECTOR3 axis; 	axis.x = 0.1f;	axis.y = 1.0f;	axis.z = -0.2f;
		rotation.CreateFromRotationRADIANS(fDeltaTime * 0.75f * PI, axis.x, axis.y, axis.z);
		*quat = rotation * *quat;
	}
}
	
void VisualManager::SelectNext()
{
	if ((m_dwSelectedIndex + 1) < m_vVisuals.size())
	{
		m_dwSelectedIndex++;
	}
}

void VisualManager::SelectPrevious()
{
	if (m_dwSelectedIndex != 0)
	{
		m_dwSelectedIndex--;
	}
}

void VisualManager::SetAutoRotate(bool bNewRotate)
{
	m_bAutoRotate = bNewRotate;
}

void VisualManager::SetPosQuat(PosQuat pq)
{
	m_PosQuat = pq;
}

void VisualManager::ToggleAutoRotate()
{
	m_bAutoRotate = !m_bAutoRotate;
}