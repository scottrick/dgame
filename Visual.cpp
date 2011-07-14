#include "Visual.h"

#include "GeometryData.h"
#include "GeometryDesc.h"
#include "Material.h"
#include "TextureManager.h"

#include <fstream>
#include <iostream>
#include <GL/glut.h>
using namespace std;

Visual::Visual()
{
	Init();
}

Visual::~Visual()
{
	ClearGeometryDescriptions();
	ClearMaterials();
	DestroyDisplayList();
}

void Visual::AddGeometryDesc(GeometryDesc *pNewDesc)
{
	if (pNewDesc)
	{
		m_vGeometryDescriptions.push_back(pNewDesc);
		pNewDesc->AddRef();
	}
}

void Visual::AddMaterial(Material *pNewMat)
{
	if (pNewMat)
	{
		m_vMaterials.push_back(pNewMat);
		pNewMat->AddRef();
	}
}

bool Visual::CreateFromDisplayList(int dwDisplayList, float fScale, char cChar)
{
	if (m_dwDisplayList != -1) {
		assert(0 && "can't create from display list; visual already created!");
		return false;
	}

	m_dwDisplayList = dwDisplayList;
	m_fSCALE		= fScale;
	m_sName			= &cChar;
	
	return true;
}

void Visual::ClearGeometryDescriptions()
{
	GeometryDesc							*pDesc = 0;
	vector<GeometryDesc *>::iterator		iter;
	for (iter = m_vGeometryDescriptions.begin(); iter != m_vGeometryDescriptions.end(); iter++)
	{
		pDesc = *iter;
		if (pDesc)
		{		
			pDesc->SetVisual(0);
			pDesc->Release();
		}
	}
	m_vGeometryDescriptions.clear();
}

void Visual::ClearMaterials()
{
	Material								*pMat = 0;
	vector<Material *>::iterator			iter;
	for (iter = m_vMaterials.begin(); iter != m_vMaterials.end(); iter++)
	{
		pMat = *iter;
		if (pMat)
		{
			pMat->Release();
		}
	}
	m_vMaterials.clear();
}	

void Visual::CreateDisplayList()
{
	if (m_dwDisplayList != -1)
	{ //display list already exists!
		return;
	}

	m_dwDisplayList = glGenLists(1);
	glNewList(m_dwDisplayList, GL_COMPILE);
	RenderSlow();
	glEndList();
}

void Visual::DestroyDisplayList()
{
	if (m_dwDisplayList == -1)
	{ //no display list to destroy!
		return;
	}

	glDeleteLists(m_dwDisplayList, 1);
	m_dwDisplayList = -1;
}

Material *Visual::GetMaterialByName(const char *pName) const
{
	Material *pMat		= 0;

	vector<Material *>::const_iterator iter;
	for (iter = m_vMaterials.begin(); iter != m_vMaterials.end(); iter++)
	{
		pMat = *iter;
		if (pMat)
		{
			if (strcmp(pName, pMat->GetMaterialName()) == 0)
			{
				//found the material!
				return pMat;
			}	
		}

		pMat = 0;
	}

	return pMat; //returns NULL if material wasn't found
}

void Visual::GetMoreChunks(ifstream *pFile, int dwStart, int dwEnd, int dwIndent)
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

/*
	returns the last part of the file name.  "\textures\grass.tga"  returns  "grass.tga"
*/
const char *Visual::GetShortName()
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

void Visual::Init()
{
	m_bReversedNormals	= false;

	m_vMAX.x	= 0.0f;
	m_vMAX.y	= 0.0f;
	m_vMAX.z	= 0.0f;
	m_vMIN.x	= 0.0f;
	m_vMIN.y	= 0.0f;
	m_vMIN.z	= 0.0f;

	m_fSCALE	= 1.0f;
	m_vTRANS.x	= 0.0f;
	m_vTRANS.y	= 0.0f;
	m_vTRANS.z	= 0.0f;
	
	m_dwDisplayList		= -1; //-1 Signifies that this Visual does not yet have a display list!
	m_sName				= "";
	m_dwNumVertices		= 0;
	m_dwNumFaces		= 0;
}

