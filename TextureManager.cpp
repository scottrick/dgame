#include "TextureManager.h"

#include "File.h"
#include "Matrix44.h"
#include "TextNode.h"
#include "Texture.h"

#include <GL/glut.h>

TextureManager *gTextureManager()
{
	static TextureManager theTextureManager;
	return &theTextureManager;
}

TextureManager::TextureManager()
{
	Init();
}

TextureManager::~TextureManager()
{
	Cleanup();
}

void TextureManager::AddTexture(Texture *pAdd)
{
	if (pAdd)
	{	
		pAdd->AddRef();
		m_vTextures.push_back(pAdd);
	}
}

void TextureManager::Cleanup()
{
	ClearTextNode();
	ClearTextures();
}

void TextureManager::ClearTextNode()
{
	if (m_pTextNode)
	{
		m_pTextNode->Release();
		m_pTextNode = 0;
	}
}	

void TextureManager::ClearTextures()
{
	vector<Texture *>::iterator		iter;
	Texture							*pTexture = 0;

	for (iter = m_vTextures.begin(); iter != m_vTextures.end(); iter++)
	{
		pTexture = *iter;
		pTexture->Release();
	}

	m_vTextures.clear();
}

void TextureManager::DebugInfoRender(const float &fRatio, const PosQuat &pq)
{
	Texture *pCurrent = 0;

	//render currently selected texture for debug display
	if (m_dwSelectedIndex < m_vTextures.size())
	{
		pCurrent = m_vTextures.at(m_dwSelectedIndex);
	}

	if (!pCurrent)
	{
		return;
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	GLboolean bLightingWasOn = glIsEnabled(GL_LIGHTING);
	if (bLightingWasOn)
	{ //turn off lighting
		glDisable(GL_LIGHTING);
	}
	
	GLboolean bTextureWasOn = glIsEnabled(GL_TEXTURE_2D);
	if (!bTextureWasOn)
	{ //turn textures on if it wasn't already on
		glEnable(GL_TEXTURE_2D);
	}

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, pCurrent->GetTexture());

	glMultMatrixf(pq.quat.ToMatrix().m);

	float fScale = 0.33f;
	float fBorder = fScale * 1.05f;
	float fBorderSize = fBorder - fScale;

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(-fScale, -fScale, 0.0f);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(-fScale, fScale, 0.0f);
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(fScale, fScale, 0.0f);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(fScale, -fScale, 0.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 1.0f);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-fBorder, -fBorder, 0.0f);
	glVertex3f(-fBorder, fBorder, 0.0f);
	glVertex3f(-fBorder + fBorderSize, fBorder, 0.0f);
	glVertex3f(-fBorder + fBorderSize, -fBorder, 0.0f);

	glVertex3f(fBorder, -fBorder, 0.0f);
	glVertex3f(fBorder, fBorder, 0.0f);
	glVertex3f(fBorder - fBorderSize, fBorder, 0.0f);
	glVertex3f(fBorder - fBorderSize, -fBorder, 0.0f);

	glVertex3f(-fBorder, -fBorder, 0.0f);
	glVertex3f(-fBorder, -fBorder + fBorderSize, 0.0f);
	glVertex3f(fBorder, -fBorder + fBorderSize, 0.0f);
	glVertex3f(fBorder, -fBorder, 0.0f);

	glVertex3f(-fBorder, fBorder, 0.0f);
	glVertex3f(-fBorder, fBorder - fBorderSize, 0.0f);
	glVertex3f(fBorder, fBorder - fBorderSize, 0.0f);
	glVertex3f(fBorder, fBorder, 0.0f);

	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);

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
	sprintf_s(pDetails, size, "%dx%d, %u bytes, %s", pCurrent->GetSizeX(), pCurrent->GetSizeY(), pCurrent->GetSizeInBytes(), pCurrent->GetName());

	glRasterPos2f(-fRatio, 0.97f);
	for (const char *c = pDetails; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
	}
	free(pDetails);

	this->RenderHelpText(fRatio);

	glEnable(GL_TEXTURE_2D);

	if (!bTextureWasOn)
	{ //disable textures if it wasn't on initially
		glDisable(GL_TEXTURE_2D);
	}

	if (bLightingWasOn)
	{ //if it was on, turn it back on!
		glEnable(GL_LIGHTING);
	}
}

void TextureManager::DebugScreenRender()
{
	
}

/*
	Returns the texture number, -1 if it isn't found
*/
int TextureManager::GetTextureByFileName(const char *pFileName)
{
	if (!pFileName)
	{
		return -1; //bad filename, return not found
	}

	vector<Texture *>::iterator iter;

	//First check based on full file path and name
	for (iter = m_vTextures.begin(); iter != m_vTextures.end(); iter++)
	{
		if (strcmp(pFileName, (*iter)->GetName()) == 0)
		{
			return (*iter)->GetTexture();
		}
	}

	//if nothing matches based on the full path, then search by just the file name
	for (iter = m_vTextures.begin(); iter != m_vTextures.end(); iter++)
	{
		if (strcmp(pFileName, (*iter)->GetShortName()) == 0)
		{
			return (*iter)->GetTexture();
		}
	}

	return -1; //not found!
}

void TextureManager::Init()
{	
	m_bTexturesLoaded	= false;
	m_pTextNode			= 0;
}

void TextureManager::LoadAllTGA(File *pDirectory, bool bRecursive)
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
					LoadAllTGA(pFile, bRecursive);
				}
				else if (!pFile->IsDirectory())
				{	//if its not a directory, check to see if the extension is TGA.  If so, load the texture!
					if ((strcmp(pFile->GetExtension().c_str(), "tga") == 0) || (strcmp(pFile->GetExtension().c_str(), "TGA") == 0))
					{
						LoadTexture(pFile->GetName().c_str());
					}
				}
			}
		}
	}	
}

void TextureManager::LoadTexture(const char *pFileName)
{
	Texture *pNewTexture = new Texture();
	GLboolean result = pNewTexture->LoadFromTGA(pFileName);

	if (result)
	{
		AddTexture(pNewTexture);
	}
	
	pNewTexture->Release();
}

void TextureManager::LoadTextures()
{
	if (!m_bTexturesLoaded)
	{
		File *pTextureDirectory = new File("data\\textures");
		pTextureDirectory->EnumerateContents(true);
		//pTextureDirectory->Print(0);

		//load all textures!   (*.TGA) 
		LoadAllTGA(pTextureDirectory, true);

		pTextureDirectory->Release();

		m_bTexturesLoaded = true;
	}
}

void TextureManager::Refresh(const float &fDeltaTime)
{
	
}
	
void TextureManager::SelectNext()
{
	if ((m_dwSelectedIndex + 1) < m_vTextures.size())
	{
		m_dwSelectedIndex++;
		ClearTextNode();
	}
}

void TextureManager::SelectPrevious()
{
	if (m_dwSelectedIndex != 0)
	{
		m_dwSelectedIndex--;
		ClearTextNode();
	}
}