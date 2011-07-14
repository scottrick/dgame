#include "CollisionManager.h"

#include "Game.h"
#include "Node.h"
#include "Object.h"
//#include "ObjectManager.h"
#include "PhysicalObject.h"
#include "PosQuat.h"
#include "Projectile.h"
#include "Scene.h"

#include <GL/glut.h>
#include <iostream>
using namespace std;

CollisionManager *gCollisionManager()
{
	static CollisionManager theCollisionManager;
	return &theCollisionManager;
}

CollisionManager::CollisionManager()
{
	Init();
}

CollisionManager::~CollisionManager()
{
	Cleanup();
}

bool CollisionManager::AddPhysicalObjectToBucket(PhysicalObject *pObject, CollisionBucket *pBucket)
{
	if (pBucket && pObject)
	{
		pBucket->m_vContents.push_back(pObject);
		pObject->GetNode()->m_dwBucket = (unsigned int)pBucket->m_dwY * (unsigned int)GAME_SIZE_X + (unsigned int)pBucket->m_dwX;
		return true;
	}
	else
	{
		return false;
	}
}	

void CollisionManager::Cleanup()
{
	CollisionBucket *pBucket = 0;
	for (unsigned int i = 0; i < m_vBuckets.size(); ++i)
	{
		pBucket = m_vBuckets.at(i);
		if (pBucket)
		{
			delete pBucket;
			pBucket = 0;
		}
	}
	m_vBuckets.clear();
}

void CollisionManager::ClearBuckets()
{
	CollisionBucket *pBucket = 0;

	for (unsigned int i = 0; i < m_vBuckets.size(); ++i)
	{
		pBucket = m_vBuckets.at(i);
		if (pBucket)
		{
			pBucket->ClearContents();
			pBucket = 0;
		}
	}
}

void CollisionManager::DebugInfoRender(const float &fRatio, const PosQuat &pq)
{
	GLboolean bTextureWasOn = glIsEnabled(GL_TEXTURE_2D);
	GLboolean bLightingWasOn = glIsEnabled(GL_LIGHTING);
	
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D); //disable textures when rendering the text

	{
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		size_t size = 50;
		char *pDetails = (char *)malloc(sizeof(char) * size);
		memset(pDetails, 0, size);
		sprintf_s(pDetails, size, "Collision Manager, %d Buckets", m_vBuckets.size());

		glRasterPos2f(-0.33f, 0.97f);
		for (const char *c = pDetails; *c != '\0'; c++)
		{
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
		}
		free(pDetails);
	}

	CollisionBucket *pBucket = m_vBuckets.at(m_dwSelectedIndex);
	if (pBucket)
	{	
		{
			size_t size = 50;
			char *pDetails = (char *)malloc(sizeof(char) * size);
			memset(pDetails, 0, size);
			sprintf_s(pDetails, size, "Index %d, Bucket (%f, %f)", m_dwSelectedIndex, pBucket->m_dwX, pBucket->m_dwY);

			glRasterPos2f(-0.33f, 0.92f);
			for (const char *c = pDetails; *c != '\0'; c++)
			{
				glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
			}
			free(pDetails);
		}

		{
			size_t size = 50;
			char *pDetails = (char *)malloc(sizeof(char) * size);
			memset(pDetails, 0, size);
			sprintf_s(pDetails, size, "  Contents (%d PhysicalObjects)", pBucket->m_vContents.size());

			glRasterPos2f(-0.33f, 0.87f);
			for (const char *c = pDetails; *c != '\0'; c++)
			{
				glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
			}
			free(pDetails);
		}

		list<PhysicalObject *>::iterator	iter;
		unsigned int						i			= 0;
		for (iter = pBucket->m_vContents.begin();iter != pBucket->m_vContents.end(); iter++)
		{
			PhysicalObject *pObject = *iter;
			if (pObject)
			{
				VECTOR3 vColor = pObject->GetDebugColor();
				glColor3f(vColor.x, vColor.y, vColor.z);

				size_t size = 50;
				char *pDetails = (char *)malloc(sizeof(char) * size);
				memset(pDetails, 0, size);
				sprintf_s(pDetails, size, "    %s", pObject->GetClassName());

				glRasterPos2f(-0.33f, 0.82f - (0.05f * (float)i));
				for (const char *c = pDetails; *c != '\0'; c++)
				{
					glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
				}
				free(pDetails);
			}

			++i;
		}
	}		

	if (bTextureWasOn)
	{
		glEnable(GL_TEXTURE_2D);
	}
	if (bLightingWasOn)
	{
		glEnable(GL_LIGHTING);
	}	
}

