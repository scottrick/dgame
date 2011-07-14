#include "TestShockwaveThing.h"

#include "AddObjectEvent.h"
#include "Block.h"
#include "EventManager.h"
#include "PosQuat.h"
#include "Shockwave.h"
#include "utility.h"

TestShockwaveThing::TestShockwaveThing()
{
	m_fTimeTillNextThing = GenerateRandomFloat(2.0f) + 0.5f;
}

TestShockwaveThing::~TestShockwaveThing()
{

}

void TestShockwaveThing::FromBlock(Block *pBlock)
{

}

void TestShockwaveThing::Refresh(const float &fDeltaTime)
{
	m_fTimeTillNextThing -= fDeltaTime;

	if (m_fTimeTillNextThing <= 0.0f)
	{
		float fMaxIntensity = GenerateRandomFloat(5.0f) + 4.98f;
		float fRate			= GenerateRandomFloat(1.5f) + 0.4f;
		m_fTimeTillNextThing = fMaxIntensity / fRate;

		PosQuat pq;
		pq.pos.x			= GenerateRandomFloat(7.0f) - 3.5f;
		pq.pos.y			= GenerateRandomFloat(7.0f) - 3.5f;

		Shockwave *pShockwave = new Shockwave(fMaxIntensity, fRate, pq);

		AddObjectEvent *pNewAddEvent = new AddObjectEvent();
		pNewAddEvent->SetObject(pShockwave, this->GetScene());
		gEventManager()->AddEvent(pNewAddEvent);
		pNewAddEvent->Release();
		pShockwave->Release();
	}
}	

Block *TestShockwaveThing::ToBlock()
{
	return 0;
}