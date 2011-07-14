#include "HumanFiringPlugin.h"

#include "def.h"
#include "Input.h"
#include "PhysicalObject.h"
#include "PlayerShip.h"

#include <iostream>
using namespace std;

HumanFiringPlugin::HumanFiringPlugin()
{
	Init();
}

HumanFiringPlugin::~HumanFiringPlugin()
{
	ClearControlledObjects();
}

//Do NOT add ref the objects they control!  
void HumanFiringPlugin::AddControlledObject(PhysicalObject *pNewObject)
{
	if (pNewObject)
	{
		m_vControlledObjects.push_back(pNewObject);
	}
}

void HumanFiringPlugin::AssignFiringTask(PhysicalObject *pObject)
{
	//HumanMovementPlugins should NEVER be assigning firing tasks to physical objects!  
	printf("Big Problem (tm)...  HumanFiringPlugin::AssignFiringTask() . . .\n");
}

void HumanFiringPlugin::ClearControlledObjects()
{
	m_vControlledObjects.clear();
}

void HumanFiringPlugin::FromBlock(Block *pBlock)
{

}

HumanFiringPlugin *HumanFiringPlugin::GetPlugin()
{
	static HumanFiringPlugin *pPlugin = new HumanFiringPlugin();
	return pPlugin;
}

void HumanFiringPlugin::Init()
{

}

void HumanFiringPlugin::Print(int indent) const
{
	printf("%*sHumanFiringPlugin 0x%p (%s)\n", indent, "", this);
}

void HumanFiringPlugin::Refresh(const float &fDeltaTime)
{
	if (m_vControlledObjects.size() > 0)
	{
		int			dwAlphaInput			= gInput()->GetKeyboardAlphaFlags();

		//HANDLE FIRING
		if (dwAlphaInput & INPUT_SPACE)
		{
			vector<PhysicalObject *>::iterator	iter;
			PhysicalObject						*pObject		= 0;
			for (iter = m_vControlledObjects.begin(); iter != m_vControlledObjects.end(); iter++)
			{
				pObject = (*iter);
				if (pObject && pObject->IsAlive())
				{ //always fire weapon one for now...
					pObject->FireWeaponOne();
				}
			}			
		}
	}	
}

void HumanFiringPlugin::RemoveControlledObject(PhysicalObject *pRemove)
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

void HumanFiringPlugin::SignalRemoved(PhysicalObject *pObject)
{
	RemoveControlledObject(pObject);
}

Block *HumanFiringPlugin::ToBlock()
{
	return 0;
}