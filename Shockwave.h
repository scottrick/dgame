#ifndef _SHOCKWAVE_
#define _SHOCKWAVE_

#include "PostProcessEffect.h"
#include "PosQuat.h"

class PhysicalObject;

class Shockwave : public PostProcessEffect
{
public:
	Shockwave(const float &fMaxIntensity, const float &fRate, const PosQuat &pq);
	Shockwave(PhysicalObject *pCreateFromThisObject, bool bMega);

	void					FromBlock(Block *pBlock);
	const char				*GetClassName(void) const			{ return "Shockwave"; }
	void					PostRender() const;
	void					Refresh(const float &fDeltaTime);	
	Block					*ToBlock();

protected:
	virtual					~Shockwave();

	float					m_fCurrentIntensity;
	float					m_fMaxIntensity;
	float					m_fRate;
	PosQuat					m_PosQuat;

private:


};

#endif