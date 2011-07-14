#include "Material.h"

#include "TextureManager.h"

#include <fstream>
using namespace std;

Material::Material()
{
	Init();
}

Material::~Material()
{

}

void Material::GetMoreChunks(ifstream *pFile, int dwStart, int dwEnd, int dwIndent)
{
	if (!pFile)
	{
		return;
	}

	int			dwNextChunk			= dwStart;
	int			dwChunkID			= 0;
	int			dwChunkSize			= 0;

	while (pFile->good() && (dwNextChunk < dwEnd))
	{
		pFile->seekg(dwNextChunk);
		pFile->read((char *) &dwChunkID, 2);
		pFile->read((char *) &dwChunkSize, 4);
		
		LoadChunk(pFile, dwNextChunk, dwIndent);

		dwNextChunk += dwChunkSize;
	}
}	

void Material::Init()
{
	m_vAmbient.x			= 0.1f;
	m_vAmbient.y			= 0.1f;
	m_vAmbient.z			= 0.1f;
	m_vDiffuse.x			= 1.0f;
	m_vDiffuse.y			= 1.0f;
	m_vDiffuse.z			= 1.0f;
	m_vSpecular.x			= 1.0f;
	m_vSpecular.y			= 1.0f;
	m_vSpecular.z			= 1.0f;
	//m_vEmission.x			= 0.0f;
	//m_vEmission.y			= 0.0f;
	//m_vEmission.z			= 0.0f;
	//m_vEmission.w			= 0.0f;
	m_fSpecularExponent		= 50.0f;
	m_fTransparency			= 1.0f;
	m_sName					= "NO_MAT_NAME";
	m_sTexture1Name			= "NO_TEXTURE_NAME";
	m_dwTexture1			= -1;
}

void Material::LoadAFFFChunk(std::ifstream *pFile, int dwStart, int dwIndent)
{
	if (!pFile)
	{
		return;
	}
	
	int		dwID				= 0;
	int		dwTotalSize			= 0;
	
	pFile->seekg(dwStart);
	pFile->read((char *) &dwID, 2);
	pFile->read((char *) &dwTotalSize, 4);

	if (dwID == 0xAFFF) //is a 3ds file!
	{
		GetMoreChunks(pFile, ((int)pFile->tellg()), ((int)pFile->tellg()) + dwTotalSize - 6, dwIndent);
		//LoadChunk(pFile, ((int)pFile->tellg()) - 6, dwIndent);
	}
}	

