#include "ParticleEffect.h"

#include "EventManager.h"
#include "ParticleManager.h"
#include "RemoveObjectEvent.h"

ParticleEffect::ParticleEffect()
{
	Init();
}

ParticleEffect::~ParticleEffect()
{
	SetBatch(0);
}

void ParticleEffect::Init()
{
	m_bActive	= true;
	m_pBatch	= 0;
}

void ParticleEffect::Refresh(const float &fDeltaTime)
{
	RemoveIfInactive();

	if (!m_pBatch) 
	{
		return;
	}

	m_bActive = false;
	
	for (int i = 0; i < NUM_PARTICLES_PER_BATCH; i++)
	{
		particle &part = m_pBatch->parts[i];

		if (part.fEnergy > 0.0f) 
		{ //update active particles
			part.Update(fDeltaTime);
			m_bActive = true;
		}
	}
}

void ParticleEffect::RemoveIfInactive()
{
	if (!m_bActive)
	{
		RemoveObjectEvent *pNewRemoveEvent = new RemoveObjectEvent();
		pNewRemoveEvent->SetObject(this);
		gEventManager()->AddEvent(pNewRemoveEvent);
		pNewRemoveEvent->Release();
	}
}

void ParticleEffect::SetBatch(batch *pBatch)
{
	if (m_pBatch != pBatch)
	{
		if (m_pBatch)
		{
			gParticleManager()->ReturnBatch(m_pBatch);
			m_pBatch = 0;
		}

		if (pBatch)
		{
			m_pBatch = pBatch;
		}
	}
}