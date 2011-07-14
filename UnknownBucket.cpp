#include "UnknownBucket.h"

UnknownBucket::UnknownBucket()
{
	Init();
}	

UnknownBucket::~UnknownBucket()
{

}

void UnknownBucket::Init()
{
	m_pClassName		= 0;
}

void UnknownBucket::AddUnknown(Unknown *pUnknown)
{
	if (pUnknown)
	{
		m_vUnknowns.push_back(pUnknown);
	}
}

void UnknownBucket::Clear()
{
	m_vUnknowns.clear();
}

void UnknownBucket::RemoveUnknown(Unknown *pUnknown)
{
	if (pUnknown)
	{
		list<Unknown *>::iterator iter;
		for (iter = m_vUnknowns.begin(); iter != m_vUnknowns.end(); iter++)
		{
			if ((*iter) == pUnknown)
			{
				m_vUnknowns.erase(iter);
				break;
			}
		}
	}
}