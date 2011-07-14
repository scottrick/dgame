#include "PowerUp.h"

PowerUp::PowerUp()
{
	Init();
}

PowerUp::~PowerUp()
{

}

void PowerUp::Init()
{
	SetPhysicalObjectType(PhysicalObject::ePROJECTILE);
}

void PowerUp::Refresh(const float &fDeltaTime)
{
	PhysicalObject::Refresh(fDeltaTime);
}