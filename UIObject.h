#ifndef _UI_OBJECT_
#define _UI_OBJECT_

#include "def.h"
#include "Object.h"

class UIObject : public Object
{
public:
	UIObject()														{ Init(); }

	const VECTOR3&				GetPosition() const					{ return m_vPosition; }
	const VECTOR3&				GetScale() const					{ return m_vScale; }
	virtual void				HandleInput(const unsigned char &key, const int &x, const int &y, const unsigned int &dwKeyboardFlags) {}; //override!
	virtual void				Render()							= 0;
	void						SetHasFocus(bool bNewFocus)			{ m_bHasFocus = bNewFocus; }
	void						SetPosition(const VECTOR3 &vPos)	{ m_vPosition = vPos; }
	void						SetScale(const VECTOR3 &vScale)		{ m_vScale = vScale; }	

protected:
	virtual ~UIObject()												{};

	bool						m_bHasFocus;

	VECTOR3						m_vPosition;
	VECTOR3						m_vScale;

private:
	void						Init();

};

#endif