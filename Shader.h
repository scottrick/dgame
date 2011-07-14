#ifndef _SHADER_
#define _SHADER_

#include "Unknown.h"

#include <GL/glew.h>
#include <GL/glut.h>

#include <string>
using namespace std;

class Shader : public Unknown {
public:
	Shader();

	const char			*GetName() const		{ return m_sName.c_str(); }
	int					GetShader() const		{ return m_dwShader; }
	const char			*GetShortName() const;


protected:
	virtual				~Shader();

	virtual GLuint		CreateShader() = 0;
	void				LoadShader(const char *pFilename);

	int					m_dwShader;
	string				m_sName;

private:
	

};

#endif