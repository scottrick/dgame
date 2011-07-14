#ifndef _BLOCK_
#define _BLOCK_

#include "Unknown.h"

/*
	A class to encapsulate a block of data.  These blocks are used to write data to files.  
*/
class Block : public Unknown
{
public:

	Block();

	virtual const char			*GetClassName() const						{ return "Block"; }
	void						*GetData()									{ return m_pData; }
	int							GetDataSize() const							{ return m_dwDataSize; }

	virtual void				Print(int indent) const;
	void						SetData(void *pNewData, int dwNewSize);

protected:

	int							m_dwDataSize;
	void						*m_pData;

private:
	virtual ~Block();

	void						Init();

};

#endif