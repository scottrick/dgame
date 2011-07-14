#ifndef _COMPUTER_SHIP_
#define _COMPUTER_SHIP_

#include "PhysicalObject.h"

#include <string>
using namespace std;

class Block;
class CommandSegment;
class EngineeringSegment;
class FiringTask;
class MovementTask;
class PropulsionSegment;
class Segment;

class ComputerShip : public PhysicalObject
{
public:
	ComputerShip();

	void								Clone(ComputerShip *pClone);
	void								Collide(PhysicalObject *pOtherObject);
	PhysicalObject						*Duplicate();
	void								FromBlock(Block *pBlock);
	void								Generate(int dwLevel);
	virtual const char					*GetClassName() const				{ return "ComputerShip"; }
	CommandSegment						*GetCommandSegment() const			{ return m_pCommandSegment; }
	virtual VECTOR3						GetDebugColor() const				{ return VECTOR3(1.0f, 0.0f, 0.0f); }
	EngineeringSegment					*GetEngineeringSegment() const		{ return m_pEngineeringSegment; }
	virtual FiringPlugin				*GetFiringPlugin() const;
	virtual MovementPlugin				*GetMovementPlugin() const;
	PropulsionSegment					*GetPropulsionSegment() const		{ return m_pPropulsionSegment; }
	string								GetShipName() const					{ return m_sShipName; }
	virtual Shockwave					*GetShockwave();
	
	virtual void						Print(int indent) const;
	virtual void						Refresh(const float &fDeltaTime);
	void								SetCommandSegment(CommandSegment *pSegment);
	void								SetEngineeringSegment(EngineeringSegment *pSegment);
	virtual void						SetFiringPlugin(FiringPlugin *pPlugin);
	virtual void						SetMovementPlugin(MovementPlugin *pPlugin);
	void								SetPropulsionSegment(PropulsionSegment *pSegment);
	void								SetShipName(string sNewName)		{ m_sShipName = sNewName; }
	Block								*ToBlock();

protected:
	virtual ~ComputerShip();

	virtual void						OnDeath();

private:
	void								Init();
		
	CommandSegment						*m_pCommandSegment;
	EngineeringSegment					*m_pEngineeringSegment;
	PropulsionSegment					*m_pPropulsionSegment;

	string								m_sShipName;
};

#endif
