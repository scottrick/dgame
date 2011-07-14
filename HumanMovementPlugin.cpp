#include "HumanMovementPlugin.h"

#include "def.h"
#include "Input.h"
#include "PhysicalObject.h"

#include <iostream>
using namespace std;

HumanMovementPlugin::HumanMovementPlugin()
{
	Init();
}

HumanMovementPlugin::~HumanMovementPlugin()
{
	ClearControlledObjects();
}

//Do NOT add ref the objects they control!  
void HumanMovementPlugin::AddControlledObject(PhysicalObject *pNewObject)
{
	if (pNewObject)
	{
		m_vControlledObjects.push_back(pNewObject);
	}
}

void HumanMovementPlugin::AssignMovementTask(PhysicalObject *pObject)
{
	//HumanMovementPlugins should NEVER be assigning movement tasks to physical objects!  
	printf("Big Problem (tm)...  HumanMovementPlugin::AssignMovementTask() . . .\n");
}

void HumanMovementPlugin::ClearControlledObjects()
{
	m_vControlledObjects.clear();
}

void HumanMovementPlugin::FromBlock(Block *pBlock)
{

}

HumanMovementPlugin *HumanMovementPlugin::GetPlugin()
{
	static HumanMovementPlugin *pPlugin = new HumanMovementPlugin();
	return pPlugin;
}

void HumanMovementPlugin::Init()
{
	m_fAcceleration		= 6.0f;
	m_fSpeed			= 6.0f;
}

void HumanMovementPlugin::Print(int indent) const
{
	printf("%*sHumanMovementPlugin 0x%p (%s)\n", indent, "", this);
}

void HumanMovementPlugin::Refresh(const float &fDeltaTime)
{
	if (m_vControlledObjects.size() > 0)
	{
		int			dwAlphaInput			= gInput()->GetKeyboardAlphaFlags();

		VECTOR3 vVelocity;
		
		if (dwAlphaInput & INPUT_CHAR_W)
		{
			vVelocity.y				+= 1.0f;
		}

		if (dwAlphaInput & INPUT_CHAR_S)
		{
			vVelocity.y				-= 1.0f;
		}

		if (dwAlphaInput & INPUT_CHAR_D)
		{
			vVelocity.x				+= 1.0f;
		}

		if (dwAlphaInput & INPUT_CHAR_A)
		{
			vVelocity.x				-= 1.0f;
		}

		float	fMagnitudeSquared	= MagnitudeSquaredVECTOR3(vVelocity);
		bool	bNoChange			= false;

		if (fMagnitudeSquared > 0.0f)
		{
			NormalizeVECTOR3(vVelocity);

			vVelocity.x		*= m_fSpeed;
			vVelocity.y		*= m_fSpeed;
			vVelocity.z		*= m_fSpeed;
		}
		else if (fMagnitudeSquared == 0.0f)
		{
			bNoChange		= true;	
		}

		vector<PhysicalObject *>::iterator			iter;
		PhysicalObject								*pPhysObject			= 0;
		for (iter = m_vControlledObjects.begin(); iter != m_vControlledObjects.end(); iter++)
		{
			pPhysObject = *iter;
			if (pPhysObject)
			{
				if (bNoChange)
				{
					vVelocity.x *= 0.0f;
					vVelocity.y *= 0.0f;
					vVelocity.z *= 0.0f;
				}
				
				pPhysObject->SetVelocity(vVelocity);
			}
		}
	}
}

void HumanMovementPlugin::RemoveControlledObject(PhysicalObject *pRemove)
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

void HumanMovementPlugin::SignalRemoved(PhysicalObject *pObject)
{
	RemoveControlledObject(pObject);
}

Block *HumanMovementPlugin::ToBlock()
{
	return 0;
}