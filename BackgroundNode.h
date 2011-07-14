#ifndef _BACKGROUND_NODE_
#define _BACKGROUND_NODE_

#include "Node.h"

class BackgroundNode : public Node
{
public:
	BackgroundNode();

	virtual void					Render();
	
protected:
	virtual ~BackgroundNode();
	
private:
	void							Init();

};

#endif