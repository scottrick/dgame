#ifndef _SPAWNER_
#define _SPAWNER_

#include "Object.h"

#include <list>

#define MAX_NUMBER_OF_COMPUTER_SHIPS	25

class Block;
class PhysicalObject;
class Sector;

class Spawner : public Object 
{
public:
	Spawner(Sector *pSector, PhysicalObject *pObject);		

	virtual void				FromBlock(Block *pBlock);
	void						Refresh(const float &fDeltaTime);
	virtual const char			*GetClassName() const				{ return "Spawner"; }

	unsigned int				GetNumRemaining() const				{ return m_dwNumRemainingAlive; }
	unsigned int				GetNumTotal() const					{ return m_dwNumTotal; }

	bool						IsFinished() const					{ return m_bFinished; }
	void						Randomize();

	void						SetAverageDelay(float fAverageDelay);
	void						SetObjectToSpawn(PhysicalObject *pObject);
	void						SetRepeating(bool bRepeating);
	void						SetSector(Sector *pSector);
	Block						*ToBlock();

protected:
	~Spawner();

	bool				m_bRepeating;
	bool				m_bFinished;
	unsigned int		m_dwNumRemainingInGroup;
	unsigned int		m_dwNumRemainingToSpawn;
	unsigned int		m_dwNumRemainingAlive;
	unsigned int		m_dwNumTotal;
	float				m_fAverageDelay;
	float				m_fCurrentDelay;
	float				m_fDelay;
	float				m_fXLoc;
	float				m_fXChange;
	PhysicalObject		*m_pObjectToSpawn;
	Sector				*m_pSector;

	list<PhysicalObject *> m_AliveObjects;

private:
	Spawner();

	void				ClearAliveObjects();
	void				Init();

};

#endif
