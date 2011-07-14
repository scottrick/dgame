#ifndef _DRAW_BUFFER_
#define _DRAW_BUFFER_

#include "Unknown.h"
#include <GL/glew.h>

class DrawBuffer : public Unknown
{
public:
	DrawBuffer(int x, int y);
	
	void			Bind();
	static void		BindDefault();

	const char		*GetClassName() const					{ return "DrawBuffer"; }
	GLuint			GetBufferTexture() const				{ return m_dwTexture; }
	void			Print(int indent) const;
	
protected:
	virtual			~DrawBuffer();

	GLuint			m_dwDepthBuffer;
	GLuint			m_dwFrameBuffer;
	GLuint			m_dwTexture;

private:

};

#endif