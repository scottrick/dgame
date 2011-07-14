#ifndef _PROPULSION_SEGMENT
#define _PROPULSION_SEGMENT

#include "Segment.h"

class MovementPlugin;

#define CONVENTIONAL_ODDS			6
#define JUMP_ODDS					0
#define STATIONARY_ODDS				0
#define TOTAL_PROPULSION_ODDS		(CONVENTIONAL_ODDS + JUMP_ODDS + STATIONARY_ODDS)

class PropulsionSegment : public Segment
{
public:
	
	enum PropulsionSegmentType {
		eTYPE_CONVENTIONAL,
		eTYPE_JUMP,
		eTYPE_STATIONARY,

		eNUM_TYPES,
		eTYPE_NO_TYPE,
	};

	PropulsionSegment();

	void						Clone(PropulsionSegment *pClone);
	PhysicalObject				*Duplicate();
	virtual void				Generate(int dwLevel);
	void						Generate(int dwLevel, PropulsionSegmentType type); 
	virtual const char			*GetClassName() const				{ return "PropulsionSegment"; }
	//MovementPlugin				*GetMovementPlugin
	PropulsionSegmentType		GetPropulsionSegmentType() const	{ return m_Type; }
	virtual string				GetTypeString() const;
	//void						SetMovementPlugin(MovementPlugin *pPlugin);
	void						SetPropulsionSegmentType(PropulsionSegmentType newType)		{ m_Type = newType; }

protected:
	virtual ~PropulsionSegment();

private:
	void						GenerateShipModifiers(int dwLevel);
	void						Init();

	//MovementPlugin				*m_pMovementPlugin;
	PropulsionSegmentType		m_Type;

};

#endif