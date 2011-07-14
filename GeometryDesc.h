#ifndef _GEOMETRY_DESC_
#define _GEOMETRY_DESC_

#include "GeometryData.h"
#include "Unknown.h"
#include "Vertex.h"

#include <vector>
using namespace std;

class Material;
class Visual;

/*
	Describes a set of geometry (triangle strip, fan, list, etc)
*/
class GeometryDesc : public Unknown
{
public:

	enum Type {
		eTRIANGLES,
		eTRIANGLE_STRIP,

		eNUM_TYPES,
	};

	GeometryDesc();

	void							CreateTest(GeometryData *pData);
	virtual const char				*GetClassName() const				{ return "GeometryDesc"; }
	unsigned int					GetFaceCount() const;
	const GeometryData				*GetGeometryData() const			{ return m_pGeometryData; }
	Material						*GetMaterial()						{ return m_pMaterial; }
	void							Load4100Chunk(ifstream *pFile, int dwStart);
	virtual void					Print(int indent) const;
	void							Render();
	void							SetGeometryData(GeometryData *pData);
	void							SetMaterial(Material *pMat);
	void							SetReversedNormals(bool newReversed)	{ m_bReversedNormals = newReversed; }
	void							SetType(Type newType);
	void							SetVisual(Visual *pVisual);

protected:	
	virtual 						~GeometryDesc();

	bool							m_bReversedNormals;
	unsigned int					m_dwType;
	GeometryData					*m_pGeometryData;
	Material						*m_pMaterial;
	Visual							*m_pVisual;							//pointer to the visual this geometry desc belongs to
	vector<VECTOR3>					m_vFaceNormals;
	vector<unsigned int>			m_vIndices;

private:
	void							Init();
	void							PostProcess();						//do stuff like calculate normals

};

#endif