void Material::LoadChunk(ifstream *pFile, int dwStart, int dwIndent)
{
	if (!pFile)
	{
		return;
	}

	int			dwChunkID			= 0;
	int			dwChunkSize			= 0;

	pFile->seekg(dwStart);
	pFile->read((char *)&dwChunkID, 2);
	pFile->read((char *)&dwChunkSize, 4);

	//printf("%*s0x%04x,   0x%08x\n", dwIndent, "", dwChunkID, dwChunkSize);
	
	switch (dwChunkID)
	{
	case 0xA000:
		{ //read in material name
			m_sName = "";
			char c = 'a';

			while (c != '\0')
			{	
				pFile->read(&c, 1);
				if (c != '\0')
				{
					m_sName += c;
				}	
			}

			//printf("found name = %s\n", m_sName.c_str());
		}
		break;

	case 0xA010:
		{ //ambient color
			int id			= 0;
			int size		= 0;

			pFile->read((char *) &id, 2);
			pFile->read((char *) &size, 4);

			//only care about the 0x11 color for now, which is the first one

			int x = 0;  
			int y = 0;  
			int z = 0;

			pFile->read((char *) &x, 1);
			pFile->read((char *) &y, 1);
			pFile->read((char *) &z, 1);
			
			//printf("ambient = (%d, %d, %d)\n", x, y, z);
			m_vAmbient.x = ((float)x) / 255.0f;
			m_vAmbient.y = ((float)y) / 255.0f;
			m_vAmbient.z = ((float)z) / 255.0f;
		}
		break;

	case 0xA020:
		{ //diffuse color
			int id			= 0;
			int size		= 0;

			pFile->read((char *) &id, 2);
			pFile->read((char *) &size, 4);

			//only care about the 0x11 color for now, which is the first one

			int x = 0;  
			int y = 0;  
			int z = 0;

			pFile->read((char *) &x, 1);
			pFile->read((char *) &y, 1);
			pFile->read((char *) &z, 1);
			
			//printf("diffuse = (%d, %d, %d)\n", x, y, z);
			m_vDiffuse.x = ((float)x) / 255.0f;
			m_vDiffuse.y = ((float)y) / 255.0f;
			m_vDiffuse.z = ((float)z) / 255.0f;
		}
		break;

	case 0xA030:
		{ //specular color
			int id			= 0;
			int size		= 0;

			pFile->read((char *) &id, 2);
			pFile->read((char *) &size, 4);

			//only care about the 0x11 color for now, which is the first one

			int x = 0;  
			int y = 0;  
			int z = 0;

			pFile->read((char *) &x, 1);
			pFile->read((char *) &y, 1);
			pFile->read((char *) &z, 1);
			
			//printf("specular = (%d, %d, %d)\n", x, y, z);
			m_vSpecular.x = ((float)x) / 255.0f;
			m_vSpecular.y = ((float)y) / 255.0f;
			m_vSpecular.z = ((float)z) / 255.0f;
		}
		break;

	case 0xA040:
		{ //specular reflectivity
			
		}
		break;

	case 0xA041:
		{ //specular exponent
			int id			= 0;
			int size		= 0;

			pFile->read((char *) &id, 2);
			pFile->read((char *) &size, 4);

			if (id == 0x30)
			{ //read double byte
				int			dwShine		= 0;
				pFile->read((char *) &dwShine, 2);
				m_fSpecularExponent = (float)dwShine;
				if (m_fSpecularExponent > 128.0f)
				{
					m_fSpecularExponent = 128.0f;
				}
				if (m_fSpecularExponent < 1.0f)
				{
					m_fSpecularExponent = 1.0f;
				}
				//printf("exp = %f\n", m_fSpecularExponent);
			}
		}
		break;

	case 0xA050:
		{ //transparency
			int id			= 0;
			int size		= 0;

			pFile->read((char *) &id, 2);
			pFile->read((char *) &size, 4);

			//printf("0x%x, %d\n", id, size);

			if (id == 0x30)
			{ //read double byte
				int			dwTrans		= 0;
				pFile->read((char *) &dwTrans, 2);
				
				m_fTransparency = (float)dwTrans / 65535.0f;
				//printf("transparency = %f\n", m_fTransparency);
			}
		}
		break;

	case 0xA200:
		{	
			int id			= 0;
			int size		= 0;

			pFile->read((char *) &id, 2);
			pFile->read((char *) &size, 4);

			m_sTexture1Name = "";
			char c = 'a';

			while (c != '\0')
			{	
				pFile->read(&c, 1);
				if (c != '\0')
				{
					m_sTexture1Name += c;
				}	
			}

			//IMPORTANT NOTE:  TEXTURE FILENAME IS LIMITED TO 12 CHARACTERS BY 3DS, EVIDENTLY!!

			//printf("searching for texture map1 \"%s\"\n", m_sTexture1Name.c_str());
			m_dwTexture1 = gTextureManager()->GetTextureByFileName(m_sTexture1Name.c_str());

			if (m_dwTexture1 == -1)
			{ //if the texture was not found, assign the default texture
				m_dwTexture1 = gTextureManager()->GetTextureByFileName("no_tex.tga");
				printf("  \"%s\" -> texture not found, using default...\n", m_sTexture1Name.c_str());
			}
		}
		break;

	case 0xAFFF:
		{
			GetMoreChunks(pFile, pFile->tellg(), ((int)pFile->tellg()) + dwChunkSize - 6, dwIndent + 2);
		}
		break;

	default:
		{

		}
		break;
	}
}

void Material::Print(int indent) const
{
	printf("%*sMaterial 0x%p\n", indent, "", this);
}