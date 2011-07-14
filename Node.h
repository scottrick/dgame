#ifndef _NODE_
#define _NODE_				1

#include "PosQuat.h"
#include "Unknown.h"
#include "Visual.h"

#include <vector>
using namespace std;

#define							NODE_DO_NOT_RENDER			(1 << 1)
#define							NODE_RENDER_COLLISION_DEBUG	(1 << 2)
#define							NODE_RENDER_UPSIDEDOWN		(1 << 3)

/*
	A Node is something that can be rendered on the screen.
*/
class Node : public Unknown
{
public:
	Node();

	bool						SetVisual(const char *pFile);
	void						SetVisual(Visual *pVisual);
	virtual const char			*GetClassName() const					{ return "Node"; }
	unsigned int				GetNodeFlags() const					{ return m_dwNodeFlags; }
	VECTOR3						GetScale() const						{ return m_vScale; }
	Visual						*GetVisual()							{ return m_pVisual; }
	virtual void				Print(int indent) const;
	void						RaiseNodeFlags(unsigned int dwFlags);
	void						RemoveNodeFlags(unsigned int dwFlags);
	virtual void				Render();
	void						SetScale(VECTOR3 vScale);

	PosQuat						m_PosQuat;
	VECTOR3						m_vScale;

	unsigned int				m_dwDebugX;
	unsigned int				m_dwDebugY;
	unsigned int				m_dwBucket;

protected:
	virtual	~Node();

	unsigned int				m_dwNodeFlags;

	//vector<Visual *>			m_vVisuals; 
	Visual						*m_pVisual; //visual for this node 

private:
	void						Init();

	//gonna need a list of children nodes...


};

#endif