#ifndef _FIREWORK_EFFECT_
#define _FIREWORK_EFFECT_

#include "ParticleEffect.h"

#include "def.h"

class FireworkEffect : public ParticleEffect
{
public:
	FireworkEffect(const float &fScale, const VECTOR3 &vPos);

	void					FromBlock(Block *pBlock);
	const char				*GetClassName() const						{ return "FireworkEffect"; }
	void					Render();
	Block					*ToBlock();


protected:
	~FireworkEffect();

private:
	void					Init();
	void					Start();

	float					m_fScale;
	VECTOR3					m_vSource;
	VECTOR3					m_vWind;
	VECTOR3					m_vColor;

};

#endif