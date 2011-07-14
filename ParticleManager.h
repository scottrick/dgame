#ifndef _PARTICLE_MANAGER_
#define _PARTICLE_MANAGER_

#include "def.h"
#include "Manager.h"
#include "PosQuat.h"

#include <list>
#include <vector>
using namespace std;

#define NUM_PARTICLE_BATCHES		100
#define NUM_PARTICLES_PER_BATCH		32

struct particle 
{
	VECTOR3			vPrevPos, vVelocity, vAcceleration;
	VECTOR4			vColor;
	PosQuat			pq;
	float			fScale, fEnergy, fMaxEnergy, fSecondsPerRot;

	void Update(const float &fDeltaTime);
};

struct batch
{ //particles are doled out in batches so we're not doing 40 add/removes from lists everytime...
	particle parts[NUM_PARTICLES_PER_BATCH];
};

class ParticleManager : public Manager
{
public:
	ParticleManager();
	~ParticleManager();

	void						Cleanup();
	void						DebugInfoRender(const float &fRatio, const PosQuat &pq);
	void						DebugScreenRender();
	const char					*GetManagerName()					{ return "ParticleManager"; }
	void						Refresh(const float &fDeltaTime);
	batch						*RequestBatch();
	void						ReturnBatch(batch *pBatch);
	void						SelectNext();
	void						SelectPrevious();

protected:

	list<batch *>				m_lBatches;				//list of all batches which exist
	list<batch *>				m_lUnusedBatches;		//list of all unused batches

private:
	void						Init();
	void						ResetBatch(batch *pBatch);

};

extern ParticleManager *gParticleManager();

#endif