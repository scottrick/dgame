#ifndef _MANAGER_
#define _MANAGER_

#include "PosQuat.h"

class Manager
{
public:

	Manager();
	virtual ~Manager();
	
	virtual void						Cleanup()						= 0;
	virtual void						DebugInfoRender(const float &fRatio, const PosQuat &pq)	= 0;
	virtual void						DebugScreenRender()				= 0;
	virtual const char					*GetManagerName()				= 0;
	virtual void						Refresh(const float &fDeltaTime)= 0;
	void								RenderHelpText(float fRatio);
	virtual void						SelectNext()					= 0;
	virtual void						SelectPrevious()				= 0;
	
protected:
	
	unsigned int						m_dwSelectedIndex;

private:

	void								Init();

};

#endif