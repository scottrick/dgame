#ifndef _OBSTACLE_
#define _OBSTACLE_

#include "PhysicalObject.h"

#include "def.h"

class Obstacle : public PhysicalObject
{
public:

	enum ObstacleType {
		eDRIFTING,
		eSTATIONARY,

		eNUM_TYPES,
		eNO_TYPE,
	};

	Obstacle();
	Obstacle(ObstacleType type);

	void							Clone(Obstacle *pClone);
	virtual void					Collide(PhysicalObject *pOtherObject);
	virtual PhysicalObject			*Duplicate();
	virtual	void					FromBlock(Block *pBlock);

	void							Generate(int dwLevel);
	virtual const char				*GetClassName() const				{ return "Obstacle"; }
	virtual VECTOR3					GetDebugColor() const				{ return VECTOR3(1.0f, 1.0f, 0.0f); }
	int								GetLevel() const					{ return m_dwLevel; }
	ObstacleType					GetObstacleType() const				{ return m_ObstacleType; }
	VECTOR3							GetRotationAxis() const				{ return m_vRotationAxis; }
	float							GetRotationSpeed() const			{ return m_fSecondsPerRotation; }
	virtual Shockwave				*GetShockwave();
	virtual void					Refresh(const float &fDeltaTime);
	void							SetObstacleType(ObstacleType newType);
	virtual Block					*ToBlock();

protected:
	virtual ~Obstacle();

	void							GenerateStats();
	virtual void					OnDeath();

	int								m_dwLevel;
	VECTOR3							m_vRotationAxis;
	float							m_fSecondsPerRotation;

	ObstacleType					m_ObstacleType;

private:
	void							Init();
		
};

#endif