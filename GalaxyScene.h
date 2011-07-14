#ifndef _GALAXY_SCENE_
#define _GALAXY_SCENE_

#include "Scene.h"

#include <list>
using namespace std;

class Text;
class UIObject;
class UIScroller;

class GalaxyScene : public Scene
{
public:
	GalaxyScene();

	virtual const char			*GetClassName() const				{ return "GalaxyScene"; }
	virtual void				HandleInput(const unsigned char &key, const int &x, const int &y, const unsigned int &dwKeyboardFlags);
	virtual void				Print(int indent) const;
	virtual void				SceneEnd();
	virtual void				SceneStart();

protected:
	~GalaxyScene();

	void						SetDescriptionText(Text *pText);
	void						SetNameText(Text *pText);
	void						SetOpponentsText(Text *pText);
	void						SetScroller(UIScroller *pScroller);
	void						UpdateSectorText();
	
private:
	void						Init();

	Text						*m_pNameText;
	Text						*m_pDescriptionText;
	Text						*m_pOpponentsText;
	UIScroller					*m_pScroller;
};

#endif