#ifndef _ENGINEERING_SEGMENT_
#define _ENGINEERING_SEGMENT_

#include "Segment.h"

#define FORTIFIED_ODDS				2
#define BOOSTED_ODDS				2
#define CLONING_ODDS				1
#define TOTAL_ENGINEERING_ODDS		(FORTIFIED_ODDS + BOOSTED_ODDS + CLONING_ODDS)

class EngineeringSegment : public Segment
{
public:

	enum EngineeringSegmentType {
		eTYPE_FORTIFIED,
		eTYPE_BOOSTED,
		eTYPE_CLONING,

		eNUM_TYPES,
		eTYPE_NO_TYPE,
	};

	EngineeringSegment();

	void						Clone(EngineeringSegment *pClone);
	virtual PhysicalObject		*Duplicate();
	virtual void				Generate(int dwLevel);
	virtual const char			*GetClassName() const				{ return "EngineeringSegment"; }
	EngineeringSegmentType		GetEngineeringSegmentType()	const	{ return m_Type; }
	virtual string				GetTypeString() const;

	void						SetEngineeringSegmentType(EngineeringSegmentType newType)		{ m_Type = newType; }

protected:
	virtual ~EngineeringSegment();

private:
	void						Init();

	EngineeringSegmentType		m_Type;

};

#endif