#ifndef _FRAGMENT_SHADER_
#define _FRAGMENT_SHADER_

#include "Shader.h"

class FragmentShader : public Shader
{
public:
	FragmentShader(const char *pFilename);

	const char *GetClassName() const		{ return "FragmentShader"; }
	void Print(int indent) const;

protected:
	virtual ~FragmentShader();

	virtual GLuint CreateShader();

private:

};

#endif