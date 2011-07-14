#ifndef _TEST_NODE_
#define _TEST_NODE_

#include "Node.h"


class TestNode : public Node
{
public:

	TestNode();

	virtual const char			*GetClassName() const					{ return "TestNode"; }
	virtual void				Render();

protected:

	virtual ~TestNode();

private:

	void						Init();


};

#endif