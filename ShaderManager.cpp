#include "ShaderManager.h"

#include "File.h"
#include "ShaderProgram.h"

ShaderManager *gShaderManager()
{
	static ShaderManager theShaderManager;
	return &theShaderManager;
}

ShaderManager::ShaderManager()
{
	Init();
}

ShaderManager::~ShaderManager()
{
	Cleanup();
}

void ShaderManager::AddFragmentShader(FragmentShader *pAdd)
{
	if (pAdd)
	{
		pAdd->AddRef();
		m_vFragmentShaders.push_back(pAdd);
	}
}

void ShaderManager::AddVertexShader(VertexShader *pAdd)
{
	if (pAdd)
	{
		pAdd->AddRef();
		m_vVertexShaders.push_back(pAdd);
	}
}

void ShaderManager::Cleanup()
{
	ClearShaders();
}

void ShaderManager::ClearShaders()
{
	{
		vector<FragmentShader *>::iterator		iter;
		FragmentShader							*pShader = 0;

		for (iter = m_vFragmentShaders.begin(); iter != m_vFragmentShaders.end(); iter++)
		{
			pShader = *iter;
			pShader->Release();
		}

		m_vFragmentShaders.clear();
	}

	{
		vector<VertexShader *>::iterator		iter;
		VertexShader							*pShader = 0;

		for (iter = m_vVertexShaders.begin(); iter != m_vVertexShaders.end(); iter++)
		{
			pShader = *iter;
			pShader->Release();
		}

		m_vVertexShaders.clear();
	}
}

ShaderProgram *ShaderManager::CreateProgram(const char *pFrag, const char *pVert)
{
	FragmentShader	*pFragment	= GetFragmentShaderByFileName(pFrag);
	VertexShader	*pVertex	= GetVertexShaderByFileName(pVert);

	if (pFragment && pVertex)
	{
		ShaderProgram *pNewProgram = new ShaderProgram(pFragment, pVertex);
		return pNewProgram;
	}

	return 0;
}

void ShaderManager::DebugInfoRender(const float &fRatio, const PosQuat &pq)
{

}

void ShaderManager::DebugScreenRender()
{
	
}

FragmentShader *ShaderManager::GetFragmentShaderByFileName(const char *pFileName)
{
	if (!pFileName)
	{
		return 0;
	}

	vector<FragmentShader *>::iterator iter = m_vFragmentShaders.begin();

	while (iter != m_vFragmentShaders.end())
	{
		if (strcmp((*iter)->GetName(), pFileName) == 0)
		{
			return *iter;
		}
		
		iter++;
	}

	iter = m_vFragmentShaders.begin();

	while (iter != m_vFragmentShaders.end())
	{
		if (strcmp((*iter)->GetShortName(), pFileName) == 0)
		{
			return *iter;
		}

		iter++;
	}

	return 0; //Not found!
}

VertexShader *ShaderManager::GetVertexShaderByFileName(const char *pFileName)
{
	if (!pFileName)
	{
		return 0;
	}

	vector<VertexShader *>::iterator iter = m_vVertexShaders.begin();

	while (iter != m_vVertexShaders.end())
	{
		if (strcmp((*iter)->GetName(), pFileName) == 0)
		{
			return *iter;
		}
		
		iter++;
	}

	iter = m_vVertexShaders.begin();

	while (iter != m_vVertexShaders.end())
	{
		if (strcmp((*iter)->GetShortName(), pFileName) == 0)
		{
			return *iter;
		}

		iter++;
	}

	return 0; //Not found!
}

void ShaderManager::Init()
{	
	m_bShadersLoaded	= false;
	m_bShadersSupported	= true;

	m_pProgramNormals	= 0;
	m_pProgramRedBump	= 0;
	m_pProgramToon		= 0;
	m_pProgramWave		= 0;
}

void ShaderManager::LoadAllShaders(File *pDirectory, bool bRecursive)
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
					LoadAllShaders(pFile, bRecursive);
				}
				else if (!pFile->IsDirectory())
				{	//if its not a directory, check to see if the extension is TGA.  If so, load the texture!
					if ((strcmp(pFile->GetExtension().c_str(), "frag") == 0) || (strcmp(pFile->GetExtension().c_str(), "FRAG") == 0))
					{
						LoadFragmentShader(pFile->GetName().c_str());
					}
					else if ((strcmp(pFile->GetExtension().c_str(), "vert") == 0) || (strcmp(pFile->GetExtension().c_str(), "VERT") == 0))
					{
						LoadVertexShader(pFile->GetName().c_str());
					}
				}
			}
		}
	}	
}

void ShaderManager::LoadFragmentShader(const char *pFileName)
{
	FragmentShader *pNewShader	= new FragmentShader(pFileName);

	if (pNewShader->GetShader() > -1)
	{
		AddFragmentShader(pNewShader);
	}

	pNewShader->Release();
}

void ShaderManager::LoadVertexShader(const char *pFileName)
{
	VertexShader *pNewShader	= new VertexShader(pFileName);

	if (pNewShader->GetShader() > -1)
	{
		AddVertexShader(pNewShader);
	}

	pNewShader->Release();
}

void ShaderManager::LoadShaders()
{
	if (!GLEW_VERSION_2_0)
	{ //shaders not supported, so don't load them...
		 m_bShadersSupported = false;
		 return;
	}

	if (!m_bShadersLoaded)
	{
		GLint varLoc;

		File *pShaderDirectory = new File("data\\shaders");
		pShaderDirectory->EnumerateContents(true);

		LoadAllShaders(pShaderDirectory, true);
		pShaderDirectory->Release();
		m_bShadersLoaded = true;


		FragmentShader	*pNewFrag	= GetFragmentShaderByFileName("red_bumpy.frag");
		VertexShader	*pNewVert	= GetVertexShaderByFileName("red_bumpy.vert");
		if (pNewFrag && pNewVert) 
		{
			m_pProgramRedBump		= new ShaderProgram(pNewFrag, pNewVert);
		}

		pNewFrag					= GetFragmentShaderByFileName("normals.frag");
		pNewVert					= GetVertexShaderByFileName("normals.vert");
		if (pNewFrag && pNewVert)
		{
			m_pProgramNormals		= new ShaderProgram(pNewFrag, pNewVert);
		}

		pNewFrag					= GetFragmentShaderByFileName("wave.frag");
		pNewVert					= GetVertexShaderByFileName("wave.vert");
		if (pNewFrag && pNewVert)
		{
			m_pProgramWave			= new ShaderProgram(pNewFrag, pNewVert);
		}

		if (m_pProgramWave)
		{
			m_pProgramWave->Use();
			varLoc				= glGetUniformLocation(m_pProgramWave->GetProgram(), "sceneTexture");
			glUniform1i(varLoc, 0);
			varLoc				= glGetUniformLocation(m_pProgramWave->GetProgram(), "waveTexture");
			glUniform1i(varLoc, 1);
		}

		pNewFrag					= GetFragmentShaderByFileName("toon.frag");
		pNewVert					= GetVertexShaderByFileName("default.vert");
		if (pNewFrag && pNewVert)
		{
			m_pProgramToon			= new ShaderProgram(pNewFrag, pNewVert);
		}
	}

	gShaderManager()->UseDefaultProgram();
}

void ShaderManager::Refresh(const float &fDeltaTime)
{
	
}
	
void ShaderManager::SelectNext()
{

}

void ShaderManager::SelectPrevious()
{

}

void ShaderManager::UseDefaultProgram()
{
	if (this->IsShadersSupported())
	{
		glUseProgram(0);
	}
}