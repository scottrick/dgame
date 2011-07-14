#include "Unknown.h"

#include "UnknownManager.h"

#include <cassert>
using namespace std;

Unknown::Unknown()
{
	Init();
}

Unknown::~Unknown()
{
	gUnknownManager()->RemoveUnknown(this); //update the UnknownManager
}

void Unknown::AddRef()
{
	assert(m_RefCount > 0);

	m_RefCount++;
}

void Unknown::Init()
{
	m_RefCount = 1;

	gUnknownManager()->AddUnknown(this); //update the UnknownManager
}

void Unknown::Release()
{
	assert(m_RefCount > 0);

	m_RefCount--;
	if (m_RefCount == 0)
	{  //delete this object!
		
		delete this;
	}
}