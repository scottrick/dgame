#ifndef _FILE_
#define _FILE_

#include "Unknown.h"

#include <string>
#include <vector>
using namespace std;

class File : public Unknown
{
public:
	File();
	File(const char *pFile);
	
	bool								EnumerateContents(bool bRecursive = false); //populates the contents vector with the contents of the directory
	File								*GetChild(unsigned int index);
	virtual const char					*GetClassName() const			{ return "File"; }
	string								GetExtension();
	string								GetName() const;
	unsigned int						GetNumChildren();
	File								*GetParent()					{ return m_pParent; }
	bool								IsDirectory() const				{ return m_bDirectory; }
	bool								IsHidden() const				{ return m_bHidden; }
	bool								IsReadyOnly() const				{ return m_bReadOnly; }
	virtual void						Print(int indent) const;
	void								SetParent(File *pNewParent);
	void								Test();

private:

	virtual	~File();
	
	File								*AddChild(const char *pName); //returns the File pointer added
	void								Init();

	bool								m_bDirectory;
	bool								m_bHidden;
	bool								m_bReadOnly;

	File								*m_pParent;
	string								m_sName;

	vector<File *>						m_vChildren;

};

#endif