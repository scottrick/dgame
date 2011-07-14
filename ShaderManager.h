#ifndef _SHADER_MANAGER_
#define _SHADER_MANAGER_

#include "Manager.h"

#include <vector>
using namespace std;

class File;
class FragmentShader;
class ShaderProgram;
class VertexShader;

class ShaderManager : public Manager
{
public:
	ShaderManager();
	~ShaderManager();

	void								AddFragmentShader(FragmentShader *pAdd);
	void								AddProgram(ShaderProgram *pAdd);
	void								AddVertexShader(VertexShader *pAdd);
	void								Cleanup();
	ShaderProgram						*CreateProgram(const char *pFrag, const char *pVert);
	void								DebugInfoRender(const float &fRatio, const PosQuat &pq);
	void								DebugScreenRender();
	const char							*GetManagerName()							{ return "ShaderManager"; }
	FragmentShader						*GetFragmentShaderByFileName(const char *pFileName);
	ShaderProgram						*GetProgramNormals() const					{ return m_pProgramNormals; }
	ShaderProgram						*GetProgramRedBump() const					{ return m_pProgramRedBump; }
	ShaderProgram						*GetProgramToon() const						{ return m_pProgramToon; }
	ShaderProgram						*GetProgramWave() const						{ return m_pProgramWave; }
	VertexShader						*GetVertexShaderByFileName(const char *pFileName);
	bool								IsShadersSupported() const					{ return m_bShadersSupported; }
	void								LoadFragmentShader(const char *pFileName);
	void								LoadShaders();
	void								LoadVertexShader(const char *pFileName);
	void								Refresh(const float &fDeltaTime);
	void								SelectNext();
	void								SelectPrevious();
	void								UseDefaultProgram();

private:
	void								ClearShaders();
	void								Init();
	void								LoadAllShaders(File *pDirectory, bool bRecursive);

	bool								m_bShadersLoaded;
	bool								m_bShadersSupported;

	ShaderProgram						*m_pProgramNormals;
	ShaderProgram						*m_pProgramRedBump;
	ShaderProgram						*m_pProgramToon;
	ShaderProgram						*m_pProgramWave;

	vector<FragmentShader *>			m_vFragmentShaders;
	vector<VertexShader *>				m_vVertexShaders;
	
};

extern ShaderManager *gShaderManager();

#endif