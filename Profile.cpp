#ifdef _PROFILING_ENABLED_

#include "Profile.h"

#include <cassert>
#include <iostream>
#include <windows.h>
using namespace std;

float							Profile::g_fEndProfile						= 0.0f;
//float							Profile::g_fFrameStart						= 0.0f;
float							Profile::g_fStartProfile					= 0.0f;
Profile::ProfileSampleHistory	Profile::g_History[NUM_PROFILE_SAMPLES];
Profile::ProfileSample			Profile::g_Samples[NUM_PROFILE_SAMPLES];

void Profile::ClearTextBuffer()
{
	printf("\n");
}

float Profile::GetElapsedTime()
{
	//return Profile::g_fEndProfile - Profile::g_fFrameStart;
	return Profile::g_fEndProfile - Profile::g_fStartProfile;
}

void Profile::GetProfileFromHistory(char *name, float *avg, float *min, float *max)
{
	unsigned int i = 0;

	while ((i < NUM_PROFILE_SAMPLES) && g_History[i].bValid)
	{
		if (strcmp(g_History[i].name, name) == 0)
		{
			*avg = g_History[i].fAvg;
			*min = g_History[i].fMin;
			*max = g_History[i].fMax;
			return;
		}

		++i;
	}

	*avg = *min = *max = 0.0f;
}

float Profile::GetTime()
{ 
	return ((float)GetTickCount()) / 1000.0f;
}

void Profile::ProfileBegin(char *name)
{
	unsigned int i = 0;

	while ((i < NUM_PROFILE_SAMPLES) && g_Samples[i].bValid)
	{
		if (strcmp(g_Samples[i].name, name) == 0)
		{ //found the sample we were looking for!
			g_Samples[i].dwOpenProfiles++;
			g_Samples[i].dwProfileInstances++;
			g_Samples[i].fStartTime = GetTime();
			assert(g_Samples[i].dwOpenProfiles == 1); //only can have one open at once
			return;
		}

		++i;
	}

	if (i >= NUM_PROFILE_SAMPLES)
	{
		assert(!"Exceeded allowed number of profile samples.");
		return;
	}

	strcpy_s(g_Samples[i].name, NAME_SIZE, name);
	g_Samples[i].bValid					= true;
	g_Samples[i].dwOpenProfiles			= 1;
	g_Samples[i].dwProfileInstances		= 1;
	g_Samples[i].fAccumulator			= 0.0f;
	g_Samples[i].fStartTime				= GetTime();
	g_Samples[i].fChildrenSampleTime	= 0.0f;
}

void Profile::ProfileDumpOutputToBuffer()
{
	static float fLastDump = GetTime();
	bool bPrint = false;

	if (GetTime() - fLastDump > 1.0f)
	{
		fLastDump = GetTime();
		bPrint = true;
	}
	
	unsigned int i = 0;

	g_fEndProfile = GetTime();
	
	if (bPrint)
	{
		ClearTextBuffer();
		PutTextBuffer("  Avg :   Min :   Max :   # : Profile Name\n");
		PutTextBuffer("------------------------------------------\n");
	}

	while ((i < NUM_PROFILE_SAMPLES) && g_Samples[i].bValid)
	{
		unsigned int indent = 0;
		float fSampleTime, fPercentTime, fAvgTime, fMinTime, fMaxTime;
		char line[NAME_SIZE], name[NAME_SIZE], indentedName[NAME_SIZE];
		char avg[NUM_SIZE], min[NUM_SIZE], max[NUM_SIZE], num[NUM_SIZE];

		if (g_Samples[i].dwOpenProfiles < 0)
		{
			assert(!"ProfileEnd() called without a ProfileBegin()!");
		}
		else if (g_Samples[i].dwOpenProfiles > 0)
		{
			assert(!"ProfileBegin() called without a ProfileEnd()!");
		}

		if (GetElapsedTime() > 0.0f)
		{
			fSampleTime		= g_Samples[i].fAccumulator - g_Samples[i].fChildrenSampleTime;
			fPercentTime	= (fSampleTime / (g_fEndProfile - g_fStartProfile)) * 100.0f;
			fAvgTime		= fMinTime = fMaxTime = fPercentTime;

			StoreProfileInHistory(g_Samples[i].name, fPercentTime);
		}

		GetProfileFromHistory(g_Samples[i].name, &fAvgTime, &fMinTime, &fMaxTime);

		sprintf_s(avg, NUM_SIZE, "%3.1f", fAvgTime);
		sprintf_s(min, NUM_SIZE, "%3.1f", fMinTime);
		sprintf_s(max, NUM_SIZE, "%3.1f", fMaxTime);
		sprintf_s(num, NUM_SIZE, "%3.1f", (float)g_Samples[i].dwProfileInstances);

		strcpy_s(indentedName, NAME_SIZE, g_Samples[i].name);

		for (indent = 0; indent < g_Samples[i].dwNumParents; indent++)
		{
			sprintf_s(name, NAME_SIZE, "   %s", indentedName);
			strcpy_s(indentedName, NAME_SIZE, name);
		}

		sprintf_s(line, NAME_SIZE, "%5s : %5s : %5s : %3s : %s\n", avg, min, max, num, indentedName);

		if (bPrint)
		{
			PutTextBuffer(line);
		}

		++i;
	}

	{
		for (unsigned int i = 0; i < NUM_PROFILE_SAMPLES; ++i)
		{
			g_Samples[i].bValid = false;
		}

		//g_fStartProfile = GetTime();
	}
}