void CollisionManager::DebugScreenRender()
{
	GLboolean bTextureWasOn = glIsEnabled(GL_TEXTURE_2D);
	GLboolean bLightingWasOn = glIsEnabled(GL_LIGHTING);
	
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D); //disable textures when rendering the text

	glColor3f(0.0f, 1.0f, 0.0f);

	float			fXStart				= -(GAME_SIZE_X / 2.0f);
	float			fYStart				= -(GAME_SIZE_Y / 2.0f);
	float			fNumX				= GAME_SIZE_X / m_fBucketSize;
	float			fNumY				= GAME_SIZE_Y / m_fBucketSize;

	glBegin(GL_LINES);

	for (float i = 0.0; i < fNumX; ++i)
	{
		glVertex3f(fXStart + i * m_fBucketSize, fYStart, 0.0f);
		glVertex3f(fXStart + i * m_fBucketSize, fYStart + GAME_SIZE_Y, 0.0f);
	}

	for (float i = 0.0f; i < fNumY; ++i)
	{
		glVertex3f(fXStart, fYStart + i * m_fBucketSize, 0.0f);
		glVertex3f(fXStart + GAME_SIZE_X, fYStart + i * m_fBucketSize, 0.0f);
	}
	
	glEnd();

	//highlight currently selected bucket
	CollisionBucket *pBucket = m_vBuckets.at(m_dwSelectedIndex);
	if (pBucket)
	{
		float fX = (float)pBucket->m_dwX - (GAME_SIZE_X / 2.0f);
		float fY = (float)pBucket->m_dwY - (GAME_SIZE_Y / 2.0f);

		glColor3f(0.95f, 0.95f, 0.95f);
		glBegin(GL_QUADS);
		glVertex3f(fX, fY, 0.0f);
		glVertex3f(fX + m_fBucketSize, fY, 0.0f);
		glVertex3f(fX + m_fBucketSize, fY + m_fBucketSize, 0.0f);
		glVertex3f(fX, fY + m_fBucketSize, 0.0f);
		glEnd();
	}

	if (bTextureWasOn)
	{
		glEnable(GL_TEXTURE_2D);
	}
	if (bLightingWasOn)
	{
		glEnable(GL_LIGHTING);
	}
}	

void CollisionManager::FindCollisions(CollisionBucket *pBucket)
{
	PhysicalObject *pObjectOne = 0;
	PhysicalObject *pObjectTwo = 0;

	//compare every object in the bucket to one another and create a collision object for any that are colliding
	list<PhysicalObject *>::iterator	iterI;
	unsigned int						i		= 0;
	for (iterI = pBucket->m_vContents.begin(); iterI != pBucket->m_vContents.end(); iterI++)
	{
		pObjectOne = *iterI;

		if (pObjectOne && pObjectOne->GetNode())
		{
			float fRadiusOne = pObjectOne->GetNode()->m_vScale.x;
			if (fRadiusOne < pObjectOne->GetNode()->m_vScale.y)
			{
				fRadiusOne = pObjectOne->GetNode()->m_vScale.y;
			}
			fRadiusOne = fRadiusOne / 2.0f;

			list<PhysicalObject *>::iterator	iterJ		= iterI; //iterJ is one further than 
			for (iterJ++; iterJ != pBucket->m_vContents.end(); iterJ++)
			{
				pObjectTwo = *iterJ;

				if ((pObjectTwo->GetPhysicalObjectType() == PhysicalObject::eCOMPUTER_SHIP) && (pObjectOne->GetPhysicalObjectType() == PhysicalObject::eCOMPUTER_SHIP))
				{ //computer ships can't collide, so don't check!
					//dont check....
				}
				else if ((pObjectTwo->GetPhysicalObjectType() == PhysicalObject::ePROJECTILE) && (pObjectOne->GetPhysicalObjectType() == PhysicalObject::ePROJECTILE))
				{ //projectiles don't collide.. so don't check

				}
				else if ((pObjectTwo->GetPhysicalObjectType() == PhysicalObject::ePROJECTILE) && 
					(pObjectOne->GetPhysicalObjectType() == PhysicalObject::eCOMPUTER_SHIP) && 
					(((Projectile *)pObjectTwo)->GetCreator()->GetPhysicalObjectType() 
					== PhysicalObject::eCOMPUTER_SHIP))
				{ //computer created projectile and a computer ship...
					
				}
				else if ((pObjectOne->GetPhysicalObjectType() == PhysicalObject::ePROJECTILE) && 
					(pObjectTwo->GetPhysicalObjectType() == PhysicalObject::eCOMPUTER_SHIP) && 
					(((Projectile *)pObjectOne)->GetCreator()->GetPhysicalObjectType() 
					== PhysicalObject::eCOMPUTER_SHIP))
				{ //computer created projectile and a computer ship...

				}
				else {
					if (pObjectTwo && pObjectTwo->GetNode())
					{
						float fRadiusTwo = pObjectOne->GetNode()->m_vScale.x;
						if (fRadiusTwo < pObjectOne->GetNode()->m_vScale.y)
						{
							fRadiusTwo = pObjectOne->GetNode()->m_vScale.y;
						}
						fRadiusTwo = fRadiusTwo / 2.0f;
						
						//Compare the two objects for collision
						float fXDistSqr		= pObjectOne->GetNode()->m_PosQuat.pos.x - pObjectTwo->GetNode()->m_PosQuat.pos.x;
						fXDistSqr			= fXDistSqr * fXDistSqr;
						float fYDistSqr		= pObjectOne->GetNode()->m_PosQuat.pos.y - pObjectTwo->GetNode()->m_PosQuat.pos.y;
						fYDistSqr			= fYDistSqr * fYDistSqr;
						float fRDistSqr		= fRadiusOne * fRadiusOne + fRadiusTwo + fRadiusTwo;
						float fRadiusSqr	= (fRadiusOne + fRadiusTwo) * (fRadiusOne + fRadiusTwo);
					
						if ((fXDistSqr + fYDistSqr) < fRadiusSqr)
						{ //collision occured
							pObjectOne->Collide(pObjectTwo);
							pObjectTwo->Collide(pObjectOne);
						}
					}

					pObjectTwo = 0;
				}
			}
		}

		++i;
		pObjectOne = 0;
	}
}

