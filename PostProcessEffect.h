#ifndef _POST_PROCESS_EFFECT_
#define _POST_PROCESS_EFFECT_

#include "Object.h"

class PostProcessEffect : public Object
{
public:
	PostProcessEffect();

	virtual const char				*GetClassName() const					= 0;
	virtual void					PostRender() const						= 0;

protected:
	virtual ~PostProcessEffect();


private:


};

#endif