void Visual::LoadChunk(ifstream *pFile, int dwStart, int dwIndent)
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
		case 0x0002:
			{
				int dwVersion = 0;
				pFile->read((char *) &dwVersion, 4);
				//printf("  Version %d  (0x%08x)\n", dwVersion, dwVersion);
			}
			break;

		case 0x3d3d:
			{
				GetMoreChunks(pFile, pFile->tellg(), ((int)pFile->tellg()) + dwChunkSize - 6, dwIndent + 2);
			}
			break;

		case 0x4000:
			{
				int dwEnd = ((int)pFile->tellg()) + dwChunkSize - 6;

				string sName = "";
				char c = 'a';
				while (c != '\0')
				{	
					pFile->read(&c, 1);
					if (c != '\0')
					{
						sName += c;
					}	
				}

				//printf("NAME=%s\n", sName.c_str());

				GetMoreChunks(pFile, pFile->tellg(), dwEnd, dwIndent + 2);
			}
			break;

		case 0x4100:
			{
				GeometryDesc *pNewDesc = new GeometryDesc();
				pNewDesc->SetReversedNormals(m_bReversedNormals);
				pNewDesc->SetVisual(this);
				pNewDesc->Load4100Chunk(pFile, dwStart);
				AddGeometryDesc(pNewDesc);
				pNewDesc->Release();
			}
			break;

		case 0x4110: //the vertex list
			{
				assert(0);//SHOULDNT HIT THIS
			}
			break;

		case 0x4d4d:
			{
				GetMoreChunks(pFile, pFile->tellg(), ((int)pFile->tellg()) + dwChunkSize - 6, dwIndent + 2);
			}
			break;

		case 0xafff:
			{
				Material *pNewMat = new Material();
				pNewMat->LoadAFFFChunk(pFile, ((int)pFile->tellg()) - 6, dwIndent + 2);
				AddMaterial(pNewMat);
				pNewMat->Release();
			}
			break;

		default:
			break;

	}	
}

bool Visual::LoadFrom3DS(const char *pFileName)
{
	if (!pFileName)
	{
		return false;
	}

	printf("Loading 3DS \"%s\"...\n", pFileName);
	ifstream file(pFileName, ios::binary);	

	if (!file.good())
	{
		file.close();
		printf("FILE NOT FOUND\n");
		return false;
	}

	int		dwID				= 0;
	int		dwTotalSize			= 0;
	
	file.read((char *) &dwID, 2);
	file.read((char *) &dwTotalSize, 4);

	if (dwID == 0x4d4d) //is a 3ds file!
	{
		LoadChunk(&file, 0, 0);
	}

	file.close();

	UpdateExtents();	//examine the geometry and find stuff lawl
	UpdateStats();		//sets the face and vertices counts for the whole visual

	/*
	printf("%u Faces Found!\n", GetFaceCount());
	printf("EXTENTS\n X(%f, %f)\n Y(%f, %f)\n Z(%f, %f)\n", m_vMAX.x, m_vMIN.x, m_vMAX.y, m_vMIN.y, m_vMAX.z, m_vMIN.z);
	printf("SCALE %f\n", m_fSCALE);
	printf("TRANS (%f, %f, %f)\n", m_vTRANS.x, m_vTRANS.y, m_vTRANS.z);
	*/

	CreateDisplayList();
	ClearGeometryDescriptions(); //don't need the info anymore so get rid of it!

	m_sName = pFileName;

	return true;
}

/*
bool Visual::LoadFromVISUAL(const char *pFileName)
{
	printf("Loading VISUALS \"%s\"...\n", pFileName);

	float		fSpaceWidth;
	ifstream	input;
	input.open(pFileName, istream::binary);

	char		buffer[14];
	input.read(buffer, 14);
	if (input.fail()) {
		return false; //failure!!!
	}
	
	const char header[15] = "dGameTextData\0"; //check to make sure it has a valid header
	for(int i = 0; i < 8; ++i) {
		if (buffer[i] != header[i]) {
			return false;  //failure!!
		}
	}
	
	input.read(buffer, 5);
	fSpaceWidth = ToFloat(buffer);




	input.close();

	return true;
}
*/

