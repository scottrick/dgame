#include <fstream>
#include <iostream>
using namespace std;

#include "Shader.h"

Shader::Shader() 
{
	m_dwShader	= -1; //-1 means no shader created, etc yet
	m_sName		= "";
}

Shader::~Shader() 
{
	if (m_dwShader > 0) 
	{
		glDeleteShader(m_dwShader);
		m_dwShader = -1;
	}
}

/*
	returns the last part of the file name.  "\shaders\test.vert"  returns  "test.vert"
*/
const char *Shader::GetShortName() const
{	
	const char *pName = GetName();
	const char *pCurrent = pName;

	while (*pCurrent != '\0')
	{
		if (*pCurrent == '\\')
		{
			pName = pCurrent + 1;
		}

		pCurrent++;
	}

	return pName;
}

void Shader::LoadShader(const char *pFilename) 
{
	printf("Loading Shader \"%s\"...  ", pFilename);
	ifstream shaderFile(pFilename, ios::in | ios::binary | ios::ate);
	char	*pMemBlock		= 0;
	int		size			= 0;
	
	if (!shaderFile.is_open())
	{ //abort if the file isn't open
		printf("FAILED\n");
		return;
	}

	m_sName					= pFilename;
	size					= (int) shaderFile.tellg();
	pMemBlock				= new char[size];

	shaderFile.seekg(0, ios::beg);
	shaderFile.read(pMemBlock, size);
	shaderFile.close();

	const GLchar *pSource	= (GLchar *) pMemBlock;
	m_dwShader				= CreateShader();

	glShaderSource(m_dwShader, 1, &pSource, &size);
	delete pMemBlock;

	glCompileShader(m_dwShader);

	//Check to see if it compiled correctly
	GLint length = 0;
	glGetShaderiv(m_dwShader, GL_INFO_LOG_LENGTH, &length);

	GLint compileStatus = 0;
	glGetShaderiv(m_dwShader, GL_COMPILE_STATUS, &compileStatus);

	if (!compileStatus) { //failed to compile shader, so delete it
		printf("\n --> ERROR compiling shader \"%s\"\n", pFilename);
			
		glDeleteShader(m_dwShader);
		m_dwShader	= -1; //set to invalid since it didn't compile
	}

	if (length > 1)
	{ //there is a log entry, so print it off
		if (compileStatus)
		{
			printf("\n");
		}

		printf(" --> INFO LOG \"%s\"\n", pFilename);
			
		GLchar *pLog = new GLchar[length];
		GLsizei actualLength;

		glGetShaderInfoLog(m_dwShader, length, &actualLength, pLog);

		printf("     %s --> END INFO\n", pLog);

		delete pLog;
	}
	else 
	{
		printf("OK\n");
	}
}