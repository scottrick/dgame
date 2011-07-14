#ifndef _TEXTURE_MANAGER_
#define _TEXTURE_MANAGER_

#include "Manager.h"

#include <vector>
using namespace std;

class File;
class TextNode;
class Texture;

class TextureManager : public Manager
{
public:
	TextureManager();
	~TextureManager();

	void								AddTexture(Texture *pAdd);
	void								Cleanup();
	void								DebugInfoRender(const float &fRatio, const PosQuat &pq);
	void								DebugScreenRender();
	const char							*GetManagerName()							{ return "TextureManager"; }
	int									GetTextureByFileName(const char *pFileName);
	void								LoadTexture(const char *pFileName);
	void								LoadTextures();
	void								Refresh(const float &fDeltaTime);
	void								RemoveTexture(Texture *pRemove);
	void								SelectNext();
	void								SelectPrevious();

private:

	void								ClearTextNode();
	void								ClearTextures();
	void								Init();
	void								LoadAllTGA(File *pDirectory, bool bRecursive);

	bool								m_bTexturesLoaded;

	TextNode							*m_pTextNode;

	vector<Texture *>					m_vTextures;
	
};

extern TextureManager *gTextureManager();

#endif