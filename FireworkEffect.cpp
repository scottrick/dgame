#include "FireworkEffect.h"

#include "Matrix44.h"
#include "ParticleManager.h"
#include "PosQuat.h"
#include "TextureManager.h"
#include "utility.h"

#include <GL/glut.h>

FireworkEffect::FireworkEffect(const float &fScale, const VECTOR3 &vPos)
{
	Init();

	m_fScale	= fScale;
	m_vSource	= vPos;
	SetBatch(gParticleManager()->RequestBatch());

	Start();
}

FireworkEffect::~FireworkEffect()
{

}

void FireworkEffect::FromBlock(Block *pBlock)
{

}

void FireworkEffect::Init()
{
	m_vSource		= VECTOR3(0.0f, 0.0f, 0.0f);
	m_vWind			= VECTOR3(0.33f, 2.33f, 0.0f);
	
	float r			= GenerateRandomFloat(1.0f);
	float g			= GenerateRandomFloat(1.0f);
	float b			= GenerateRandomFloat(1.0f);

	while (r + g + b < 1.0f)
	{
		r *= 2.0f;
		g *= 2.0f;
		b *= 2.0f;
	}
	
	if (r > 1.0f)
	{
		r = 1.0f;
	}
	if (g > 1.0f)
	{
		g = 1.0f;
	}
	if (b > 1.0f)
	{
		b = 1.0f;
	}

	m_vColor		= VECTOR3(r, g, b);
}
	
Block *FireworkEffect::ToBlock()
{
	return 0;
}

void FireworkEffect::Render()
{
	if (!m_pBatch)
	{
		return;
	}
	
	static int dwFireworkTexture	= gTextureManager()->GetTextureByFileName("firework_2.tga");
	glBindTexture(GL_TEXTURE_2D, dwFireworkTexture);
	glColor3f(m_vColor.x, m_vColor.y, m_vColor.z);

	//draw particles...
	for (int i = 0; i < NUM_PARTICLES_PER_BATCH; i++)
	{
		particle &part = m_pBatch->parts[i];

		if (part.fEnergy > 0.0f)
		{
			glPushMatrix();

			//translate to correct position
			glTranslatef(part.pq.pos.x, part.pq.pos.y, part.pq.pos.z);

			//rotate the particle correctly
			glMultMatrixf(part.pq.quat.ToMatrix().m);

			float fScaleMod = 1.0f;
			if (part.fEnergy > part.fMaxEnergy)
			{
				fScaleMod = 1.0f - (part.fEnergy - part.fMaxEnergy) / 0.15f;
			}
			
			//scale particle correctly
			float fScaleToUse = m_fScale * part.fScale * fScaleMod;
			glScalef(fScaleToUse, fScaleToUse, fScaleToUse);

			float fPartScale = part.fMaxEnergy; //temp storage so we dont have to calc twice
			if (part.fEnergy < fPartScale)
			{
				fPartScale = part.fEnergy / fPartScale;	
				glScalef(fPartScale, fPartScale, 1.0f);
			}

			glCallList(m_dwTexturedQuadDisplayList);

			glPopMatrix();
		}
	}
}

void FireworkEffect::Start()
{
	if (!m_pBatch) 
	{
		return;
	}

	int numArms				= GenerateRandomInt(1) + 3; //3 or 4 arms
	int numParticlesPerArm	= NUM_PARTICLES_PER_BATCH / numArms;
	int dwCurrentParticle	= 0;

	for (int dwArm = 1; dwArm <= numArms; dwArm++)
	{
		float	fAngle			= GenerateRandomFloat(2.0f * PI);		
		float	fXMod			= m_fScale * cos(fAngle) * 0.35f;
		float	fYMod			= m_fScale * sin(fAngle) * 0.35f;
		int		dwNumToLeave	= 0;
		int		k				= 1;
		
		if (dwArm != numArms)
		{ // not the last one, so leave some particles
			dwNumToLeave		= NUM_PARTICLES_PER_BATCH - (numParticlesPerArm * dwArm);
		}		

		for (; dwCurrentParticle < NUM_PARTICLES_PER_BATCH - dwNumToLeave; dwCurrentParticle++)
		{
			particle &part		= m_pBatch->parts[dwCurrentParticle];

			part.pq.pos			= m_vSource;
			part.pq.pos.x		+= fXMod * (float)k;
			part.pq.pos.y		+= fYMod * (float)k;
			part.fScale			= 1.0f - GenerateRandomFloat(0.40f);
			part.fMaxEnergy		= 1.75f;
			part.fEnergy		= part.fMaxEnergy + part.fMaxEnergy * 0.1f * (float)k / (float)numParticlesPerArm;
			part.fSecondsPerRot	= GenerateRandomFloat(7.0f) + 3.0f;

			if (GenerateRandomInt(1))
			{
				part.fSecondsPerRot	= -part.fSecondsPerRot;
			}

			part.vAcceleration	= m_vWind;
			k++;
		}
	}
}