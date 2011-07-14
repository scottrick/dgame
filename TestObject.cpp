#include "TestObject.h"

#include "Node.h"
#include "TextNode.h"

#include <iostream>
#include <cmath>
using namespace std;

TestObject::TestObject()
{
	Init();
}

TestObject::~TestObject()
{

}

void TestObject::Init()
{
	SetObjectType(Object::eTYPE_TEST_OBJECT);

	bXUp				= true;
	bXUp				= true;
	fRotation			= 0.0f;

	Node *pNewNode		= new TextNode("HatfatText49");
	pNewNode->m_PosQuat.pos.x -= 9.0f;
	pNewNode->m_PosQuat.pos.y += 2.0f;
	pNewNode->m_vScale.x = 3.20f;
	pNewNode->m_vScale.y = 3.20f;
	SetNode(pNewNode);
	pNewNode->Release();
}

void TestObject::Refresh(const float &fDeltaTime)
{
	return; //no rotating for now

	VECTOR3 *pos = &(m_pNode->m_PosQuat.pos);
	Quat *quat = &(m_pNode->m_PosQuat.quat);

	goto hacky;

	//update the position with some crappy bouncing thing
	if (bXUp)
	{
		pos->x += 0.05f;

		if (pos->x > 2.0f)
		{
			bXUp = !bXUp;
		}
	}
	else
	{
		pos->x -= 0.05f;

		if (pos->x < -2.0f)
		{
			bXUp = !bXUp;
		}
	}

hacky:
	//update the rotation!
	//rotate by small amount every time
	Quat rotation;

	VECTOR3 axis;
	axis.x = 0.1f;
	axis.y = 1.0f;
	axis.z = 0.2f;
	//NormalizeVECTOR3(axis);

	rotation.CreateFromRotationRADIANS(0.02f, axis.x, axis.y, axis.z);
	*quat = rotation * *quat;
}