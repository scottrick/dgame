#ifndef _SEGMENT_
#define _SEGMENT_

#include "PhysicalObject.h"

#include <string>
using namespace std;

class ShipModifier;

class Segment : public PhysicalObject
{
public:
	Segment();

	void								Clone(Segment *pClone);
	virtual void						Collide(PhysicalObject *pOtherObject);
	virtual void						Destroy();
	virtual void						FromBlock(Block *pBlock);
	virtual void						Generate(int dwLevel)					= 0;

	virtual const char					*GetClassName()	const					= 0;
	virtual string						GetTypeString() const					= 0;
	virtual VECTOR3						GetDebugColor() const					{ return VECTOR3(1.0f, 0.0f, 0.0f); }	Block								*ToBlock();
	ShipModifier						*GetShipModifier()						{ return m_pShipModifier; }
	void								SetShipModifier(ShipModifier *pModifier);
	virtual Shockwave					*GetShockwave();

protected:
	virtual ~Segment();

	ShipModifier						*m_pShipModifier;

private:
	void								Init();


};

#endif