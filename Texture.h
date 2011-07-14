#ifndef _TEXTURE_
#define _TEXTURE_

#include "Unknown.h"

#include <iostream>
#include <string>
#include <GL/glut.h>
using namespace std;

class Texture : public Unknown
{
public:
	
	typedef struct {
	   char			idlength;
	   char			colourmaptype;
	   char			datatypecode;
	   short int	colourmaporigin;
	   short int	colourmaplength;
	   char			colourmapdepth;
	   short int	x_origin;
	   short int	y_origin;
	   short		width;
	   short		height;
	   char			bitsperpixel;
	   char			imagedescriptor;
	} TGA_HEADER;

	Texture();
	
	void						GenerateTestTexture(GLuint XSize, GLuint YSize);
	virtual const char			*GetClassName() const				{ return "Texture"; }
	GLubyte						*GetData()							{ return m_pTexture; }
	const char					*GetName() const					{ return m_sName.c_str(); }
	const char					*GetShortName() const;
	unsigned int				GetSizeInBytes() const;
	unsigned int				GetSizeX() const					{ return m_dwXSize; }
	unsigned int				GetSizeY() const					{ return m_dwYSize; }
	GLuint						GetTexture() const					{ return texture; }
	GLboolean					LoadFromTGA(const char *pFileName);
	virtual void				Print(int indent) const;

protected:

	virtual ~Texture();

private:

	void						Init();
	void						SetTextureSize(GLuint XSize, GLuint YSize);
	void						SetupTexture(); //called after the texture information has been generated or read in

	unsigned int				m_dwXSize; //texture size in X-Direction
	unsigned int				m_dwYSize; //texture size in Y-Direction

	GLubyte						*m_pTexture;
	
	string						m_sName;
	GLuint						texture;

};

#endif