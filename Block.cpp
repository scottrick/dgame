#include "Block.h"

#include <iostream>
using namespace std;

Block::Block()
{
	Init();
}

Block::~Block()
{
	SetData(0, 0);
}

void Block::Init()
{
	m_pData			= 0;
	m_dwDataSize	= 0;
}

void Block::Print(int indent) const
{
	printf("%*sBlock 0x%p Data@0x%p, Size=%d\n", indent, "", this, m_pData, m_dwDataSize);
}

void Block::SetData(void *pNewData, int dwNewSize)
{
	if (pNewData != m_pData)
	{
		if (m_pData)
		{
			free(m_pData);
			m_dwDataSize = 0;
		}

		if (pNewData)
		{
			m_pData = pNewData;
			m_dwDataSize = dwNewSize;
		}
	}
}