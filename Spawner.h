#ifndef _SPAWNER_
#define _SPAWNER_

#include "Object.h"

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

	void						Randomize();

	void						SetAverageDelay(float fAverageDelay);
	void						SetObjectToSpawn(PhysicalObject *pObject);
	void						SetRepeating(bool bRepeating);
	void						SetSector(Sector *pSector);
	Block						*ToBlock();

protected:
	~Spawner();

	bool				m_bRepeating;
	int					m_dwNumRemaining;
	float				m_fAverageDelay;
	float				m_fCurrentDelay;
	float				m_fDelay;
	float				m_fXLoc;
	float				m_fXChange;
	PhysicalObject		*m_pObjectToSpawn;
	Sector				*m_pSector;

private:
	Spawner();

	void				Init();

};

#endif
