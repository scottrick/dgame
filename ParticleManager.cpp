#include "ParticleManager.h"

ParticleManager *gParticleManager()
{
	static ParticleManager theParticleManager;
	return &theParticleManager;
}

ParticleManager::ParticleManager()
{
	Init();
}

ParticleManager::~ParticleManager()
{
	Cleanup();
}

void ParticleManager::Cleanup()
{
	list<batch *>::iterator		iter;
	batch						*pBatch		= 0;

	for (iter = m_lBatches.begin(); iter != m_lBatches.end(); iter++)
	{
		pBatch = *iter;
		if (pBatch)
		{
			delete pBatch;
			pBatch = 0;
		}
	}

	m_lBatches.clear();
	m_lUnusedBatches.clear();
}

void ParticleManager::DebugInfoRender(const float &fRatio, const PosQuat &pq)
{

}

void ParticleManager::DebugScreenRender()
{

}

void ParticleManager::Init()
{
	batch *pNewBatch = 0;

	for (int i = 0; i < NUM_PARTICLE_BATCHES; ++i)
	{
		pNewBatch = new batch();
		m_lBatches.push_back(pNewBatch);
		m_lUnusedBatches.push_back(pNewBatch);
	}	
}

void ParticleManager::Refresh(const float &fDeltaTime)
{

}

batch *ParticleManager::RequestBatch()
{
	if (m_lUnusedBatches.size() > 0)
	{
		batch *pNewBatch = *(m_lUnusedBatches.begin());
		m_lUnusedBatches.erase(m_lUnusedBatches.begin());
		return pNewBatch;
	}
	else
	{ //no more batches available...
		return 0;
	}
}

void ParticleManager::ResetBatch(batch *pBatch)
{
	if (pBatch)
	{		
		for (int i = 0; i < NUM_PARTICLES_PER_BATCH; i++)
		{
			particle &part = pBatch->parts[i];

			part.vPrevPos.x			= 0.0f;
			part.vPrevPos.y			= 0.0f;
			part.vPrevPos.z			= 0.0f;
			part.vVelocity.x		= 0.0f;
			part.vVelocity.y		= 0.0f;
			part.vVelocity.z		= 0.0f;
			part.vAcceleration.x	= 0.0f;
			part.vAcceleration.y	= 0.0f;
			part.vAcceleration.z	= 0.0f;
			part.vColor.x			= 1.0f;
			part.vColor.y			= 1.0f;
			part.vColor.z			= 1.0f;
			PosQuat newPQ;
			part.pq					= newPQ;
			part.fScale				= 1.0f;
			part.fEnergy			= 0;
			part.fMaxEnergy			= 0;
			part.fSecondsPerRot		= 1.0f;
		}	
	}
}

void ParticleManager::ReturnBatch(batch *pBatch)
{
	if (pBatch)
	{
		m_lUnusedBatches.push_back(pBatch);
		ResetBatch(pBatch);		
	}
}

void ParticleManager::SelectNext()
{

}

void ParticleManager::SelectPrevious()
{

}

void particle::Update(const float &fDeltaTime)
{
	float fDecay		= 0.9f * fDeltaTime;
	this->vPrevPos		= this->pq.pos;							//save old position

	this->vVelocity.x	-= this->vVelocity.x * fDecay;
	this->vVelocity.y	-= this->vVelocity.y * fDecay;
	this->vVelocity.z	-= this->vVelocity.z * fDecay;
	this->vVelocity.x	+= this->vAcceleration.x * fDeltaTime;	//update velocity
	this->vVelocity.y	+= this->vAcceleration.y * fDeltaTime;
	this->vVelocity.z	+= this->vAcceleration.z * fDeltaTime;
	
	this->pq.pos.x		+= this->vVelocity.x * fDeltaTime;		//update position
	this->pq.pos.y		+= this->vVelocity.y * fDeltaTime;
	this->pq.pos.z		+= this->vVelocity.z * fDeltaTime;

	if (fSecondsPerRot != 0.0f)
	{
		float			fPercentOfFullRotation	= fDeltaTime / fSecondsPerRot;
		Quat			*quat					= &(this->pq.quat);
		Quat			rotation;
		
		rotation.CreateFromRotationRADIANS(2.0f * PI * fPercentOfFullRotation, 0.0f, 0.0f, 1.0f);
		*quat = rotation * *quat;		
	}

	this->fEnergy		-= fDeltaTime;
}