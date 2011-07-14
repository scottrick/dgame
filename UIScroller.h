#ifndef _UI_SCROLLER_
#define _UI_SCROLLER_

#include "UIObject.h"

#include <vector>

class UIScroller : public UIObject
{
public:
	UIScroller();

	void					AddUIObject(UIObject *pUIObject);
	void					FromBlock(Block *pBlock);
	const char				*GetClassName() const						{ return "UIScroller"; }
	int						GetSelectedIndex() const					{ return m_dwSelected; }
	void					HandleInput(const unsigned char &key, const int &x, const int &y, const unsigned int &dwKeyboardFlags);
	void					Refresh(const float &fDeltaTime);
	void					Render();
	int						GetMaxDisplayable() const					{ return m_dwMaxDisplayable; }
	void					SelectNext();
	void					SelectPrev();
	void					SetDimensions(const VECTOR3 &vDim)			{ m_vDimensions = vDim; }
	void					SetMaxDisplayable(const int &dwNewMax)		{ m_dwMaxDisplayable = dwNewMax; }
	void					SetRenderBlending(bool bNewBlend)			{ m_bRenderWithBlending = bNewBlend; }
	void					SetSelected(int dwIndex);
	Block					*ToBlock();

protected:
	~UIScroller();

	void					RemoveAllUIObjects();

	int						m_dwMaxDisplayable;

private:
	void					Init();

	vector<UIObject *>		m_vContents;
	bool					m_bCircular;
	bool					m_bRenderWithBlending;

	int						m_dwSelected;
	int						m_dwMinDisplayed;

	float					m_fTimeLeftToScroll;	//current time left to scroll to destination
	float					m_fTimePerScroll;		//total time each change of selection has to scroll

	VECTOR3					m_vDimensions;			//maximum dimensions occupied by this scroller

};

#endif