#ifndef _SHADER_PROGRAM_
#define _SHADER_PROGRAM_

#pragma comment(lib, "glew32.lib")

#include "FragmentShader.h"
#include "VertexShader.h"

class ShaderProgram : public Unknown
{
public:
	ShaderProgram(const char *pFragmentFile, const char *pVertexFile);
	ShaderProgram(FragmentShader *pFragmentShader, VertexShader *pVertexShader);

	const char		*GetClassName() const			{ return "ShaderProgram"; }
	GLuint			GetProgram() const				{ return m_dwProgram; }
	void			Print(int indent) const;
	void			Use();

protected:
	virtual			~ShaderProgram();

	void			CreateProgram();
	void			DeleteProgram();
	void			Init();
	void			SetFragmentShader(FragmentShader *pShader);
	void			SetVertexShader(VertexShader *pShader);

	GLuint			m_dwProgram;

	FragmentShader	*m_pFragmentShader;
	VertexShader	*m_pVertexShader;

private:


};

#endif