CollisionBucket *CollisionManager::GetBucket(int x, int y)
{
	if ((x < 0) || (y < 0))
	{
		return 0;
	}

	CollisionBucket *pBucket = 0;

	unsigned int dwIndex = (unsigned int)GAME_SIZE_X * y + x;

	//printf("(%d, %d) = bucket %d\n", x, y, dwIndex);
	if (dwIndex < m_vBuckets.size())
	{
		pBucket = m_vBuckets.at(dwIndex);
	}

	return pBucket;
}

void CollisionManager::Init()
{
	m_fBucketSize						= 1.0f;
	m_fTimeTillNextCheck				= 0.0f;
	CollisionBucket		*pBucket		= 0;

	for (float y = 0; y < GAME_SIZE_Y; y++)
	{
		for (float x = 0; x < GAME_SIZE_X; x++)
		{
			pBucket				= new CollisionBucket();
			pBucket->m_dwX		= x;
			pBucket->m_dwY		= y;
			m_vBuckets.push_back(pBucket);
			pBucket				= 0;
		}
	}
}	

void CollisionManager::Refresh(const float &fDeltaTime)
{
	m_fTimeTillNextCheck -= fDeltaTime; //update counter for collision checks

	if (m_fTimeTillNextCheck <= 0.0f)
	{
		m_fTimeTillNextCheck += SECONDS_PER_COLLISION_CHECK;

		//CHECK FOR COLLISIONS!!!
		ClearBuckets();

		// (1) Put each phys object into the correct bucket(s)
		SortIntoBuckets();

		// (2) For each bucket, compare for collisions between all members, and resolve them
		CollisionBucket *pBucket = 0;
		for (unsigned int i = 0; i < m_vBuckets.size(); ++i)
		{
			pBucket = m_vBuckets.at(i);
			if (pBucket && (pBucket->m_vContents.size() > 1))
			{
				FindCollisions(pBucket); //look for collisions if the bucket has more than one physical object in it
			}
		}
	}
}

void CollisionManager::SelectNext()
{
	if ((m_dwSelectedIndex + 1) < m_vBuckets.size())
	{
		m_dwSelectedIndex++;
	}
}

void CollisionManager::SelectPrevious()
{
	if (m_dwSelectedIndex != 0)
	{
		m_dwSelectedIndex--;
	}

}

