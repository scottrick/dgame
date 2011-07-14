#ifndef _TESTOBJECT_
#define _TESTOBJECT_				1

#include "Object.h"

class TestObject : public Object
{
public:
	TestObject();

	virtual const char			*GetClassName() const			{ return "TestObject"; }
	virtual void				Refresh(const float &fDeltaTime);

private:
	virtual ~TestObject();

	void						Init();

	bool						bXUp;
	bool						bYUp;

	float						fRotation;
};

#endif