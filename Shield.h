#ifndef _SHIELD_
#define _SHIELD_

#include "Effect.h"

#include "Def.h"

class Block;
class Node;

class Shield : public Effect
{
public:
	Shield(Node *pNode);

	void				Clone(Shield *pClone);
	void				FromBlock(Block *pBlock);
	const char			*GetClassName() const		{ return "Shield"; }
	VECTOR3				GetColor() const			{ return m_vColor; }
	void				Ping();
	void				Refresh(const float &fDeltaTime);
	void				Render();
	void				SetColor(const VECTOR3 &vColor);
	void				SetNode(Node *pNode);
	Block				*ToBlock();

protected:

	bool				m_bActive;
	float				m_fCurrentFade;
	float				m_fFadeTime;
	VECTOR3				m_vColor;

	Node				*m_pNode; //node this is shielding...

private:
	virtual ~Shield();

	void				Init();

};

#endif