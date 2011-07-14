#include "GeometryDesc.h"

#include "Material.h"
#include "TextureManager.h"
#include "Vertex.h"
#include "Visual.h"

#include "GL/glut.h"
#include <fstream>
using namespace std;

GeometryDesc::GeometryDesc()
{
	Init();
}	

GeometryDesc::~GeometryDesc()
{
	SetGeometryData(0);
	SetMaterial(0);
}

void GeometryDesc::CreateTest(GeometryData *pData)
{
	if (!pData)
	{
		return;
	}

	m_dwType			= GeometryDesc::eTRIANGLES;
	SetGeometryData(pData);
		
	if (m_pGeometryData->GetNumVertices() > 6) //need at least 7 vertices to create this test thingy
	{
		//just three triangles, so push back 9 indices
		m_vIndices.push_back(0);
		m_vIndices.push_back(1);
		m_vIndices.push_back(2);
		m_vIndices.push_back(0);
		m_vIndices.push_back(3);
		m_vIndices.push_back(4);
		m_vIndices.push_back(0);
		m_vIndices.push_back(5);
		m_vIndices.push_back(6);
	}
}

unsigned int GeometryDesc::GetFaceCount() const
{
	unsigned int dwCount = 0;

	switch (m_dwType)
	{
	case GeometryDesc::eTRIANGLES:
		{
			dwCount = (unsigned int)(m_vIndices.size() / 3);
		}
		break;

	default:
		break;
	}	

	return dwCount;
}	

void GeometryDesc::Init()
{
	m_bReversedNormals	= false;
	m_dwType			= GeometryDesc::eTRIANGLES; //default type
	m_pGeometryData		= 0;
	m_pMaterial			= 0;
	m_pVisual			= 0;
}	

void GeometryDesc::Load4100Chunk(ifstream *pFile, int dwStart)
{
	if (!pFile)
	{
		return;
	}

	int			dwTotalSize			= 0;
	int			dwNextChunk			= 0;
	int			dwChunkID			= 0;
	int			dwChunkSize			= 0;
	int			dwEnd				= 0;

	pFile->seekg(dwStart);
	pFile->read((char *) &dwChunkID, 2);
	pFile->read((char *) &dwTotalSize, 4);
	
	if (dwChunkID != 0x4100)
	{
		return;
	}

	dwNextChunk		= pFile->tellg();
	dwEnd			= dwStart + dwTotalSize;
	
	GeometryData *pNewData = new GeometryData();
	SetGeometryData(pNewData);
	pNewData->Release();

	while (pFile->good() && (dwNextChunk < dwEnd))
	{
		pFile->seekg(dwNextChunk);
		pFile->read((char *) &dwChunkID, 2);
		pFile->read((char *) &dwChunkSize, 4);

		//printf("0x%x,  0x%x\n", dwChunkID, dwChunkSize);
		
		switch (dwChunkID)
		{
		case 0x4110: //vertex list
			{
				pNewData->Load4110Chunk(pFile, dwNextChunk);
			}
			break;

		case 0x4120:
			{
				unsigned int		dwNumFaces	= 0;
				unsigned int		v1			= 0;
				unsigned int		v2			= 0;
				unsigned int		v3			= 0;
				unsigned int		flags		= 0;

				pFile->read((char *) &dwNumFaces, 2);

				for (unsigned int i = 0; i < dwNumFaces; ++i)
				{
					pFile->read((char *) &v1, 2);
					pFile->read((char *) &v2, 2);
					pFile->read((char *) &v3, 2);
					pFile->read((char *) &flags, 2);
	
					m_vIndices.push_back(v1);
					m_vIndices.push_back(v2);
					m_vIndices.push_back(v3);
					//nothing to do with the flags for now
				}

				pFile->read((char *) &v1, 2);
				pFile->read((char *) &v2, 4);
				//printf(" =============0x%x,   0x%x\n", v1, v2);

				if (v1 == 0x4130)
				{
					char c = ' ';
					string mat = "";
					while (c != '\0')
					{
						pFile->read(&c, 1);
						mat += c;
					}
					//printf("found material!  material = %s\n", mat.c_str());
					
					//found the material name, now set material pointer if that material has been created
					if (m_pVisual)
					{
						SetMaterial(m_pVisual->GetMaterialByName(mat.c_str()));
					}
				}
			}
			break;
	
		case 0x4140:
			{		
				unsigned int	dwNumTex	= 0;
				float			fX			= 0.0f;
				float			fY			= 0.0f;
				Vertex			*pVertex	= 0;

				pFile->read((char *) &dwNumTex, 2);

				/*
				printf("  %u texture coordinates found\n", dwNumTex);
				printf("loading texture coordinates\n");
				printf("  %u vertices found\n", pNewData->GetNumVertices());
				*/
				
				for (unsigned int i = 0; i < dwNumTex; ++i)
				{
					pFile->read((char *) &fX, 4); //read in each pair of texture coordinates
					pFile->read((char *) &fY, 4); 
					pVertex = pNewData->GetVertex(i);

					//printf("tex (%f, %f)\n", fX, fY);

					if (pVertex)
					{
						pVertex->m_vTex.x = fX;
						pVertex->m_vTex.y = fY;
					}
				}
			}
			break;

		default:
			break;

		}

		dwNextChunk += dwChunkSize;
	}

	PostProcess();  //calculate normals and other crap
}