void CollisionManager::SortIntoBuckets()
{
	Object			*pObject			= 0;
	PhysicalObject	*pPhysObject		= 0;
	float			fXStart				= -(GAME_SIZE_X / 2.0f);
	float			fYStart				= -(GAME_SIZE_Y / 2.0f);
	float			fRadius				= 0.0f;

	list<Object *>::const_iterator		iter;
	for (iter = gGame()->GetCurrentScene()->GetObjectIteratorBegin(); iter != gGame()->GetCurrentScene()->GetObjectIteratorEnd(); iter++)
	{
		pObject = *iter;
		if (pObject && pObject->GetNode() && (pObject->GetObjectType() == Object::eTYPE_PHYSICAL_OBJECT))
		{	
			pPhysObject = (PhysicalObject *)pObject;	
			
			//Find the object's radius
			fRadius = pPhysObject->GetNode()->m_vScale.x;
			if (fRadius < pPhysObject->GetNode()->m_vScale.y)
			{
				fRadius = pPhysObject->GetNode()->m_vScale.y;
			}
			fRadius = fRadius / 2.0f;

			//If the object has a radius less than the bucket size, then do it the faster way
			if (fRadius < m_fBucketSize)
			{
				//find X column and Y column
				float	fPositionX			= pPhysObject->GetNode()->m_PosQuat.pos.x - fXStart;
				float	fPositionY			= pPhysObject->GetNode()->m_PosQuat.pos.y - fYStart;
				float	fFloorX				= floor(fPositionX);
				float	fFloorY				= floor(fPositionY);
				float	fWidthX				= 0.5f * pPhysObject->GetNode()->m_vScale.x;
				float	fWidthY				= 0.5f * pPhysObject->GetNode()->m_vScale.y;
				int		dwBucketX			= (int) fFloorX;
				int		dwBucketY			= (int) fFloorY;

				bool bXGreater	= false;
				bool bXLess		= false;
				bool bYGreater	= false;
				bool bYLess		= false;

				if ((fPositionX + fWidthX - fFloorX) >= 1.0f)
				{
					bXGreater		= true;
				}
				if ((fPositionX - fWidthX - fFloorX) <= 0.0f)
				{
					bXLess			= true;
				}
				if ((fPositionY + fWidthY - fFloorY) >= 1.0f)
				{
					bYGreater		= true;
				}
				if ((fPositionY - fWidthY - fFloorY) <= 0.0f)
				{
					bYLess			= true;
				}
				
				//add to bucket, if it exists
				AddPhysicalObjectToBucket(pPhysObject, GetBucket(dwBucketX, dwBucketY));

				//add to the necessary surrounding buckets!
				if (bXGreater)
				{
					AddPhysicalObjectToBucket(pPhysObject, GetBucket(dwBucketX + 1, dwBucketY));
					
					if (bYGreater)
					{
						AddPhysicalObjectToBucket(pPhysObject, GetBucket(dwBucketX + 1, dwBucketY + 1));
					}
					if (bYLess)
					{
						AddPhysicalObjectToBucket(pPhysObject, GetBucket(dwBucketX + 1, dwBucketY - 1));
					}
				}

				if (bXLess)
				{
					AddPhysicalObjectToBucket(pPhysObject, GetBucket(dwBucketX - 1, dwBucketY));
					
					if (bYGreater)
					{
						AddPhysicalObjectToBucket(pPhysObject, GetBucket(dwBucketX - 1, dwBucketY + 1));
					}
					if (bYLess)
					{
						AddPhysicalObjectToBucket(pPhysObject, GetBucket(dwBucketX - 1, dwBucketY - 1));
					}
				}

				if (bYGreater)
				{
					AddPhysicalObjectToBucket(pPhysObject, GetBucket(dwBucketX, dwBucketY + 1));
				}

				if (bYLess)
				{
					AddPhysicalObjectToBucket(pPhysObject, GetBucket(dwBucketX, dwBucketY - 1));
				}

				//temporary debug output
				pPhysObject->GetNode()->m_dwDebugX = (unsigned int) fFloorX;
				pPhysObject->GetNode()->m_dwDebugY = (unsigned int) fFloorY;
			}
			else
			{
				printf("NOT HANDLING COLLISION FOR THIS BIG OBJECT!!!!!!!!\n");
			}
			
			pPhysObject = 0;
		}		
		else
		{
			//printf("-------OTHER: %s\n", pObject->GetClassName());
		}

		pObject = 0;
	}
}	

CollisionBucket::CollisionBucket()
{
	m_dwX = -1;
	m_dwY = -1;
}

CollisionBucket::~CollisionBucket()
{
	ClearContents();
}

void CollisionBucket::ClearContents()
{
	m_vContents.clear();
}