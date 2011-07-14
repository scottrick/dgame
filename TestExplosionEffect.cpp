#include "TestExplosionEffect.h"

#include "EventManager.h"
#include "Node.h"
#include "PhysicalObject.h"
#include "RemoveObjectEvent.h"
#include "utility.h"

TestExplosionEffect::TestExplosionEffect()
{
	Init();
}

TestExplosionEffect::~TestExplosionEffect()
{

}

void TestExplosionEffect::CreateFromPhysicalObject(PhysicalObject *pPhysObject)
{
	if (pPhysObject && pPhysObject->GetNode())
	{
		m_pNode->m_vScale.x = (pPhysObject->GetNode()->m_vScale.x * 0.5f) * (GenerateRandomFloat(0.2f) + 0.9f);
		m_pNode->m_vScale.y = pPhysObject->GetNode()->m_vScale.y * 0.5f * (GenerateRandomFloat(0.2f) + 0.9f);
		m_pNode->m_vScale.z = pPhysObject->GetNode()->m_vScale.z * 0.5f * (GenerateRandomFloat(0.2f) + 0.9f);
		m_pNode->m_PosQuat.pos.x = pPhysObject->GetNode()->m_PosQuat.pos.x;
		m_pNode->m_PosQuat.pos.y = pPhysObject->GetNode()->m_PosQuat.pos.y;
		m_pNode->m_PosQuat.pos.z = pPhysObject->GetNode()->m_PosQuat.pos.z;
	}
}

void TestExplosionEffect::FromBlock(Block *pBlock)
{

}

void TestExplosionEffect::Init()
{
	m_fCurrentAge		= 0.0f;
	m_fDuration			= GenerateRandomFloat(0.3f) + 0.3f;
	m_fRotation			= GenerateRandomFloat(6.0f * PI) - 3.0f * PI;

	Node *pNewNode		= new Node();
	pNewNode->SetVisual("data\\visuals\\test_explosion.3ds");
	SetNode(pNewNode);
	pNewNode->Release();
}

void TestExplosionEffect::Refresh(const float &fDeltaTime)
{
	if (m_pNode)
	{
		m_fCurrentAge		+= fDeltaTime;

		//update rotation
		Quat *quat = &(m_pNode->m_PosQuat.quat);
		Quat rotation;
		VECTOR3 axis; 	axis.x = 0.0f;	axis.y = 0.0f;	axis.z = 1.0f;
		rotation.CreateFromRotationRADIANS(m_fRotation * fDeltaTime, axis.x, axis.y, axis.z);
		*quat = rotation * *quat;
		
		if (m_fCurrentAge < m_fDuration)
		{ //not dead yet, so scale appropriately
			if (m_fCurrentAge < (0.3f * m_fDuration))
			{
				float fExplosionGrowth = EXPLOSION_GROW_SPEED * fDeltaTime;

				m_pNode->m_vScale.x *= fExplosionGrowth + 1.0f;
				m_pNode->m_vScale.y *= fExplosionGrowth + 1.0f;
				m_pNode->m_vScale.z *= fExplosionGrowth + 1.0f;
			}
			else 
			{
				float fExplosionGrowth = 1.0f - (EXPLOSION_SHRINK_SPEED * fDeltaTime);
				if (fExplosionGrowth <= 0.0f)
				{
					fExplosionGrowth = 0.0f;
				}

				m_pNode->m_vScale.x *= fExplosionGrowth;
				m_pNode->m_vScale.y *= fExplosionGrowth;
				m_pNode->m_vScale.z *= fExplosionGrowth;
			}
		}
		else
		{ //dead! 
			RemoveObjectEvent *pNewRemoveEvent = new RemoveObjectEvent();
			pNewRemoveEvent->SetObject(this);
			gEventManager()->AddEvent(pNewRemoveEvent);
			pNewRemoveEvent->Release();
		}
	}
}

Block *TestExplosionEffect::ToBlock()
{
	return 0;
}