void Visual::Print(int indent) const
{
	printf("%*sVisual 0x%p\n", indent, " ", this);
}

void Visual::Render()
{
	if (m_dwDisplayList == -1)
	{	
		RenderSlow();
	}
	else 
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		glCallList(m_dwDisplayList);
	}
}

void Visual::RenderSlow() //renders without display lists!
{
	glPushMatrix();

	//SCALE SO IT ISNT SO BIG
	glScalef(m_fSCALE, m_fSCALE, m_fSCALE);

	//TRANSLATE THIS THING SO IT IS CENTERED LAWL
	glTranslatef(m_vTRANS.x, m_vTRANS.y, m_vTRANS.z);

	vector<GeometryDesc *>::iterator iter;
	GeometryDesc *pDesc = 0;
	for (iter = m_vGeometryDescriptions.begin(); iter != m_vGeometryDescriptions.end(); iter++)
	{
		pDesc = *iter;
		if (pDesc)
		{	
			pDesc->Render();
		}
	}

	glPopMatrix();
}

void Visual::Test()
{
	/*
	//create test geometrydata
	GeometryData *pTestData = new GeometryData();
	pTestData->CreateTest();
	SetGeometryData(pTestData);
	pTestData->Release();

	//create test geometrydesc from the test data
	GeometryDesc *pTestDesc = new GeometryDesc();
	pTestDesc->CreateTest(m_pGeometryData);
	m_vGeometryDescriptions.push_back(pTestDesc);
	*/
}

void Visual::UpdateExtents()
{
	GeometryDesc *pDesc = 0;
	vector<GeometryDesc *>::iterator iter;
	for (iter = m_vGeometryDescriptions.begin(); iter != m_vGeometryDescriptions.end(); iter++)
	{
		pDesc = *iter;
		if (pDesc && pDesc->GetGeometryData())
		{
			VECTOR3 max = pDesc->GetGeometryData()->GetExtentsMax();
			VECTOR3 min	= pDesc->GetGeometryData()->GetExtentsMin();

			if (max.x > m_vMAX.x)
			{
				m_vMAX.x = max.x;
			}
			if (max.y > m_vMAX.y)
			{
				m_vMAX.y = max.y;
			}
			if (max.z > m_vMAX.z)
			{
				m_vMAX.z = max.z;
			}
			if (min.x < m_vMIN.x)
			{
				m_vMIN.x = min.x;
			}
			if (min.y < m_vMIN.y)
			{
				m_vMIN.y = min.y;
			}
			if (min.z < m_vMIN.z)
			{
				m_vMIN.z = min.z;
			}
		}
	}

	float fMaxSize		= 0.0f;
	float xSize			= m_vMAX.x - m_vMIN.x;
	float ySize			= m_vMAX.y - m_vMIN.y;
	float zSize			= m_vMAX.z - m_vMIN.z;
	
	if (xSize > ySize)
	{
		fMaxSize = xSize;
	}
	else
	{
		fMaxSize = ySize;
	}
	if (zSize > fMaxSize)
	{
		fMaxSize = zSize;
	}

	if (fMaxSize > MATH_NORMALIZATION_ALLOWABLE_ERROR)
	{
		m_fSCALE = 1.0f / fMaxSize;
	}
	else 
	{
		m_fSCALE = 1.0f;
	}

	//calculate m_fTRANS here?
	m_vTRANS.x = -(m_vMAX.x + m_vMIN.x) / 2.0f;
	m_vTRANS.y = -(m_vMAX.y + m_vMIN.y) / 2.0f;
	m_vTRANS.z = -(m_vMAX.z + m_vMIN.z) / 2.0f;
}

void Visual::UpdateStats()
{
	m_dwNumFaces		= 0;
	m_dwNumVertices		= 0;

	vector<GeometryDesc *>::const_iterator iter;
	for (iter = m_vGeometryDescriptions.begin(); iter != m_vGeometryDescriptions.end(); iter++)
	{
		m_dwNumFaces		+= (*iter)->GetFaceCount();
		m_dwNumVertices		+= (*iter)->GetGeometryData()->GetNumVertices();
	}
}