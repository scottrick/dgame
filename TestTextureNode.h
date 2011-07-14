#ifndef _TEST_TEXTURE_NODE_
#define _TEST_TEXTURE_NODE_

#include "Node.h"

class TestTextureNode : public Node
{
public:

	TestTextureNode();

	virtual const char			*GetClassName() const					{ return "TestTextureNode"; }
	virtual void				Render();

protected:

	virtual						~TestTextureNode();

private:

	int							m_dwTexture;

	void						Init();

};

#endif