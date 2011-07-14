#ifndef _UI_LABEL_
#define _UI_LABEL_

#include "UIObject.h"

class UILabel : public UIObject
{
public:
	UILabel();
	UILabel(int dwTexture);

	virtual void				FromBlock(Block *pBlock);
	virtual const char			*GetClassName() const				{ return "UILabel"; }
	virtual void				Print(int indent) const;
	virtual void				Refresh(const float &fDeltaTime);
	virtual void				Render();
	void						SetLabelTexture(int dwTexture);
	virtual Block				*ToBlock();

protected:
	virtual ~UILabel();

	int							m_dwLabelTextureID;

private:
	void						Init();

};

#endif