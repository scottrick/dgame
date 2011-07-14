#include "VertexShader.h"

VertexShader::VertexShader(const char *pFilename) 
{
	if (pFilename) 
	{
		LoadShader(pFilename);
	}
}

VertexShader::~VertexShader()
{

}

GLuint VertexShader::CreateShader()
{
	return glCreateShader(GL_VERTEX_SHADER);
}

void VertexShader::Print(int indent) const
{
	printf("%*sVertexShader 0x%p\n", indent, " ", this);
}