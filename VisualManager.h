#ifndef _VISUAL_MANAGER_
#define _VISUAL_MANAGER_

#include "Manager.h"

#include "PosQuat.h"

#include <vector>
using namespace std;

class File;
class Visual;

class VisualManager : public Manager
{
public:
	VisualManager();
	~VisualManager();

	void								AddVisual(Visual *pAdd);
	void								Cleanup();
	void								DebugInfoRender(const float &fRatio, const PosQuat &pq);
	void								DebugScreenRender();
	const char							*GetManagerName()							{ return "VisualManager"; }
	Visual								*GetVisualByFileName(const char *pFileName);
	void								LoadVisuals();
	void								Refresh(const float &fDeltaTime);
	void								RemoveVisual(Visual *pRemove);
	void								SelectNext();
	void								SelectPrevious();
	void								SetAutoRotate(bool bNewRotate);
	void								SetPosQuat(PosQuat pq);
	void								ToggleAutoRotate();

private:

	void								ClearVisuals();
	void								Init();
	void								Load3DS(const char *pFileName, bool bFontDirectory);
	void								LoadAllVisuals(File *pDirectory, bool bRecursive, bool bFontDirectory);
	
	bool								m_bVisualsLoaded;

	vector<Visual *>					m_vVisuals;
	PosQuat								m_PosQuat;		//current rotation for debug display
	bool								m_bAutoRotate;	
	
};

extern VisualManager *gVisualManager();

#endif