void GeometryDesc::PostProcess()
{
	/*
	for each face {
	   calculate face normal
	   add face normal to all 3 vertices of face
	}
	for each vertex {
	   noramlize normal
	}
	*/
	
	if (!m_pGeometryData)
	{
		return;
	}

	unsigned int dwNumFaces = (unsigned int) (m_vIndices.size() / 3);
	Vertex	*p1, *p2, *p3;

	for (unsigned int i = 0; i < dwNumFaces; ++i)
	{
		VECTOR3 v, u;
		
		//printf("getting %d %d %d of %d\n", i * 3, i * 3 + 1, i * 3 + 2, m_vIndices.size());
		
		if (m_bReversedNormals)
		{	
			p1 = m_pGeometryData->GetVertex(m_vIndices.at((i * 3) + 0));  //Find the three vertices!
			p3 = m_pGeometryData->GetVertex(m_vIndices.at((i * 3) + 1));
			p2 = m_pGeometryData->GetVertex(m_vIndices.at((i * 3) + 2));
		}
		else
		{
			p1 = m_pGeometryData->GetVertex(m_vIndices.at((i * 3) + 0));  //Find the three vertices!
			p2 = m_pGeometryData->GetVertex(m_vIndices.at((i * 3) + 1));
			p3 = m_pGeometryData->GetVertex(m_vIndices.at((i * 3) + 2));
		}

		assert (p1 && p2 && p3);
 
		v.x = p2->m_vPos.x - p1->m_vPos.x;   //Calculate the two vectors!
		v.y = p2->m_vPos.y - p1->m_vPos.y;
		v.z = p2->m_vPos.z - p1->m_vPos.z;

		u.x = p3->m_vPos.x - p1->m_vPos.x;
		u.y = p3->m_vPos.y - p1->m_vPos.y;
		u.z = p3->m_vPos.z - p1->m_vPos.z;

		v = CrossProduct(v, u);
		NormalizeVECTOR3(v);
		m_vFaceNormals.push_back(v);

		//add this normal to the vertex normals (which will be normalized when this is all done
		p1->m_vNorm.x += v.x;
		p1->m_vNorm.y += v.y;
		p1->m_vNorm.z += v.z;
		p2->m_vNorm.x += v.x;
		p2->m_vNorm.y += v.y;
		p2->m_vNorm.z += v.z;
		p3->m_vNorm.x += v.x;
		p3->m_vNorm.y += v.y;
		p3->m_vNorm.z += v.z;
	}

	m_pGeometryData->NormalizeVertexNormals();

	return;
}

void GeometryDesc::Print(int indent) const
{
	printf("%*sGeometryDesc 0x%p\n", indent, "", this);
}

