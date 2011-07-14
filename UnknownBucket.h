#ifndef _UNKNOWN_BUCKET_
#define _UNKNOWN_BUCKET_

#include "Unknown.h"

#include <list>
using namespace std;

class UnknownBucket 
{
public:
	UnknownBucket();
	~UnknownBucket();

	void							AddUnknown(Unknown *pUnknown);
	void							Clear();
	const char						*GetClassBucketName()					{ return m_pClassName; }
	unsigned int					GetCount()								{ return m_vUnknowns.size(); }
	void							RemoveUnknown(Unknown *pUnknown);
	void							SetClassBucketName(const char *pName)	{ m_pClassName = pName; }

protected:
	const char						*m_pClassName;
	list<Unknown *>					m_vUnknowns;

private:
	void							Init();

};

#endif