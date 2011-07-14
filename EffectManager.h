/*
#ifndef _EFFECT_MANAGER_
#define _EFFECT_MANAGER_

#include "Manager.h"

#include <list>
using namespace std;

class Effect;
class TestExplosionEffect;

#define TEST_EFFECT_POOL_SIZE			500

class EffectManager : public Manager
{
public:

	EffectManager();
	~EffectManager();
	
	void						Cleanup();
	void						DebugInfoRender();
	void						DebugScreenRender();
	const char					*GetManagerName()				{ return "EffectManager"; }
	void						Refresh(const float &fDeltaTime);
	void						SelectNext();
	void						SelectPrevious();
	
protected:
	
	unsigned int				m_dwSelectedIndex;

private:

	void						Init();

};

extern EffectManager *gEffectManager();

#endif
*/