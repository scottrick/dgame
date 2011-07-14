#ifndef _TEXTNODE_
#define _TEXTNODE_				1

#include "Node.h"
#include "PosQuat.h"
#include "Unknown.h"
#include "Visual.h"

#include <vector>
using namespace std;

/*
	A TextNode is a special Node for drawing text
*/
class TextNode : public Node
{
public:
	TextNode(const char *pText);
	TextNode(const char *pText, VECTOR3 vColor);

	virtual const char			*GetClassName() const					{ return "TextNode"; }
	float						GetTextWidth() const					{ return m_fTextWidth; }
	virtual void				Print(int indent) const;
	virtual void				Render();
	void						RenderSlow();
	void						SetTextColor(VECTOR3 vNewColor)			{ m_vTextColor = vNewColor; }

protected:
	virtual	~TextNode();

private:
	void						CreateDisplayList();
	void						DestroyDisplayList();
	void						Init();

	int							m_dwDisplayList;
	const char					*m_pText;
	VECTOR3						m_vTextColor;

	float						m_fTextWidth;

};

#endif