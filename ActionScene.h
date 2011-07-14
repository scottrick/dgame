#ifndef _ACTION_SCENE_
#define _ACTION_SCENE_

#include "Scene.h"

class Background;
class PhysicalObject;
class Sector;
class ShaderProgram;

class ActionScene : public Scene
{
public:
	ActionScene();

	void						DrawPlayerInfo(); //kinda hacky for now..
	virtual const char			*GetClassName() const				{ return "ActionScene"; }
	Sector						*GetSector()						{ return m_pSector; }
	virtual void				Print(int indent) const;
	void						RefreshManagers(const float &fDeltaTime);
	virtual void				Render(Manager *pDebugManager);
	virtual void				SceneEnd();
	virtual void				SceneStart();
	void						SetPlayerObject(PhysicalObject *pNewObject);
	void						SetSector(Sector *pSector);

protected:
	~ActionScene();

	PhysicalObject				*m_pPlayerObject;			//Current object controlled by the player
	Sector						*m_pSector;					//Current Sector being played by the player

private:
	void						Init();

};

#endif