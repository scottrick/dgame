#ifndef _UI_EFFECT_
#define _UI_EFFECT_

#include "UIObject.h"

class Effect;

class UIEffect : public UIObject
{
public:
	UIEffect();

	void						FromBlock(Block *pBlock);
	const char					*GetClassName() const		{ return "UIEffect"; }
	void						Refresh(const float &fDeltaTime);
	void						Render();
	void						SetEffect(Effect *pEffect);
	Block						*ToBlock();

protected:
	~UIEffect();

private:
	
	void						Init();

	Effect						*m_pEffect;

};

#endif