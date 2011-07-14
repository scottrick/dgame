#include "Text.h"

#include "TextNode.h"

Text::Text(const char *pText)
{
	m_pText = pText;
	Init();
	Setup(VECTOR3(1.0f, 1.0f, 1.0f)); //default WHITE color
}

Text::Text(const char *pText, VECTOR3 vColor)
{
	m_pText = pText;
	Init();
	Setup(vColor);
}

Text::~Text()
{
		
}	

void Text::FromBlock(Block *pBlock)
{
	
}

void Text::Init()
{
	SetObjectType(Object::eTYPE_TEXT_OBJECT);
	m_bRotate			= false;
}

void Text::Refresh(const float &fDeltaTime)
{
	if (m_bRotate)
	{
		//update the rotation for debug visual display
		Quat *quat = &(m_pNode->m_PosQuat.quat);
		Quat rotation;
		VECTOR3 axis; 	axis.x = 1.0f;	axis.y = 0.0f;	axis.z = 0.0f;
		rotation.CreateFromRotationRADIANS(fDeltaTime * 0.75f * PI, axis.x, axis.y, axis.z);
		
		//no rotation for now when working on the letters
		*quat = rotation * *quat;
	}
}

void Text::Setup(VECTOR3 vColor)
{
	Node *pNewNode		= new TextNode(m_pText, vColor);
	SetNode(pNewNode);
	pNewNode->Release();
}

Block *Text::ToBlock()
{
	return 0;
}