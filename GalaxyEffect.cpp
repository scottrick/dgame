#include "GalaxyEffect.h"

#include "Block.h"
#include "Matrix44.h"
#include "TextureManager.h"
#include "utility.h"

#include <GL/glut.h>

GalaxyEffect::GalaxyEffect()
{
	Init();
	Generate(); //auto generate a random galaxy effect!
}

GalaxyEffect::~GalaxyEffect()
{

}

void GalaxyEffect::FromBlock(Block *pBlock)
{

}

void GalaxyEffect::Generate()
{
	if (GenerateRandomInt(1)) {
		m_bClockwise = true;
	}
	else {
		m_bClockwise = false;
	}

	int dwRange = MAX_NUMBER_OF_LAYERS - MIN_NUMBER_OF_LAYERS;
	int dwNumLayers = GenerateRandomInt(dwRange) + MIN_NUMBER_OF_LAYERS;

	for (int i = 0; i < dwNumLayers; i++)
	{
		galLayer newLayer;

		if (i == 0)
		{ //first layer is always a center texture, otherwise get a arm texture
			newLayer.m_dwTexture = GenerateRandomInt(2);
			switch (newLayer.m_dwTexture)
			{
			case 0:
				newLayer.m_dwTexture = gTextureManager()->GetTextureByFileName("center_1.tga");
				break;

			case 1:
				newLayer.m_dwTexture = gTextureManager()->GetTextureByFileName("center_2.tga");
				break;

			case 2:
				newLayer.m_dwTexture = gTextureManager()->GetTextureByFileName("center_3.tga");
				break;

			default:
				printf("GalaxyEffect::Generate() --> Invalid center texture!\n");
				break;
			}
		}
		else
		{
			newLayer.m_dwTexture = GenerateRandomInt(9);
			switch (newLayer.m_dwTexture)
			{
			case 0:
				newLayer.m_dwTexture = gTextureManager()->GetTextureByFileName("arms_1.tga");
				break;

			case 1:
				newLayer.m_dwTexture = gTextureManager()->GetTextureByFileName("arms_2.tga");
				break;

			case 2:
				newLayer.m_dwTexture = gTextureManager()->GetTextureByFileName("arms_3.tga");
				break;

			case 3:
				newLayer.m_dwTexture = gTextureManager()->GetTextureByFileName("arms_4.tga");
				break;
				
			case 4:
				newLayer.m_dwTexture = gTextureManager()->GetTextureByFileName("arms_5.tga");
				break;

			case 5:
				newLayer.m_dwTexture = gTextureManager()->GetTextureByFileName("arms_6.tga");
				break;

			case 6:
				newLayer.m_dwTexture = gTextureManager()->GetTextureByFileName("arms_7.tga");
				break;

			case 7:
				newLayer.m_dwTexture = gTextureManager()->GetTextureByFileName("arms_8.tga");
				break;

			case 8:
				newLayer.m_dwTexture = gTextureManager()->GetTextureByFileName("arms_9.tga");
				break;

			case 9:
				newLayer.m_dwTexture = gTextureManager()->GetTextureByFileName("arms_10.tga");
				break;

			default:
				printf("GalaxyEffect::Generate() --> Invalid arm texture!\n");
				break;		
			}
		}

		float fMaxColor = 5.0f / ((float)dwNumLayers + 2.0f);

		newLayer.m_fSecondsPerRotation	= GenerateRandomFloat(18.0f) + 9.0f;
		newLayer.m_vColor				= VECTOR3(GenerateRandomFloat(fMaxColor), GenerateRandomFloat(fMaxColor), GenerateRandomFloat(fMaxColor));
		float fRotation					= GenerateRandomFloat(2.0f * PI);
		newLayer.m_qRotation.CreateFromRotationRADIANS(fRotation, 0.0f, 0.0f, 1.0f);

		this->m_lLayers.push_front(newLayer);
	}
}

void GalaxyEffect::Init()
{

}

void GalaxyEffect::Refresh(const float &fDeltaTime)
{
	float	fPercent;
	Quat	rotation;

	list<galLayer>::iterator iter;
	for (iter = m_lLayers.begin(); iter != m_lLayers.end(); iter++)
	{
		fPercent	= fDeltaTime / (*iter).m_fSecondsPerRotation;
		rotation.CreateFromRotationRADIANS(2.0f * PI * fPercent, 0.0f, 0.0f, 1.0f);
		(*iter).m_qRotation	= rotation * (*iter).m_qRotation;
	}
}

void GalaxyEffect::Render()
{
	glPushMatrix();

	if (!m_bClockwise)
	{
		Quat rot;
		rot.CreateFromRotationRADIANS(PI, 0.0f, 1.0f, 0.0f);
		glMultMatrixf(rot.ToMatrix().m);
	}

	list<galLayer>::iterator iter;
	for (iter = m_lLayers.begin(); iter != m_lLayers.end(); iter++)
	{
		glPushMatrix();
		glColor3f((*iter).m_vColor.x, (*iter).m_vColor.y, (*iter).m_vColor.z);
		glMultMatrixf((*iter).m_qRotation.ToMatrix().m);
		glBindTexture(GL_TEXTURE_2D, (*iter).m_dwTexture);
		glCallList(m_dwTexturedQuadDisplayList);
		glPopMatrix();
	}

	glPopMatrix();
}

Block *GalaxyEffect::ToBlock()
{
	return 0;
}