#include "UnknownManager.h"

#include "Unknown.h"
#include "UnknownBucket.h"

#include <cassert>
#include <GL/glut.h>
using namespace std;

UnknownManager *gUnknownManager()
{
	static UnknownManager theUnknownManager;
	return &theUnknownManager;
}

UnknownManager::UnknownManager()
{
	Init();
}

UnknownManager::~UnknownManager()
{
	Cleanup();
}

void UnknownManager::AddToBuckets(Unknown *pAdd)
{
#ifdef _UNKNOWN_DEBUG
	UnknownBucket							*pBucket		= 0;
	deque<UnknownBucket *>::iterator			bucketIter;
	for (bucketIter = m_vUnknownBuckets.begin(); bucketIter != m_vUnknownBuckets.end(); bucketIter++)
	{ //look through all the buckets and find the bucket for the unknown we are adding!
		pBucket = *bucketIter;
		if (strcmp(pBucket->GetClassBucketName(), pAdd->GetClassName()) == 0)
		{
			//found a match
			break;
		}
		pBucket = 0;
	}

	if (!pBucket)
	{ //didn't find a match, so create a new bucket for this class
		pBucket = new UnknownBucket();
		pBucket->SetClassBucketName(pAdd->GetClassName());
		m_vUnknownBuckets.push_back(pBucket);
	}

	pBucket->AddUnknown(pAdd);
#endif
}

/*
	Since Adding and Removing from the UnknownManager is handled automatically in the Unknown constructor / destructor,
	the UnknownManager does *NOT* AddRef and Release them.
*/
void UnknownManager::AddUnknown(Unknown *pAdd)
{
#ifdef _UNKNOWN_DEBUG
	if (pAdd)
	{	
		m_vUnsorted.push_back(pAdd); //add unknown to unsorted list
	}
#endif
}

void UnknownManager::Cleanup()
{
#ifdef _UNKNOWN_DEBUG
	UnknownBucket							*pBucket		= 0;
	deque<UnknownBucket *>::iterator			iter;

	for (iter = m_vUnknownBuckets.begin(); iter != m_vUnknownBuckets.end(); iter++)
	{
		pBucket = *iter;

		if (pBucket)
		{
			delete pBucket;
			pBucket = 0;
		}
	}		
	m_vUnknownBuckets.clear();
	m_vUnsorted.clear();
#endif	
}

void UnknownManager::ClearBuckets()
{
#ifdef _UNKNOWN_DEBUG
	deque<UnknownBucket *>::iterator iter;
	for (iter = m_vUnknownBuckets.begin(); iter != m_vUnknownBuckets.end(); iter++)
	{	
		if (*iter)
		{
			(*iter)->Clear();
		}
	}
#endif
}

void UnknownManager::DebugInfoRender(const float &fRatio, const PosQuat &pq)
{
#ifdef _UNKNOWN_DEBUG
	UpdateBuckets();

	GLboolean								bTextureWasOn	= glIsEnabled(GL_TEXTURE_2D);
	GLboolean								bLightingWasOn	= glIsEnabled(GL_LIGHTING);
	unsigned int							dwPadding		= 0;
	unsigned int							dwLength		= 0;
	unsigned int							dwSpace			= 25;
	float									fOffset			= 0.05f;
	float									fXStart			= -fRatio;

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D); //disable textures when rendering the text

	//reset matrices so text is always in the correct spot
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, fRatio, 0.1, 50.0); //update this so we dont get crappy distortion
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 1.75, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glColor3f(1.0f, 0.0f, 0.0f);

	{
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		size_t size = 50;
		char *pDetails = (char *)malloc(sizeof(char) * size);
		memset(pDetails, 0, size);
		sprintf_s(pDetails, size, "Unknown Manager");

		glRasterPos2f(fXStart, 0.97f);
		for (const char *c = pDetails; *c != '\0'; c++)
		{
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
		}
		free(pDetails);
	}

	//draw bucket summaries!
	deque<UnknownBucket *>::iterator	iter;
	UnknownBucket						*pBucket		= 0;
	unsigned int						dwCurrentIndex	= 0;
	for (iter = m_vUnknownBuckets.begin(); iter != m_vUnknownBuckets.end(); iter++)
	{
		pBucket = *iter;
		if (pBucket)
		{
			if (dwCurrentIndex == m_dwSelectedIndex)
			{
				glColor4f(0.3f, 0.3f, 1.0f, 1.0f);

				//draw extra debug information about the selected unknown type maybe?
			}
			else
			{
				glColor4f(0.8f, 0.8f, 1.0f, 1.0f);
			}
			
			size_t size = 50;
			char *pDetails = (char *)malloc(sizeof(char) * size);
			memset(pDetails, 0, size);

			dwLength = (unsigned int)strlen(pBucket->GetClassBucketName());
			if (dwLength >= dwSpace)
			{
				dwPadding = 1;
			}
			else
			{
				dwPadding = dwSpace - dwLength;
			}

			sprintf_s(pDetails, size, "  %s%*s %u", pBucket->GetClassBucketName(), dwPadding, "", pBucket->GetCount());

			if (0.97f - fOffset < -0.95f)
			{
				fXStart += 0.6f;
				fOffset = 0.05f;
			}

			glRasterPos2f(fXStart, 0.97f - fOffset);
			for (const char *c = pDetails; *c != '\0'; c++)
			{
				glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
			}
			free(pDetails);
			fOffset += 0.05f;
		}

		dwCurrentIndex++;
	}

	if (bTextureWasOn)
	{
		glEnable(GL_TEXTURE_2D);
	}
	if (bLightingWasOn)
	{
		glEnable(GL_LIGHTING);
	}
