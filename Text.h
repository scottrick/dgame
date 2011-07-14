#ifndef _TEXT_
#define _TEXT_

#include "Object.h"
#include "TextNode.h"

class Text : public Object
{
public:
	Text(const char *pText);
	Text(const char *pText, VECTOR3 vColor);

	virtual void				FromBlock(Block *pBlock); 
	virtual const char			*GetClassName() const			{ return "Text"; }
	const char					*GetText() const				{ return m_pText; }
	TextNode					*GetTextNode() const			{ return (TextNode *)m_pNode; }

	virtual void				Refresh(const float &fDeltaTime);
	void						SetRotate(bool bNewRotate)		{ m_bRotate = bNewRotate; }
	virtual Block				*ToBlock();	

protected:
	~Text();	

private:
	void						Init();
	void						Setup(VECTOR3 vColor);

	const char					*m_pText;

	bool						m_bRotate;

};

#endif