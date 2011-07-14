#include "Texture.h"

#include <fstream>
using namespace std;

Texture::Texture()
{
	Init();
}

Texture::~Texture()
{
	SetTextureSize(0, 0);
}

void Texture::GenerateTestTexture(GLuint XSize, GLuint YSize)
{
	if ((XSize < 1) || (YSize < 1)) //invalid size, so just return
	{
		return;
	}

	SetTextureSize(XSize, YSize);

	float temp = 0.0f;
	GLuint value;

	//create a big checkerboard texture (8x8 blocks)
	for (unsigned int i = 0; i < XSize; ++i)
	{
		for (unsigned int j = 0; j < YSize; j++)
		{
			temp = (float)i / (float)XSize;
			value = (GLuint)(temp * 255.0f);

			m_pTexture[((j * XSize) + i) * 4] = value;
			m_pTexture[((j * XSize) + i) * 4 + 1] = value;
			m_pTexture[((j * XSize) + i) * 4 + 2] = value;
			m_pTexture[((j * XSize) + i) * 4 + 3] = (GLbyte)0xff; //alpha

			/*
			m_fTexture[((i * dwSize) + j) * 4] = 1.0f;
			m_fTexture[((i * dwSize) + j) * 4 + 1] = 0.4f;
			m_fTexture[((i * dwSize) + j) * 4 + 2] = 0.2f;
			m_fTexture[((i * dwSize) + j) * 4 + 3] = 1.0f;
			*/
		}
	}

	SetupTexture();
}

/*
	returns the last part of the file name.  "\textures\grass.tga"  returns  "grass.tga"
*/
const char *Texture::GetShortName() const
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

unsigned int Texture::GetSizeInBytes() const
{
	unsigned int dwSize = m_dwXSize * m_dwYSize * 4;

	return dwSize;
}

void Texture::Init()
{
	m_dwXSize			= 64; //default size
	m_dwYSize			= 64;
	m_pTexture			= 0;
	texture				= 0xBADBAD;
	m_sName				= "";
}

GLboolean Texture::LoadFromTGA(const char *pFileName)
{
	if (!pFileName)
	{
		return false;
	}

	printf("Loading Texture \"%s\"...\n", pFileName);
	ifstream file(pFileName, ios::binary);	

	if (!file.good())
	{
		file.close();
		return false;
	}

	TGA_HEADER header;

	//Byte Alignment is preventing me from reading the whole struct at once...
	file.read((char *)(&header.idlength), sizeof(char));
	file.read((char *)(&header.colourmaptype), sizeof(char));
	file.read((char *)(&header.datatypecode), sizeof(char));
	file.read((char *)(&header.colourmaporigin), sizeof(short int));
	file.read((char *)(&header.colourmaplength), sizeof(short int));
	file.read((char *)(&header.colourmapdepth), sizeof(char));
	file.read((char *)(&header.x_origin), sizeof(short int));
	file.read((char *)(&header.y_origin), sizeof(short int));
	file.read((char *)(&header.width), sizeof(short int));
	file.read((char *)(&header.height), sizeof(short int));
	file.read((char *)(&header.bitsperpixel), sizeof(char));
	file.read((char *)(&header.imagedescriptor), sizeof(char));

	SetTextureSize(header.width, header.height); //allocate the necessary space

	GLuint numPixels = header.width * header.height;
	GLbyte r, g, b, a;
	a = (GLbyte)0xff; //default alpha

	switch (header.bitsperpixel)
	{
		case 24: //read RGB values, default alpha to 255
			{
				for (GLuint i = 0; i < numPixels; ++i)
				{
					if (file.eof() || (!file.good()))
					{
						//EOF reached early!  bad file!
						SetTextureSize(0, 0);
						file.close();
						return false;
					}

					file.read((char *)(&b), sizeof(GLbyte));
					file.read((char *)(&g), sizeof(GLbyte));
					file.read((char *)(&r), sizeof(GLbyte));
					
					m_pTexture[i * 4 + 0] = r;
					m_pTexture[i * 4 + 1] = g;
					m_pTexture[i * 4 + 2] = b;
					m_pTexture[i * 4 + 3] = a;
				}
			}
			break;

		case 32: //read RGBA values
			{
				for (GLuint i = 0; i < numPixels; ++i)
				{
					if (file.eof() || (!file.good()))
					{
						//EOF reached early!  bad file!
						SetTextureSize(0, 0);
						file.close();
						return false;
					}
					
					file.read((char *)(&b), sizeof(GLbyte));
					file.read((char *)(&g), sizeof(GLbyte));
					file.read((char *)(&r), sizeof(GLbyte));
					file.read((char *)(&a), sizeof(GLbyte));

					m_pTexture[i * 4 + 0] = r;
					m_pTexture[i * 4 + 1] = g;
					m_pTexture[i * 4 + 2] = b;
					m_pTexture[i * 4 + 3] = a;
				}
			}
			break;

		default:
			//not handled bits per pixel
			break;
	}

	file.close();

	m_sName = pFileName;

	//lastly, complete texture setup
	SetupTexture();

	return true;
}

void Texture::Print(int indent) const
{
	printf("%*sTexture 0x%p\n", indent, "", this);
	printf("%*s  Size %dx%d\n", indent, "", m_dwXSize, m_dwYSize);
}

/*
	Deletes any texture information currently being stored in this object and allocates new memory if the size is greater than zero.
*/
void Texture::SetTextureSize(GLuint XSize, GLuint YSize)
{
	if (m_pTexture)
	{
		free(m_pTexture); //free the current memory
		m_pTexture = 0;
		m_dwXSize = 0;
		m_dwYSize = 0;
	}

	if ((XSize > 0) && (YSize > 0))
	{
		m_pTexture = (GLubyte *) malloc(sizeof(GLubyte) * 4 * XSize * YSize); //allocate memory for the texture;  XSize * YSize * 4, four because rgba value for each pixel
		m_dwXSize = XSize;
		m_dwYSize = YSize;
	}
}

void Texture::SetupTexture()
{
	if (m_pTexture)
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		//Setup the texture to use Mip-Mapping 

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); //GL_CLAMP?
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_dwXSize, m_dwYSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pTexture);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, m_dwXSize, m_dwYSize, GL_RGBA, GL_UNSIGNED_BYTE, m_pTexture);

		free(m_pTexture);
		m_pTexture = 0;
	}
}