#endif
}

void UnknownManager::DebugScreenRender()
{

}

void UnknownManager::Init()
{
#ifdef _UNKNOWN_DEBUG
	m_dwUpdateCounter = 0;
#endif
}

void UnknownManager::Print()
{
#ifdef _UNKNOWN_DEBUG
	UpdateBuckets(); //update the buuckets if necessary
	printf("UnknownManager Summary WITH extra DEBUG information\n");
	printf("  BUCKET                         COUNT\n");
	printf("  ------------------------------------\n");
	
	unsigned int							dwPadding		= 0;
	unsigned int							dwLength		= 0;
	unsigned int							dwSpace			= 30;
	UnknownBucket							*pBucket		= 0;
	deque<UnknownBucket *>::iterator		iter;
	
	for (iter = m_vUnknownBuckets.begin(); iter != m_vUnknownBuckets.end(); iter++)
	{
		pBucket = *iter;
		dwLength = (unsigned int)strlen(pBucket->GetClassBucketName());
		if (dwLength >= dwSpace)
		{
			dwPadding = 1;
		}
		else
		{
			dwPadding = dwSpace - dwLength;
		}
		printf("  %s%*s %u\n", pBucket->GetClassBucketName(), dwPadding, "", pBucket->GetCount());
	}
#else
	printf("UnknownManager Summary\n");
	printf(" -- UNABLE TO SUMMARIZE!  #DEFINE _UNKNOWN_DEBUG --\n");
#endif
}

void UnknownManager::Refresh(const float &fDeltaTime)
{
	
}

void UnknownManager::RemoveFromBuckets(Unknown *pRemove)
{ //at this point, we can no longer tell what type of class it was, so just attempt to remove from every bucket
#ifdef _UNKNOWN_DEBUG
	UnknownBucket							*pBucket		= 0;
	deque<UnknownBucket *>::iterator			bucketIter;
	for (bucketIter = m_vUnknownBuckets.begin(); bucketIter != m_vUnknownBuckets.end(); bucketIter++)
	{ //look through all the buckets and find the bucket for the unknown we are adding!
		pBucket = *bucketIter;
		pBucket->RemoveUnknown(pRemove);
	}

	//remove from the unsorted vector, if it is in there
	deque<Unknown *>::iterator				unsortedIter;
	for (unsortedIter = m_vUnsorted.begin(); unsortedIter != m_vUnsorted.end(); unsortedIter++)
	{
		if ((*unsortedIter) == pRemove)
		{
			m_vUnsorted.erase(unsortedIter);
			break;
		}
	}
#endif
}

void UnknownManager::RemoveUnknown(Unknown *pRemove)
{
#ifdef _UNKNOWN_DEBUG
	if (pRemove)
	{
		RemoveFromBuckets(pRemove);
	}
#endif
}

void UnknownManager::SelectNext()
{
#ifdef _UNKNOWN_DEBUG
	if ((m_dwSelectedIndex + 1) < m_vUnknownBuckets.size())
	{
		m_dwSelectedIndex++;
	}
#endif
}

void UnknownManager::SelectPrevious()
{
	if (m_dwSelectedIndex != 0)
	{
		m_dwSelectedIndex--;
	}
}

void UnknownManager::UpdateBuckets()
{
#ifdef _UNKNOWN_DEBUG
	{
		if (m_dwUpdateCounter > 0) {
			m_dwUpdateCounter--;
			return;  
		}
		else
		{
			m_dwUpdateCounter = 25; //update every 25 times is all...
		}

		if (m_vUnsorted.size() == 0)
		{
			return;
		}
		
		Unknown								*pUnknown		= 0;
		UnknownBucket						*pBucket		= 0;
		deque<Unknown *>::iterator			unknownIter;

		for (unknownIter = m_vUnsorted.begin(); unknownIter != m_vUnsorted.end(); unknownIter++)
		{
			deque<UnknownBucket *>::iterator	bucketIter;
			pUnknown	= *unknownIter;

			for (bucketIter = m_vUnknownBuckets.begin(); bucketIter != m_vUnknownBuckets.end(); bucketIter++)
			{ //look through all the buckets and find the bucket for the unknown we are adding!
				pBucket = *bucketIter;
				if (strcmp(pBucket->GetClassBucketName(), pUnknown->GetClassName()) == 0)
				{
					//found a match
					break;
				}
				pBucket = 0;
			}

			if (!pBucket)
			{ //didn't find a match, so create a new bucket for this class
				pBucket = new UnknownBucket();
				pBucket->SetClassBucketName(pUnknown->GetClassName());
				m_vUnknownBuckets.push_back(pBucket);
			}

			pBucket->AddUnknown(pUnknown);
		}

		m_vUnsorted.clear();
	}
#endif
}