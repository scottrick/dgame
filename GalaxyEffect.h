#ifndef GALAXY_EFFECT
#define GALAXY_EFFECT

#include "Effect.h"

#include "Quat.h"
#include <list>
using namespace std;

#define MIN_NUMBER_OF_LAYERS		3
#define MAX_NUMBER_OF_LAYERS		8

struct galLayer
{
	int						m_dwTexture;
	float					m_fSecondsPerRotation;
	Quat					m_qRotation;
	VECTOR3					m_vColor;	
};

class GalaxyEffect : public Effect
{
public:
	GalaxyEffect();

	void					FromBlock(Block *pBlock);
	void					Generate();
	const char				*GetClassName() const			{ return "GalaxyEffect"; }
	void					Refresh(const float &fDeltaTime);
	void					Render();
	Block					*ToBlock();

protected:
	~GalaxyEffect();

	bool					m_bClockwise;

	list<galLayer>			m_lLayers;

private:
	void					Init();

};

#endif