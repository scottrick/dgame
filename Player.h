#ifndef _PLAYER_
#define _PLAYER_

#include "Object.h"

#include <list>
using namespace std;

class PhysicalObject;
class Sector;

class Player : public Object
{
public:
	Player();

	virtual void						FromBlock(Block *pBlock);
	virtual const char					*GetClassName() const			{ return "Player"; }
	PhysicalObject						*GetPlayerObject() const		{ return m_pPlayerObject; }
	Sector								*GetSector(unsigned int dwIndex) const;
	const list<Sector *>				*GetSectors() const				{ return &m_lSectors; }
	Sector								*GetTargetSector() const		{ return m_pTargetSector; }


	virtual void						Print(int indent) const;
	virtual void						Refresh(const float &fDeltaTime);
	void								SetPlayerObject(PhysicalObject *pObject);
	void								SetTargetSector(Sector *pSector);
	virtual Block						*ToBlock();

protected:
	virtual ~Player();

	void								ClearSectors();

	list<Sector *>						m_lSectors;

	PhysicalObject						*m_pPlayerObject;
	Sector								*m_pTargetSector;

private:

	void								Init();

};

#endif