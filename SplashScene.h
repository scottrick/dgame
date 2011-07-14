#ifndef _SPLASH_SCENE_
#define _SPLASH_SCENE_

#include "Scene.h"

class SplashScene : public Scene
{
public:
	SplashScene();

	virtual const char			*GetClassName() const				{ return "SplashScene"; }
	virtual void				Print(int indent) const;
	virtual void				SceneEnd();
	virtual void				SceneStart();

protected:
	~SplashScene();

	bool						m_bDateParsed;
	int							m_dwMonth;
	int							m_dwDay;
	int							m_dwYear;
	
private:

	void						Init();
	void						ParseDate();

};

#endif