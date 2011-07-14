#ifndef _GEOMETRY_DATA_
#define _GEOMETRY_DATA_

#include "Unknown.h"
#include "Vertex.h"

#include <vector>
using namespace std;

class GeometryData : public Unknown
{
public:
	GeometryData();

	void						CreateTest();
	
	virtual const char			*GetClassName() const				{ return "GeometryData"; }
	VECTOR3						GetExtentsMax() const				{ return m_vMAX; }
	VECTOR3						GetExtentsMin() const				{ return m_vMIN; }
	unsigned int				GetNumVertices() const				{ return (unsigned int)m_vVertices.size(); }
	Vertex						*GetVertex(unsigned int index);
	void						Load4110Chunk(ifstream *pFile, int dwStart);
	void						NormalizeVertexNormals();
	virtual void				Print(int indent) const;
	void						PushBackVertex(Vertex newVertex);
	
protected:
	virtual						~GeometryData();

	vector<Vertex>				m_vVertices;
	
	// THE MIN / MAX VALUES so we can determine the geometries extents!
	VECTOR3						m_vMAX;
	VECTOR3						m_vMIN;

private:
	void						Init();
	void						ProcessVertex(Vertex *pVertex);

};

#endif