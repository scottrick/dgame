#ifndef _EFFECT_
#define _EFFECT_

#include "Object.h"

class Effect : public Object
{
public:
	Effect();

	void							Clone(Effect *pClone);

	virtual const char				*GetClassName() const					= 0;
	virtual void					Render()								= 0;

	static int						m_dwTexturedQuadDisplayList;

protected:
	virtual ~Effect();

private:
	static int						GenerateTexturedQuadDisplayList();
	void							Init();

};

#endif