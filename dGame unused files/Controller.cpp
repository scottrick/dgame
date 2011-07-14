#include "Controller.h"

#include "PhysicalObject.h"

Controller::Controller()
{
	Init();
}

Controller::~Controller()
{
	ClearControlledObjects();	
}

void Controller::AddControlledObject(PhysicalObject *pNewObject)
{
	if (pNewObject)
	{
		m_vControlledObjects.push_back(pNewObject);
	}
}

void Controller::ClearControlledObjects()
{
	m_vControlledObjects.clear();
}

void Controller::Init()
{
	SetObjectType(Object::eTYPE_CONTROLLER);
}

void Controller::RemoveControlledObject(PhysicalObject *pRemove)
{
	if (pRemove)
	{
		vector<PhysicalObject *>::iterator iter = m_vControlledObjects.begin();
		while (iter != m_vControlledObjects.end())
		{
			if (*iter == pRemove)
			{
				iter = m_vControlledObjects.erase(iter);	//remove it from the list!
			}
			else
			{
				iter++;
			}
		}
	}
}