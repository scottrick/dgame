#include "TestFireEvent.h"

#include "Game.h"
#include "Projectile.h"
#include "Scene.h"

#include <iostream>
using namespace std;

TestFireEvent::TestFireEvent()
{
	Init();
}

TestFireEvent::~TestFireEvent()
{
	SetProjectile(0);
}

void TestFireEvent::Execute()
{
	//printf("executing test fire event!!\n");
	gGame()->GetCurrentScene()->AddObject(m_pProjectile);
	SetProjectile(0);
}

void TestFireEvent::Init()
{
	m_pProjectile = 0;
}

void TestFireEvent::Print(int indent) const
{
	
}

void TestFireEvent::SetProjectile(Projectile *pProjectile)
{
	if (pProjectile != m_pProjectile)
	{
		if (m_pProjectile)
		{
			m_pProjectile->Release();
			m_pProjectile = 0;
		}

		if (pProjectile)
		{
			m_pProjectile = pProjectile;
			m_pProjectile->AddRef();
		}
	}
}