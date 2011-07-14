#ifndef _VERTEX_SHADER_
#define _VERTEX_SHADER_

#include "Shader.h"

class VertexShader : public Shader
{
public:
	VertexShader(const char *pFilename);

	const char *GetClassName() const		{ return "VertexShader"; }
	void Print(int indent) const;

protected:
	virtual ~VertexShader();

	virtual GLuint CreateShader();

private:

};

#endif