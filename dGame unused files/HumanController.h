#ifndef _HUMAN_CONTROLLER_
#define _HUMAN_CONTROLLER_

#include "Controller.h"

class HumanController : public Controller
{
public:
	HumanController();

	virtual void					FromBlock(Block *pBlock);
	virtual const char				*GetClassName() const				{ return "HumanController"; }
	virtual void					Print(int indent) const;
	virtual void					Refresh(int dwDeltaTime);
	virtual Block					*ToBlock();

protected:
	virtual ~HumanController();

private:
	void							Init();

	float							m_fAcceleration;
	float							m_fSpeed;

	int								m_dwTempFireCooldown;
	int								m_dwCurrentCooldown;

};

#endif