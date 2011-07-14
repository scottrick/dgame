#ifndef _PARTICLE_EFFECT_
#define _PARTICLE_EFFECT_

#include "Effect.h"

struct batch;

class ParticleEffect : public Effect
{
public:
	ParticleEffect();

	bool							GetActive() const						{ return m_bActive; }
	virtual const char				*GetClassName() const					= 0;
	virtual void					Refresh(const float &fDeltaTime);
	void							RemoveIfInactive();
	virtual void					Render()								= 0;
	void							SetBatch(batch *pBatch);

protected:

	~ParticleEffect();

	bool							m_bActive;
	batch							*m_pBatch; //particle batch!

private:
	void							Init();

};

#endif