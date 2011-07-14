#ifndef _SPLASH_EFFECT_
#define _SPLASH_EFFECT_

#include "ParticleEffect.h"

#include "ParticleManager.h"

#define NUM_BATCHES			15
#define TIME_TO_FIRE_ALL	2.0f

#define	TOP					1
#define	BOTTOM				2
#define LEFT				3
#define RIGHT				4

class SplashEffect : public ParticleEffect
{
public:
	SplashEffect();

	void					FromBlock(Block *pBlock);
	const char				*GetClassName() const						{ return "SplashEffect"; }
	void					Refresh(const float &fDeltaTime);
	void					Render();
	Block					*ToBlock();

protected:
	virtual ~SplashEffect();

	batch					**m_pBatches;
	VECTOR3					m_vLaunchPoint;

private:
	void					Init();
	void					Randomize();

	float					m_fFireDelay;
	float					m_fTimeSinceLastLaunch;
	float					m_fLaunchPointSpeed;
	float					m_fSessionTimeLeft;

	bool					m_bShooting;
	bool					m_bClockwise;
	int						m_dwLoc;

};

#endif