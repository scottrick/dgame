#ifndef _SPARK_EFFECT_
#define _SPARK_EFFECT_

#include "ParticleEffect.h"

#include "def.h"

class SparkEffect : public ParticleEffect
{
public:
	SparkEffect(const float &fScale, Node *pSourceNode, VECTOR3 vDir);

	void					FromBlock(Block *pBlock);
	const char				*GetClassName() const						{ return "SparkEffect"; }
	void					Render();
	Block					*ToBlock();


protected:
	~SparkEffect();

private:
	void					Init();
	void					Start();

	float					m_fScale;
	VECTOR3					m_vSource;
	VECTOR3					m_vDir;

};

#endif