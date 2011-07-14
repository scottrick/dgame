#include "Manager.h"

#include <cstdlib>
#include <iostream>
using namespace std;

#include <GL/glut.h>

Manager::Manager()
{
	Init();
}

Manager::~Manager()
{

}

void Manager::Init()
{
	m_dwSelectedIndex = 0;
}

void Manager::RenderHelpText(float fRatio)
{
	size_t size = 200;
	char *pDetails = (char *)malloc(sizeof(char) * size);
	memset(pDetails, 0, size);
	sprintf_s(pDetails, size, "Use the PLUS '+' and MINUS '-' keys to change through the different resources.");

	glRasterPos2f(-fRatio, 0.87f);
	for (const char *c = pDetails; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
	}

	memset(pDetails, 0, size);
	sprintf_s(pDetails, size, "Use the LEFT '[' and RIGHT ']' BRACKET keys to change the zoom.");
	
	glRasterPos2f(-fRatio, 0.82f);
	for (const char *c = pDetails; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
	}

	memset(pDetails, 0, size);
	sprintf_s(pDetails, size, "DRAG the MOUSE to rotate.");
	
	glRasterPos2f(-fRatio, 0.77f);
	for (const char *c = pDetails; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
	}

	memset(pDetails, 0, size);
	sprintf_s(pDetails, size, "CTRL + T changes to TEXTURES.  CTRL + V changes to visuals.");
	
	glRasterPos2f(-fRatio, 0.67f);
	for (const char *c = pDetails; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
	}

	memset(pDetails, 0, size);
	sprintf_s(pDetails, size, "Press R to reset the scale and rotation.");
	
	glRasterPos2f(-fRatio, 0.62f);
	for (const char *c = pDetails; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
	}

	free(pDetails);	
}