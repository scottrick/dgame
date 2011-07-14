#ifndef _VERTEX_
#define _VERTEX_

#include "def.h"

class Vertex
{
public:
	Vertex();
	~Vertex();

	VECTOR3						m_vPos;			//vertex location
	VECTOR3						m_vNorm;		//vertex normal
	VECTOR2						m_vTex;			//vertex texture coordinate

	void						Print() const;

protected:

private:
	void						Init();

};

#endif