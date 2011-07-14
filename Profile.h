#ifdef _PROFILING_ENABLED_

#ifndef _PROFILE_
#define _PROFILE_

#define	NAME_SIZE			256
#define NUM_SIZE			16
#define NUM_PROFILE_SAMPLES 10

/*
	Based on profiler from Game Programming Gems 1
*/
class Profile {
public:
	typedef struct {
		bool				bValid;
		char				name[NAME_SIZE];
		int					dwOpenProfiles;

		float				fStartTime;
		float				fAccumulator;
		float				fChildrenSampleTime;

		unsigned int		dwNumParents;
		unsigned int		dwProfileInstances;
	} ProfileSample;

	typedef struct {
		bool				bValid;
		char				name[NAME_SIZE];

		float				fAvg;
		float				fMin;
		float				fMax;
	} ProfileSampleHistory;

	static ProfileSample			g_Samples[NUM_PROFILE_SAMPLES];
	static ProfileSampleHistory		g_History[NUM_PROFILE_SAMPLES];
	static float					g_fStartProfile;
	static float					g_fEndProfile;
	//static float					g_fFrameStart;

	static void						ClearTextBuffer();
	static float					GetElapsedTime();
	static void						GetProfileFromHistory(char *name, float *avg, float *min, float *max);
	static float					GetTime();
	static void						ProfileBegin(char *name);
	static void						ProfileDumpOutputToBuffer();
	static void						ProfileEnd(char *name);
	static void						ProfileInit();
	static void						PutTextBuffer(char *pText);
	static void						StoreProfileInHistory(char *name, float percent);

private:
	Profile();
	~Profile();
};

#endif

#endif //_PROFILING_ENABLED_