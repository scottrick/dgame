#ifndef _COMPUTER_CONTROLLER_
#define _COMPUTER_CONTROLLER_

#include "Controller.h"

class MovementPlugin;
class FiringPlugin;

class ComputerController : public Controller
{
public:

	ComputerController();

	virtual void						FromBlock(Block *pBlock);
	virtual const char					*GetClassName() const			{ return "ComputerController"; }
	virtual void						Print(int indent) const;
	virtual void						Refresh(int dwDeltaTime);
	void								SetFiringPlugin(FiringPlugin *pPlugin);
	void								SetMovementPlugin(MovementPlugin *pPlugin);
	Block								*ToBlock();

protected:

	virtual ~ComputerController();
	
	FiringPlugin						*m_pFiringPlugin;
	MovementPlugin						*m_pMovementPlugin;		

private:

	void								Init();

};

#endif