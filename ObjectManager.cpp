#include "ObjectManager.h"

#include "Game.h"
#include "Node.h"
#include "Object.h"
#include "Scene.h"

#include <GL/glut.h>

ObjectManager *gObjectManager()
{
	static ObjectManager theObjectManager;
	return &theObjectManager;
}

ObjectManager::ObjectManager()
{
	Init();
}

ObjectManager::~ObjectManager()
{
}

//void ObjectManager::CheckObjectBoundaries()
//{ //objects are considered out of bounds if they are more than 1 gamesize off the screen!
//	static int dwLastChecked = 0;
//	dwLastChecked++;
//	if (dwLastChecked % 25 != 0)
//	{ //don't always do this check
//		return;
//	}
//
//	list<Object *>::iterator iter;
//	for (iter = m_vObjects.begin(); iter != m_vObjects.end(); iter++)
//	{
//		Object *pObject = *iter;
//		if (pObject && pObject->GetNode())
//		{
//			Node *pNode = pObject->GetNode();
//
//			float fDistanceSquared	= pNode->m_PosQuat.pos.x * pNode->m_PosQuat.pos.x
//									+ pNode->m_PosQuat.pos.y * pNode->m_PosQuat.pos.y;
//
//			if (fDistanceSquared > BOUNDARY_DISTANCE_SQUARED)
//			{
//				RemoveObjectEvent *pNewRemoveEvent = new RemoveObjectEvent();
//				pNewRemoveEvent->SetObject(pObject);
//				gEventManager()->AddEvent(pNewRemoveEvent);
//				pNewRemoveEvent->Release();				
//			}
//		}
//	}
//}

void ObjectManager::Cleanup()
{

}

void ObjectManager::DebugInfoRender(const float &fRatio, const PosQuat &pq)
{
	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);

	GLboolean bLightingWasOn = glIsEnabled(GL_LIGHTING);
	if (bLightingWasOn)
	{ //turn off lighting
		glDisable(GL_LIGHTING);
	}
	
	GLboolean bTextureWasOn = glIsEnabled(GL_TEXTURE_2D);
	if (bTextureWasOn)
	{ //turn off textures
		glDisable(GL_TEXTURE_2D);
	}

	//reset matrices so text is always in the correct spot
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, fRatio, 0.1, 50.0); //update this so we dont get crappy distortion
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 1.75, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	Scene *pScene = gGame()->GetCurrentScene();

	const size_t size = 60;
	char text[size];
	memset(text, 0, size);

	sprintf_s(text, size, "ObjectManager Summary: %d Objects Total.", pScene->GetNumObjects());
	glRasterPos2f(fRatio, 0.97f);
	for (const char *c = text; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
	}

	list<Object *>::const_iterator iter = pScene->GetObjectIteratorBegin();
	list<Object *>::const_iterator end = pScene->GetObjectIteratorEnd();
	Object *pObj = 0;
	Node *pNode = 0;
	int count = 0;
	int numPerColumn = 50;
	int row = 0;
	int column = 0;

	while (iter != end && count < numPerColumn)
	{
		row = count % numPerColumn;
		column = count / numPerColumn;
		pObj = *iter;
		pNode = pObj->GetNode();
		iter++;

		if (pNode)
		{
			sprintf_s(text, size, "0x%x [%s] @ (%.1f, %.1f, %.1f)", pObj, pObj->GetClassName(), 
				pNode->m_PosQuat.pos.x, pNode->m_PosQuat.pos.y, pNode->m_PosQuat.pos.z);
		}
		else
		{
			sprintf_s(text, size, "0x%x [%s] @ (NO NODE)", pObj, pObj->GetClassName());
		}

		glRasterPos2f(fRatio, 0.9f - 0.04f * row);
		for (const char *c = text; *c != '\0'; c++)
		{
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
		}

		count++;
	}


	if (bTextureWasOn)
	{ //re-enable textures if it was on initially
		glEnable(GL_TEXTURE_2D);
	}

	if (bLightingWasOn)
	{ //if it was on, turn it back on!
		glEnable(GL_LIGHTING);
	}
}

void ObjectManager::DebugScreenRender()
{

}

void ObjectManager::Init()
{

}

void ObjectManager::Print()
{
	printf("ObjectManager Summary\n");

	//unsigned int dwPadding	= 0;
	//unsigned int dwLength	= 0;
	//unsigned int dwSpace	= 25;

	//list<Object *>::iterator iter;
	//for (iter = m_vObjects.begin(); iter != m_vObjects.end(); iter++)
	//{
	//	dwLength = (unsigned int)strlen((*iter)->GetClassName());
	//	if (dwLength >= dwSpace)
	//	{
	//		dwPadding = 1;
	//	}
	//	else
	//	{
	//		dwPadding = dwSpace - dwLength;
	//	}
	//	
	//	printf("  %s%*s 0x%p  RefCount %u\n", (*iter)->GetClassName(), dwPadding, "", *iter, (*iter)->GetRefCount());
	//}
}

void ObjectManager::Refresh(const float &fDeltaTime)
{

}

//void ObjectManager::RemoveObject(Object *pRemove)
//{
//	if (pRemove)
//	{
//		list<Object *>::iterator iter;
//		for (iter = m_vObjects.begin(); iter != m_vObjects.end(); iter++)
//		{
//			if ((*iter) == pRemove)
//			{
//				//found the unknown to remove
//				(*iter)->Release();
//				m_vObjects.erase(iter);
//				break;
//			}
//		}
//	}
//}

void ObjectManager::SelectNext()
{

}

void ObjectManager::SelectPrevious()
{

}