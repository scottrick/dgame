#ifndef _TEXTVISUAL_
#define _TEXTVISUAL_

#include "def.h"
#include "Visual.h"

/*
	A TextVisual is an object which stores the mesh information about one text character.
	So, ONE character 3DS file will be stored in ONE TextVisual object.

	A TextVisual differs from a regular Visual object in that it does not automatically scale
	and translate the model to be at 0,0 and in the 1x1x1 box.  Also the normals are reversed 
	because I couldn't figure out how to get Blender to output them correctly.
*/
class TextVisual : public Visual
{
public:

	TextVisual();

	virtual void					Print(int indent) const;
	
	virtual void					RenderSlow();						//render without display lists!

protected:

	virtual							~TextVisual();

private:

	void							Init();

};

#endif