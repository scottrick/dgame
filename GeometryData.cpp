#include "GeometryData.h"

#include <fstream>
using namespace std;

GeometryData::GeometryData()
{
	Init();
}

GeometryData::~GeometryData()
{
	
}

void GeometryData::CreateTest()
{
	//vertex0
	Vertex v0, v1, v2, v3, v4, v5, v6;
	v0.m_vPos.x = 0.0f;		v0.m_vPos.y = 0.0f;  
	v1.m_vPos.x = -0.5f;	v1.m_vPos.y = -1.0f;
	v2.m_vPos.x = 0.5f;		v2.m_vPos.y = -1.0f; 
	v3.m_vPos.x = 1.0f;		v3.m_vPos.y = 0.0f; 
	v4.m_vPos.x = 0.5f;		v4.m_vPos.y = 1.0f; 
	v5.m_vPos.x = -0.5f;	v5.m_vPos.y = 1.0f; 
	v6.m_vPos.x = -1.0f;	v6.m_vPos.y = 0.0f; 

	m_vVertices.push_back(v0);
	m_vVertices.push_back(v1);
	m_vVertices.push_back(v2);
	m_vVertices.push_back(v3);
	m_vVertices.push_back(v4);
	m_vVertices.push_back(v5);
	m_vVertices.push_back(v6);
}

Vertex *GeometryData::GetVertex(unsigned int index)
{
	if (index < m_vVertices.size())
	{
		return &(m_vVertices.at(index));
	}
	else
	{
		return 0;
	}
}		

void GeometryData::Init()
{
	m_vMAX.x = 0.0f;
	m_vMAX.y = 0.0f;
	m_vMAX.z = 0.0f;
	m_vMIN.x = 0.0f;
	m_vMIN.y = 0.0f;
	m_vMIN.z = 0.0f;
}

void GeometryData::Load4110Chunk(ifstream *pFile, int dwStart)
{
	if (!pFile)
	{
		return;
	}

	int				dwChunkID		= 0;
	int				dwChunkSize		= 0;

	pFile->seekg(dwStart);
	pFile->read((char *) &dwChunkID, 2);
	pFile->read((char *) &dwChunkSize, 4);

	if (dwChunkID != 0x4110)
	{ //make sure its the right kind of chunk
		return;
	}

	unsigned int	dwNumVertices	= 0;

	pFile->read((char *) &dwNumVertices, 2);

	Vertex *pVertex = 0;
	float x, y, z;
	for (unsigned int i = 0; i < dwNumVertices; ++i)
	{
		pFile->read((char *) &x, 4);
		pFile->read((char *) &y, 4);
		pFile->read((char *) &z, 4);

		pVertex = GetVertex(i);

		if (pVertex)
		{
			pVertex->m_vPos.x = x;
			pVertex->m_vPos.y = y;
			pVertex->m_vPos.z = z;

			//update extents with the new vertex
			ProcessVertex(pVertex);
		}
		else
		{
			Vertex newVertex;
			newVertex.m_vPos.x = x;
			newVertex.m_vPos.y = y;
			newVertex.m_vPos.z = z;
			m_vVertices.push_back(newVertex);

			//update extents with the new vertex
			ProcessVertex(&newVertex);
		}
	}

	//Print(0);
}

void GeometryData::NormalizeVertexNormals()
{
	for (unsigned int i = 0; i < m_vVertices.size(); ++i)
	{
		NormalizeVECTOR3(m_vVertices.at(i).m_vNorm);
	}
}		

void GeometryData::Print(int indent) const
{
	printf("%*sGeometryData 0x%p\n", indent, "", this);

	vector<Vertex>::const_iterator iter;
	for (iter = m_vVertices.begin(); iter != m_vVertices.end(); iter++)
	{
		(*iter).Print();
	}
}

void GeometryData::ProcessVertex(Vertex *pVertex)
{
	if (pVertex->m_vPos.x > m_vMAX.x)
	{
		m_vMAX.x = pVertex->m_vPos.x;
	}
	else if (pVertex->m_vPos.x < m_vMIN.x) //only check if it is a minimum if it wasn't a max!
	{
		m_vMIN.x = pVertex->m_vPos.x;
	}

	if (pVertex->m_vPos.y > m_vMAX.y)
	{
		m_vMAX.y = pVertex->m_vPos.y;
	}
	else if (pVertex->m_vPos.y < m_vMIN.y)
	{
		m_vMIN.y = pVertex->m_vPos.y;
	}

	if (pVertex->m_vPos.z > m_vMAX.z)
	{
		m_vMAX.z = pVertex->m_vPos.z;
	}
	else if (pVertex->m_vPos.z < m_vMIN.z)
	{
		m_vMIN.z = pVertex->m_vPos.z;
	}
}

void GeometryData::PushBackVertex(Vertex newVertex) 
{
	m_vVertices.push_back(newVertex);
}	