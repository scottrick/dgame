#ifndef _NOTHING_SCENE_
#define _NOTHING_SCENE_

#include "Scene.h"

class NothingScene : public Scene
{
public:
	NothingScene();

	virtual const char			*GetClassName() const				{ return "NothingScene"; }
	virtual void				HandleInput(const unsigned char &key, const int &x, const int &y, const unsigned int &dwKeyboardFlags);
	virtual void				HandleMouseMove(const int &dwX, const int &dwY, const int &dwDeltaX, const int &dwDeltaY);
	virtual void				Print(int indent) const;
	virtual void				RenderDebug(Manager *pManager);
	virtual void				SceneEnd();
	virtual void				SceneStart();

protected:
	~NothingScene();

private:
	void						Init();

	float						m_fScale;
};

#endif