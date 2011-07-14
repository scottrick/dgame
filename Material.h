#ifndef _MATERIAL_
#define _MATERIAL_

#include "def.h"
#include "Unknown.h"
#include <string>
using namespace std;

class Material : public Unknown
{
public:
	Material();
	
	virtual const char	*GetClassName() const							{ return "Material"; }
	const char			*GetMaterialName() const						{ return m_sName.c_str(); }
	void				GetMoreChunks(ifstream *pFile, int dwStart, int dwEnd, int dwIndent);
	int					GetTexture1() const								{ return m_dwTexture1; }
	void				LoadAFFFChunk(ifstream *pFile, int dwStart, int dwIndent);
	void				LoadChunk(ifstream *pFile, int dwStart, int dwIndent);
	virtual void		Print(int indent) const;
	

	VECTOR3				m_vAmbient;
	VECTOR3				m_vDiffuse;
	VECTOR3				m_vSpecular;
	float				m_fSpecularExponent;
	float				m_fTransparency;		
	//VECTOR4				m_vEmission;

	string				m_sName;				

	string				m_sTexture1Name;
	int					m_dwTexture1;

protected:
	virtual				~Material();

private:
	void				Init();

};

#endif