void Profile::ProfileEnd(char *name)
{
	unsigned int i					= 0;
	unsigned int dwNumParents		= 0;

	while ((i < NUM_PROFILE_SAMPLES) && g_Samples[i].bValid)
	{
		if (strcmp(g_Samples[i].name, name) == 0)
		{ //found the sample we are ending
			unsigned int	dwInner		= 0;
			int				dwParent	= -1;
			float			fEndTime	= GetTime();

			g_Samples[i].dwOpenProfiles--;

			//count all parents and find the immediate parent
			while (g_Samples[dwInner].bValid == true)
			{
				if (g_Samples[dwInner].dwOpenProfiles > 0)
				{ //found a parent
					dwNumParents++;

					if (dwParent < 0)
					{
						dwParent = dwInner;
					}
					else if (g_Samples[dwInner].fStartTime >= g_Samples[dwParent].fStartTime)
					{
						dwParent = dwInner;
					}
				}

				dwInner++;
			}

			g_Samples[i].dwNumParents = dwNumParents;

			if (dwParent >= 0)
			{
				g_Samples[dwParent].fChildrenSampleTime += fEndTime - g_Samples[i].fStartTime;
			}

			g_Samples[i].fAccumulator += fEndTime - g_Samples[i].fStartTime;
			if (g_Samples[i].fAccumulator > 0.0f)
			{
				printf("");
			}
			return;
		}

		++i;
	}
}

void Profile::ProfileInit()
{
	for (int i = 0; i < NUM_PROFILE_SAMPLES; ++i)
	{
		g_Samples[i].bValid	= false;
		char *pName			= "notset";
		strcpy_s(g_Samples[i].name, 7, pName);
		g_History[i].bValid	= false;
	}

	//g_fEndProfile				= 0.0f;
	//g_fStartProfile				= GetTime();
	//g_fFrameStart				= 0.0f;
}

void Profile::PutTextBuffer(char *pText)
{
	printf("%s", pText);
}

void Profile::StoreProfileInHistory(char *name, float percent)
{
	unsigned int i = 0;
	float fOldRatio;
	float fNewRatio = 0.8f * GetElapsedTime();
	
	if (GetElapsedTime() <= 0.0f)
	{
		return;
	}

	if (fNewRatio > 1.0f)
	{
		fNewRatio = 1.0f;
	}

	fOldRatio = 1.0f - fNewRatio;

	while ((i < NUM_PROFILE_SAMPLES) && g_History[i].bValid)
	{
		if (strcmp(g_History[i].name, name) == 0)
		{
			g_History[i].fAvg = (g_History[i].fAvg * fOldRatio) + (percent * fNewRatio);

			if (percent < g_History[i].fMin)
			{
				g_History[i].fMin = percent;
			}
			else 
			{
				g_History[i].fMin = (g_History[i].fMin * fOldRatio) + (percent * fNewRatio);
			}

			if (percent > g_History[i].fMax)
			{
				g_History[i].fMax = percent;
			}
			else
			{
				g_History[i].fMax = (g_History[i].fMax * fOldRatio) + (percent * fNewRatio);
			}

			return;
		}

		++i;
	}
	
	if (i < NUM_PROFILE_SAMPLES)
	{
		strcpy_s(g_History[i].name, NAME_SIZE, name);
		g_History[i].bValid = true;

		g_History[i].fAvg = g_History[i].fMin = g_History[i].fMax = percent;
	}
	else
	{
		assert(!"Exceeded Max Available Profile Samples!");
	}
}

#endif //_PROFILING_ENABLED_