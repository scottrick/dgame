#include "DrawBuffer.h"

#include "ShaderManager.h"

#include <GL/glew.h>

DrawBuffer::DrawBuffer(int x, int y)
{
	if (gShaderManager()->IsShadersSupported())
	{
		glGenFramebuffersEXT(1, &m_dwFrameBuffer);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_dwFrameBuffer);

		glGenRenderbuffersEXT(1, &m_dwDepthBuffer);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_dwDepthBuffer);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, x, y);
		
		glGenTextures(1, &m_dwTexture);
		glBindTexture(GL_TEXTURE_2D, m_dwTexture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_dwTexture, 0);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_dwDepthBuffer);

		GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);	

		DrawBuffer::BindDefault();
	}
}

DrawBuffer::~DrawBuffer()
{
	if (gShaderManager()->IsShadersSupported())
	{
		glDeleteFramebuffersEXT(1, &m_dwFrameBuffer);
		glDeleteRenderbuffersEXT(1, &m_dwDepthBuffer);
		glDeleteTextures(1, &m_dwTexture);	
	}
}

void DrawBuffer::Bind()
{
	if (gShaderManager()->IsShadersSupported())
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_dwFrameBuffer);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_dwDepthBuffer);
	}
}

void DrawBuffer::BindDefault()
{
	if (gShaderManager()->IsShadersSupported())
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	}
}

void DrawBuffer::Print(int indent) const
{
	printf("%*sDrawBuffer 0x%p\n", indent, " ", this);
}