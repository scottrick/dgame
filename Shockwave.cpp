#include "Shockwave.h"

#include "ActionScene.h"
#include "Camera.h"
#include "EventManager.h"
#include "Game.h"
#include "Node.h"
#include "PhysicalObject.h"
#include "RemoveObjectEvent.h"
#include "Scene.h"

#include <GL/glut.h>

Shockwave::Shockwave(const float &fMaxIntensity, const float &fRate, const PosQuat &pq)
{
	m_fCurrentIntensity		= 0.0f;
	m_fMaxIntensity			= fMaxIntensity;
	m_fRate					= fRate;
	m_PosQuat				= pq;
}

Shockwave::Shockwave(PhysicalObject *pCreateFromThisObject, bool bMega)
{
	Node *pNode			= pCreateFromThisObject->GetNode();

	m_fCurrentIntensity = pNode->GetScale().x / 2.0f;
	m_fMaxIntensity		= pNode->GetScale().x / 2.0f + 1.0f;
	m_fRate				= 7.0f;
	m_PosQuat			= pNode->m_PosQuat;

	switch (pCreateFromThisObject->GetPhysicalObjectType())
	{
	case PhysicalObject::ePROJECTILE:
		m_fMaxIntensity *= 0.5f;
		break;	
	case PhysicalObject::eOBSTACLE:
		m_fMaxIntensity *= 5.0f;
		//m_fRate			*= 2.0f;
		break;
	case PhysicalObject::ePLAYER_SHIP:
		bMega = true;
		break;
	case PhysicalObject::eCOMPUTER_SHIP:
		m_fMaxIntensity		*= 2.5f;
		//m_fRate				*= 2.0f;
		//m_fMaxIntensity	*= 1.5f;
		//m_fRate			*= 1.5f;
		//bMega = true;
	default:	
		//nothing special..
		break;
	}


	if (bMega)
	{
		m_fMaxIntensity			*= 10.0f;
		//m_fRate					*= 3.0f;
		m_fCurrentIntensity		= 0.1f;
	}

	min(m_fMaxIntensity, gGame()->GetActionScene()->GetCamera()->GetPosQuat().pos.z - 0.01f);
}

Shockwave::~Shockwave()
{	

}

void Shockwave::FromBlock(Block *pBlock)
{
	
}	

void Shockwave::PostRender() const
{
	glPushMatrix();
	glTranslatef(m_PosQuat.pos.x, m_PosQuat.pos.y, m_PosQuat.pos.z);
	glColor4f(m_fMaxIntensity / 100.0f, 0.0f, 0.0f, 0.0f);
	glutSolidSphere(m_fCurrentIntensity, 50, 50);
	glPopMatrix();
}

void Shockwave::Refresh(const float &fDeltaTime)
{
	if (m_fCurrentIntensity > m_fMaxIntensity)
	{ //wave finished, so remove it
		RemoveObjectEvent *pNewRemoveEvent = new RemoveObjectEvent();
		pNewRemoveEvent->SetObject(this);
		gEventManager()->AddEvent(pNewRemoveEvent);
		pNewRemoveEvent->Release();
		return; 
	}

	m_fCurrentIntensity += m_fRate * fDeltaTime;
}

Block *Shockwave::ToBlock()
{
	return 0;
}