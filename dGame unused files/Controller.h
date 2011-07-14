#ifndef _CONTROLLER_
#define _CONTROLLER_

#include "Object.h"
#include <vector>
using namespace std;

class PhysicalObject;

/*
	A Controller controls a physical object.  PHYSICAL OBJECTs AddRef their controllers. 
	The Controllers do NOT AddRef their objects.  
*/
class Controller : public Object
{
public:
	Controller();

	void								AddControlledObject(PhysicalObject *pNewObject);
		
	virtual const char					*GetClassName() const			{ return "Controller"; }
	virtual void						Print(int indent) const			= 0;

	virtual void						Refresh(int dwDeltaTime)		= 0;
	virtual void						RemoveControlledObject(PhysicalObject *pRemove);

protected:
	virtual ~Controller();

	vector<PhysicalObject *>			m_vControlledObjects;

private:
	void								ClearControlledObjects();
	void								Init();

};

#endif