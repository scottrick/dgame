#ifndef _VISUAL_
#define _VISUAL_

#include "def.h"
#include "Unknown.h"

#include <string>
#include <vector>
using namespace std;

class GeometryData;
class GeometryDesc;
class Material;

/*
	A Visual is an object which stores the mesh information about one specific object, 
	probably loaded from the same file.  So, ONE 3DS file will be stored in ONE visual object.
*/
class Visual : public Unknown
{
public:

	Visual();

	void							AddGeometryDesc(GeometryDesc *pNewDesc);
	void							AddMaterial(Material *pNewMat);
	bool							CreateFromDisplayList(int dwDisplayList, float fScale, char cChar);
	virtual const char				*GetClassName() const				{ return "Visual"; }
	unsigned int					GetFaceCount() const				{ return m_dwNumFaces; }
	unsigned int					GetVertCount() const				{ return m_dwNumVertices; }
	Material						*GetMaterialByName(const char *pName) const;
	VECTOR3							GetMaxExtents() const				{ return m_vMAX; }
	VECTOR3							GetMinExtents() const				{ return m_vMIN; }
	const char						*GetName()							{ return m_sName.c_str(); }
	float							GetScale() const					{ return m_fSCALE; }
	const char						*GetShortName();
	VECTOR3							GetTrans() const					{ return m_vTRANS; }
	bool							LoadFrom3DS(const char *pFileName);
	virtual void					Print(int indent) const;
	
	void							Render();
	virtual void					RenderSlow();						//render without display lists!
	void							Test();

protected:

	virtual							~Visual();

	vector<GeometryDesc *>			m_vGeometryDescriptions;			//there may be multiple descriptions for the set of vertices
	vector<Material *>				m_vMaterials;						//holds all the materials, since they may be used by more than one description

	VECTOR3							m_vMAX;								//the visuals extents, not modified by scale or translation
	VECTOR3							m_vMIN;

	VECTOR3							m_vTRANS;							//the translation needed to get this thing centered at the origin
	float							m_fSCALE;							//the scale factor to get this thing in a 1x1x1 cube

	bool							m_bReversedNormals;
	int								m_dwDisplayList;					//display list ID for this Visual

private:

	void							ClearGeometryDescriptions();
	void							ClearMaterials();
	void							CreateDisplayList();
	void							DestroyDisplayList();
	void							Init();
	void							GetMoreChunks(ifstream *pFile, int dwStart, int dwEnd, int dwIndent);
	void							LoadChunk(ifstream *pFile, int dwStart, int dwIndent);
	void							UpdateExtents();
	void							UpdateStats();
	
	string							m_sName;
	unsigned int					m_dwNumFaces;
	unsigned int					m_dwNumVertices;

};

#endif