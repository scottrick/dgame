#include "Object.h"

#include "Node.h"

#include <iostream>
using namespace std;

Object::Object()
{
	Init();
}

Object::~Object()
{
	SetNode(0);
}

void Object::Clone(Object *pClone)
{
	SetObjectType(pClone->GetObjectType());

	if (pClone->GetNode())
	{
		Node *pNewNode = new Node(); //Need to make a new node so the new object's visual can have a unique location
		pNewNode->SetVisual(pClone->GetNode()->GetVisual());
		pNewNode->SetScale(pClone->GetNode()->GetScale());
		pNewNode->m_PosQuat = pClone->GetNode()->m_PosQuat;
		SetNode(pNewNode); 
		pNewNode->Release();
	}
}

void Object::Init()
{
	m_pNode				= 0;
	m_pScene			= 0;
	m_dwObjectType		= Object::eTYPE_UNKNOWN;
}

void Object::Print(int indent) const
{
	printf("%*sObject (%s) 0x%p\n", indent, " ", GetClassName(), this);
	if (m_pNode)
	{
		m_pNode->Print(indent + 2);
	}
}

void Object::SetNode(Node *pNode)
{
	if (pNode != m_pNode)
	{
		if (m_pNode)
		{
			m_pNode->Release();
			m_pNode = 0;
		}

		if (pNode)
		{
			pNode->AddRef();
			m_pNode = pNode;
		}
	}
}

void Object::SetObjectType(Object::ObjectType dwType)
{
	m_dwObjectType = dwType;
}