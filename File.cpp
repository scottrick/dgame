#include "File.h"

#include <windows.h>
#include <fstream>
#include <iostream>
#include <tchar.h>
using namespace std;

File::File()
{
	m_sName = "\\.";
	Init();
}

File::File(const char *pFile)
{
	m_sName = pFile;
	Init();
}

File::~File()
{
	SetParent(0);

	File *pFile = 0;
	vector<File *>::iterator iter;
	for (iter = m_vChildren.begin(); iter != m_vChildren.end(); iter++)
	{
		pFile = *iter;
		if (pFile)
		{
			pFile->SetParent(0);
			pFile->Release();
			pFile = 0;
		}
	}

	m_vChildren.clear();
}

File *File::AddChild(const char *pName) {
	if ((strcmp(pName, ".") == 0) || (strcmp(pName, "..") == 0))
	{
		return 0; //don't add these guys!
	}

	string sChildPath = m_sName + "\\" + pName; //create the child path
	File *pChildFile = new File(sChildPath.c_str()); //starts with one addref, so dont need to add one
	pChildFile->SetParent(this);
	m_vChildren.push_back(pChildFile);

	return pChildFile;
}

/*
	If this file is a directory, creates a child File object for every file the directory contains
*/
bool File::EnumerateContents(bool bRecursive)
{
	if (m_bDirectory)
	{
		string		sDirPath = m_sName;
		size_t		numConverted;
		sDirPath.append("\\*");

		size_t		length	= strlen(sDirPath.c_str()) + 1;
		wchar_t		*pText	= new wchar_t[length];

		if (!pText)
		{
			return false;
		}
		memset(pText, 0, length);
	
		//Convert the string into a wchar_t so it can be used
		mbstowcs_s(&numConverted, pText, length, sDirPath.c_str(), length);

		WIN32_FIND_DATA		data;
		HANDLE				hFind			= FindFirstFile(pText, &data);
		char				*pConverted		= 0;
		File				*pNewChild		= 0;
 
		delete []pText; //free the wchar_t
			
		if (hFind != INVALID_HANDLE_VALUE) 
		{ //found the directory!
			
			//Add the first child 
			length = wcslen(data.cFileName) + 1;
			pConverted = new char[length];
			memset(pConverted, 0, length);

			wcstombs_s(&numConverted, pConverted, length, data.cFileName, length);
			pNewChild = AddChild(pConverted);
			if (bRecursive && pNewChild)
			{
				pNewChild->EnumerateContents(bRecursive);
			}
			delete []pConverted;

			while (FindNextFile(hFind, &data) != 0) //add the rest of the children
			{
				length = wcslen(data.cFileName) + 1;
				pConverted = new char[length];
				memset(pConverted, 0, length);

				wcstombs_s(&numConverted, pConverted, length, data.cFileName, length);
				pNewChild = AddChild(pConverted);
				if (bRecursive && pNewChild)
				{
					pNewChild->EnumerateContents(bRecursive);
				}
				delete []pConverted;
			}
		}

		FindClose(hFind);

		return true;
	}
	else
	{
		return false;
	}
}

File *File::GetChild(unsigned int index)
{
	if (index < m_vChildren.size())
	{
		return m_vChildren.at(index);
	}
	else
	{
		return 0;
	}
}

string File::GetExtension()
{
	size_t decimal = m_sName.find_last_of(".", m_sName.length());
	
	if (decimal == string::npos)
	{
		return string("");
	}
	else
	{
		decimal++;
		return m_sName.substr(decimal, m_sName.length() - decimal);
	}
}

string File::GetName() const
{
	return m_sName;
}

unsigned int File::GetNumChildren()
{
	return (unsigned int)m_vChildren.size();
}

void File::Init()
{
	m_bDirectory		= false;
	m_bHidden			= false;
	m_bReadOnly			= false;
	m_pParent			= 0;

	//Convert the string into a wchar_t 
	int			length	= (int)strlen(m_sName.c_str()) + 1;
	wchar_t		*pText	= new wchar_t[length];
	if (!pText)
	{
		return;	
	}
	memset(pText, 0, length);
	MultiByteToWideChar(CP_ACP, 0, m_sName.c_str(), -1, pText, length);

	//get the basic file info
	WIN32_FIND_DATA		data;
	HANDLE				hFind	= FindFirstFile(pText, &data);

	if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		m_bDirectory		= true;
	}
	if (data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
	{
		m_bHidden			= true;
	}
	if (data.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
	{
		m_bReadOnly			= true;
	}

	delete []pText;
	FindClose(hFind);
}

void File::Print(int indent) const
{
	printf("%*sFile 0x%p (%s)\n", indent, "", this, GetName().c_str());
	//printf("%*s  Directory: %d, Hidden: %d, ReadOnly: %d\n", indent, " ", m_bDirectory, m_bHidden, m_bReadOnly);
	
	File *pFile = 0;
	vector<File *>::const_iterator iter;
	for (iter = m_vChildren.begin(); iter != m_vChildren.end(); iter++)
	{
		pFile = *iter;
		if (pFile)
		{
			pFile->Print(indent + 2);
		}
	}
}

void File::SetParent(File *pNewParent)
{
	if (pNewParent != m_pParent)
	{
		if (m_pParent)
		{ //get rid of the current parent
			m_pParent = 0; 
		}
		
		if (pNewParent)
		{
			m_pParent = pNewParent;
		}
	}
}

void File::Test()
{
	ifstream file(m_sName.c_str(), ios::binary);	
	double value;

	file.read((char *)(&value), 3*sizeof(char));

	while (file.good())
	{
		file.read((char *)(&value), sizeof(double));
		printf("%f\n", value);
	}

	return;

	WIN32_FIND_DATA		data;
	HANDLE				hFind		= FindFirstFile(TEXT("data\\textures\\*.tga"), &data);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		_tprintf(TEXT(" %s\n"), data.cFileName);
		while (FindNextFile(hFind, &data) != 0)
		{
			_tprintf(TEXT(" %s\n"), data.cFileName);
		}
	}

	FindClose(hFind);
}