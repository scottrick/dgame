#include "FragmentShader.h"

FragmentShader::FragmentShader(const char *pFilename) 
{
	if (pFilename) 
	{
		LoadShader(pFilename);
	}
}

FragmentShader::~FragmentShader()
{

}

GLuint FragmentShader::CreateShader()
{
	return glCreateShader(GL_FRAGMENT_SHADER);
}

void FragmentShader::Print(int indent) const
{
	printf("%*sFragmentShader 0x%p\n", indent, " ", this);
}