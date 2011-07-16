#ifndef _SECTOR_
#define _SECTOR_

#include "Game.h"
#include "Object.h"

#include <list>
#include <string>
using namespace std;

class ComputerShip;
class GalaxyEffect;
class Scene;
class Spawner;

class Sector : public Object
{
public:
	Sector();
	Sector(int dwLevel);

	void								End();
	virtual void						FromBlock(Block *pBlock);
	virtual const char					*GetClassName() const				{ return "Sector"; }
	GalaxyEffect						*GetGalaxyEffect()					{ return m_pGalaxyEffect; }
	int									GetLevel() const					{ return m_dwLevel; }
	Scene								*GetScene()							{ return m_pScene; }
	const char							*GetSectorName() const				{ return m_sSectorName.c_str(); }
	float								GetSectorProgressPercent() const;
	ComputerShip						*GetShip(unsigned int dwIndex);

	void								Pause();
	void								Play();
	virtual void						Print(int indent) const;
	virtual void						Refresh(const float &fDeltaTime);
	void								SetGalaxyEffect(GalaxyEffect *pGalaxyEffect);
	void								SetScene(Scene *pScene);
	void								SetSectorName(string newName)		{ m_sSectorName = newName; }

	virtual Block						*ToBlock();

protected:
	virtual ~Sector();

	void								Clear();
	void								Generate(int dwLevel);

	bool								m_bPlaying;
	int									m_dwLevel;
	string								m_sSectorName;

	list<Spawner *>						m_Spawners;
	ComputerShip						*m_ComputerShips[NUM_SHIP_TYPES_PER_SECTOR];
	//list<BossShip *>					m_vBossShips;   //Eventually  :P

	GalaxyEffect						*m_pGalaxyEffect;
	Scene								*m_pScene;		//Scene owner

private:

	void								ClearSpawners();
	void								Init();

};

#endif