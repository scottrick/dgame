#ifndef _FITTING_SCENE_
#define _FITTING_SCENE_

#include "Scene.h"

class FittingScene : public Scene
{
public:
	FittingScene();

	virtual const char			*GetClassName() const				{ return "FittingScene"; }
	virtual void				Print(int indent) const;
	virtual void				SceneEnd();
	virtual void				SceneStart();

protected:
	~FittingScene();
	
private:

	void						Init();

};

#endif