#ifndef _UI_CONTAINER_
#define _UI_CONTAINER_

#include "UIObject.h"

class UIContainer : public UIObject
{
public:
	UIContainer();

	virtual void				FromBlock(Block *pBlock);
	virtual const char			*GetClassName() const				{ return "UIContainer"; }
	virtual void				Print(int indent) const;
	virtual void				Refresh(const float &fDeltaTime);
	virtual void				Render();
	virtual Block				*ToBlock();

protected:
	virtual ~UIContainer();

private:
	void						Init();

};
	
#endif