#ifndef _UTILITY_FUNCTIONS_
#define _UTILITY_FUNCTIONS_

#include <cstdlib>
using namespace std;

static float GenerateRandomFloat(float range) {
	return (float)rand() / (float(RAND_MAX + 1)) * range;
}	

static unsigned int GenerateRandomInt(unsigned int range) {
	//returns random int from 0 to range
	return rand() % (range + 1);
}

static void InitializeRandomGenerator(unsigned int dwSeed)
{
	srand(dwSeed);
}

#endif