void GeometryDesc::Render()
{ //slow render for this geometry desc;  probably want to use Visual::Render() which takes advantage of display lists
	switch (m_dwType)
	{
	case GeometryDesc::eTRIANGLES:
		{
			Vertex			*pVertex		= 0;
			VECTOR3			*pNormal		= 0;
			//unsigned int	dwNumFaces		= m_vIndices.size() / 3;
			int				dwTextureID		= -1;

			if (m_pMaterial)
			{ //if the material exists, use those values instead!
				//Set Default Materials
				GLfloat mat_ambient[]			= { 0.2f, 0.2f, 0.2f, 1.0f };
				GLfloat mat_diffuse[]			= { 0.8f, 0.8f, 0.8f, 1.0f };
				GLfloat mat_specular[]			= { 0.0f, 0.0f, 0.0f, 1.0f };
				GLfloat mat_shininess[]			= { 0.0f };

				mat_ambient[0]				= m_pMaterial->m_vAmbient.x;
				mat_ambient[1]				= m_pMaterial->m_vAmbient.y;
				mat_ambient[2]				= m_pMaterial->m_vAmbient.z;
				mat_ambient[3]				= m_pMaterial->m_fTransparency;

				mat_diffuse[0]				= m_pMaterial->m_vDiffuse.x;
				mat_diffuse[1]				= m_pMaterial->m_vDiffuse.y;
				mat_diffuse[2]				= m_pMaterial->m_vDiffuse.z;
				mat_diffuse[3]				= m_pMaterial->m_fTransparency;

				mat_specular[0]				= m_pMaterial->m_vSpecular.x;
				mat_specular[1]				= m_pMaterial->m_vSpecular.y;
				mat_specular[2]				= m_pMaterial->m_vSpecular.z;
				mat_specular[3]				= m_pMaterial->m_fTransparency;

				mat_shininess[0]			= m_pMaterial->m_fSpecularExponent;

				dwTextureID					= m_pMaterial->GetTexture1();

				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
			}

			//if there is no texture, disable texturing for this object
			if (dwTextureID == -1)
			{
				glDisable(GL_TEXTURE_2D);
				//glBindTexture(GL_TEXTURE_2D, gTextureManager()->GetTextureByFileName("no_tex.tga"));
				//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,	GL_MODULATE);
				//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			}
			else
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, dwTextureID);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			}
	
			glBegin(GL_TRIANGLES);
			
			for (unsigned int i = 0; i < m_vIndices.size(); ++i)
			{
				pVertex = m_pGeometryData->GetVertex(m_vIndices.at(i));

				if (pVertex)
				{
					if (dwTextureID >= 0)
					{
						glTexCoord2f(pVertex->m_vTex.x, pVertex->m_vTex.y);
					}
					glNormal3f(pVertex->m_vNorm.x, pVertex->m_vNorm.y, pVertex->m_vNorm.z);
					glVertex3f(pVertex->m_vPos.x, pVertex->m_vPos.y, pVertex->m_vPos.z);
				}
			}

			glEnd();
		}
	break;

	default:
		break;

	}
}

void GeometryDesc::SetGeometryData(GeometryData *pData)
{
	if (pData != m_pGeometryData)
	{
		if (m_pGeometryData)
		{
			m_pGeometryData->Release();
			m_pGeometryData = 0;
		}

		if (pData)
		{
			m_pGeometryData = pData;
			m_pGeometryData->AddRef();
		}
	}
}

void GeometryDesc::SetMaterial(Material *pMat)
{
	if (m_pMaterial != pMat)
	{
		if (m_pMaterial)
		{
			m_pMaterial->Release();
			m_pMaterial = 0;
		}

		if (pMat)
		{
			m_pMaterial = pMat;
			m_pMaterial->AddRef();
		}
	}
}

void GeometryDesc::SetType(Type newType)
{
	m_dwType = newType;
}

void GeometryDesc::SetVisual(Visual *pVisual)
{
	m_pVisual = pVisual; //don't addref... this pointer should only be used to get the material when loading!
}	