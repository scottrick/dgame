#ifndef _OBJECT_
#define _OBJECT_				1

#include "Unknown.h"

class Block;
class Node;
class Scene;

/*
	An object is anything in the game world, such as a planet, or a spaceship.
	An object will probably have a node to represent it on the screen, but not necessarily.
	An object will also have a way of creating, and re-creating itself from a Block object (void *).
*/

class Object : public Unknown
{
public:

	enum ObjectType {
		eTYPE_UNKNOWN,
		eTYPE_BACKGROUND,
		eTYPE_CAMERA,
		eTYPE_CONTROLLER,
		eTYPE_EFFECT,
		eTYPE_POST_EFFECT,
		eTYPE_TEST_OBJECT,
		eTYPE_TEXT_OBJECT,
		eTYPE_PHYSICAL_OBJECT,

		eNUM_OBJECT_TYPES,
	};

	Object();

	void						Clone(Object *pClone);
	virtual void				FromBlock(Block *pBlock)		= 0;
	virtual const char			*GetClassName() const			= 0;
	Node						*GetNode()						{ return m_pNode; }
	Scene						*GetScene() const				{ return m_pScene; }
	ObjectType					GetObjectType() const			{ return m_dwObjectType; }

	virtual void				Print(int indent) const;
	virtual void				Refresh(const float &fDeltaTime) = 0;

	void						SetNode(Node *pNode);
	void						SetScene(Scene *pScene)			{ m_pScene = pScene; }
	virtual Block				*ToBlock()						= 0;
	
protected:
	virtual ~Object();

	void						SetObjectType(ObjectType dwType);

	Node						*m_pNode;						//the object's node
	Scene						*m_pScene;						//the scene this object is in

private:
	void						Init();

	ObjectType					m_dwObjectType;

};

#endif