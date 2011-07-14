#ifndef _COMMAND_SEGMENT_
#define _COMMAND_SEGMENT_

#include "Segment.h"

class FiringPlugin;
class ShipModifier;

#define	AIMED_ODDS				3
#define BLIND_ODDS				4
#define KAMIKAZE_ODDS			1
#define	TOTAL_COMMAND_ODDS		(AIMED_ODDS + BLIND_ODDS + KAMIKAZE_ODDS)

class CommandSegment : public Segment
{
public:

	enum CommandSegmentType {
		eTYPE_AIMED,
		eTYPE_BLIND,
		eTYPE_KAMIKAZE,

		eNUM_TYPES,
		eTYPE_NO_TYPE,
	};

	CommandSegment();

	void						Clone(CommandSegment *pClone);
	PhysicalObject				*Duplicate();
	virtual void				Generate(int dwLevel);
	virtual const char			*GetClassName() const				{ return "CommandSegment"; }
	CommandSegmentType			GetCommandSegmentType() const		{ return m_Type; }
	ShipModifier				*GetShipModifier()					{ return m_pShipModifier; }
	virtual string				GetTypeString() const;
	void						SetCommandSegmentType(CommandSegmentType newType)	{ m_Type = newType; }

protected:
	virtual ~CommandSegment();

private:
	void						Init();

	CommandSegmentType			m_Type;
};

#endif