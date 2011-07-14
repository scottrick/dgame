#ifndef _SCENE_
#define _SCENE_					1

#include "Unknown.h"

#include "PosQuat.h"

#include <list>
using namespace std;

class Camera;
class Effect;
class Manager;
class Node;
class Object;
class PostProcessEffect;
class UIObject;

/*
	A Scene is a collection of nodes to be rendered on the screen.  Basically just a fancy list for now.
*/
class Scene : public Unknown
{
public:
	Scene();

	bool							AddNode(Node *pNode);
	bool							AddObject(Object *pObject);
	bool							AddUIObject(UIObject *pUIObject);
	Camera							*GetCamera()						{ return m_pCamera; }
	virtual const char				*GetClassName() const				= 0;
	int								GetComputerShipCount() const		{ return m_dwComputerShipCount; }
	int								GetNumObjects() const				{ return m_lObjects.size(); }
	int								GetNumPostRenderEffects() const		{ return m_lPostProcessEffects.size(); }
	list<Object *>::const_iterator	GetObjectIteratorBegin() const		{ return m_lObjects.begin(); }
	list<Object *>::const_iterator	GetObjectIteratorEnd() const		{ return m_lObjects.end(); }
	virtual void					HandleInput(const unsigned char &key, const int &x, const int &y, const unsigned int &dwKeyboardFlags);
	virtual void					HandleMouseMove(const int &dwX, const int &dwY, const int &dwDeltaX, const int &dwDeltaY);
	virtual void					HandleMouseMovePassive(const int &dwX, const int &dwY, const int &dwDeltaX, const int &dwDeltaY);
	virtual void					Print(int indent) const;
	void							RefreshAll(const float &fDeltaTime);
	virtual void					RefreshManagers(const float &fDeltaTime);
	void							RemoveAllEffects();
	void							RemoveAllNodes();
	void							RemoveAllObjects();
	void							RemoveAllPostProcessEffects();
	bool							RemoveEffect(Effect *pEffect);
	bool							RemoveNode(Node *pNode);
	bool							RemoveObject(Object *pObject);
	bool							RemovePostProcessEffect(PostProcessEffect *pPost);
	bool							RemoveUIObject(UIObject *pUIObject);
	virtual void					Render(Manager *pDebugManager);
	virtual void					RenderDebug(Manager *pManager);
	virtual void					RenderUI();
	virtual void					SceneEnd();
	virtual void					SceneStart();
	void							ToggleBoundingBoxes();

protected:

	~Scene();

	void							RenderAllNodes();
	void							RenderAllEffects();
	void							RenderAllPostProcessEffects();
	void							RemoveAllUIObjects();
	void							SetCamera(Camera *pNewCamera);

	list<Effect *>					m_lEffects;
	list<Node *>					m_lNodes;
	list<Object *>					m_lObjects;
	list<PostProcessEffect *>		m_lPostProcessEffects;
	list<UIObject *>				m_lUIObjects;
	//list<UIObject *>				m_lUIFocusOrder;	

	Camera							*m_pCamera;
	PosQuat							m_PosQuat;

	int								m_dwComputerShipCount;

private:
	void							Init();

};

#endif