#ifndef _EXPLOSION_EFFECT_
#define _EXPLOSION_EFFECT_

#include "def.h"
#include "ParticleEffect.h"

class Block;

class ExplosionEffect : public ParticleEffect
{
public:
	ExplosionEffect(const float &fScale, Node *pSourceNode);

	void					FromBlock(Block *pBlock);
	const char				*GetClassName() const						{ return "ExplosionEffect"; }
	void					Render();
	Block					*ToBlock();

protected:
	~ExplosionEffect();

	void					SetSourceNode(Node *pSourceNode);
	void					Start();

	float					m_fScale;
	Node					*m_pSourceNode; //where the explosion spawns from!
	VECTOR3					m_vWind;

	static int				m_dwNumSmokeParticles;

private:
	void					Init();

};

#endif