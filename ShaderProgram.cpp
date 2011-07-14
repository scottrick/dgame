#include "ShaderProgram.h"

#include "ShaderManager.h"

ShaderProgram::ShaderProgram(const char *pFragmentFile, const char *pVertexFile)
{
	Init();		

	FragmentShader	*pFragmentShader	= new FragmentShader(pFragmentFile);
	VertexShader	*pVertexShader		= new VertexShader(pVertexFile);

	SetFragmentShader(pFragmentShader);
	SetVertexShader(pVertexShader);

	pFragmentShader->Release();
	pVertexShader->Release();

	CreateProgram();
}

ShaderProgram::ShaderProgram(FragmentShader *pFragmentShader, VertexShader *pVertexShader)
{
	Init();

	SetFragmentShader(pFragmentShader);
	SetVertexShader(pVertexShader);

	CreateProgram();
}

ShaderProgram::~ShaderProgram()
{
	DeleteProgram();
	SetFragmentShader(0);
	SetVertexShader(0);
}	

void ShaderProgram::CreateProgram()
{
	//delete current program, if any
	DeleteProgram();

	if (m_pFragmentShader && m_pVertexShader) 
	{
		m_dwProgram = glCreateProgram();
		glAttachShader(m_dwProgram, m_pFragmentShader->GetShader());
		glAttachShader(m_dwProgram, m_pVertexShader->GetShader());
		glLinkProgram(m_dwProgram);
	}
}

void ShaderProgram::DeleteProgram()
{
	if (m_dwProgram >= 0) 
	{
		glDeleteProgram(m_dwProgram);
	}
}

void ShaderProgram::Init()
{
	m_dwProgram			= -1;
	m_pFragmentShader	= 0;
	m_pVertexShader		= 0;
}	

void ShaderProgram::Print(int indent) const
{
	printf("%*sShaderProgram 0x%p\n", indent, " ", this);

	if (m_pFragmentShader)
	{
		m_pFragmentShader->Print(indent + 1);
	}

	if (m_pVertexShader)
	{
		m_pVertexShader->Print(indent + 1);
	}
}

void ShaderProgram::SetFragmentShader(FragmentShader *pShader)
{
	if (m_pFragmentShader != pShader)
	{
		if (m_pFragmentShader)
		{
			m_pFragmentShader->Release();
			m_pFragmentShader = 0;
		}

		if (pShader)
		{
			m_pFragmentShader = pShader;
			m_pFragmentShader->AddRef();
		}
	}
}

void ShaderProgram::SetVertexShader(VertexShader *pShader)
{
	if (m_pVertexShader != pShader)
	{
		if (m_pVertexShader)
		{
			m_pVertexShader->Release();
			m_pVertexShader = 0;
		}

		if (pShader)
		{
			m_pVertexShader = pShader;
			m_pVertexShader->AddRef();
		}
	}
}

void ShaderProgram::Use()
{
	if (gShaderManager()->IsShadersSupported())
	{
		glUseProgram(this->GetProgram());
	}
}