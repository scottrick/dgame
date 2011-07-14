#include "SparkEffect.h"

#include "Matrix44.h"
#include "Node.h"
#include "ParticleManager.h"
#include "PosQuat.h"
#include "TextureManager.h"
#include "utility.h"

#include <GL/glut.h>

#define NUM_PARTICLES_ACTUALLY_USED (NUM_PARTICLES_PER_BATCH / 2)

SparkEffect::SparkEffect(const float &fScale, Node *pSourceNode, VECTOR3 vDir)
{
	Init();

	m_fScale	= fScale;
	if (m_fScale < 0.35f)
	{
		m_fScale = 0.35f;
	}

	m_vSource	= pSourceNode->m_PosQuat.pos;
	m_vDir.x	= -vDir.x; //dir is opposite of where the object was traveling.
	m_vDir.y	= -vDir.y;
	m_vDir.z	= 0.0f;
	NormalizeVECTOR3(m_vDir);

	SetBatch(gParticleManager()->RequestBatch());

	Start();
}

SparkEffect::~SparkEffect()
{

}

void SparkEffect::FromBlock(Block *pBlock)
{

}

void SparkEffect::Init()
{
	m_vSource		= VECTOR3(0.0f, 0.0f, 0.0f);
}
	
Block *SparkEffect::ToBlock()
{
	return 0;
}

void SparkEffect::Render()
{
	if (!m_pBatch)
	{
		return;
	}
	
	static int dwSparkTexture	= gTextureManager()->GetTextureByFileName("puff.tga");
	glBindTexture(GL_TEXTURE_2D, dwSparkTexture);

	//draw particles...
	for (int i = 0; i < NUM_PARTICLES_ACTUALLY_USED; i++)
	{
		particle &part = m_pBatch->parts[i];

		glPushMatrix();

		float fSparkScale = m_fScale * 0.4f;

		//translate to correct position
		glTranslatef(part.pq.pos.x, part.pq.pos.y, part.pq.pos.z);

		//scale particle correctly
		glScalef(fSparkScale, fSparkScale, fSparkScale);

		float fColorScale = part.fEnergy / part.fMaxEnergy;
		if (fColorScale < 0.5f)
		{
			fColorScale *= 2.0f;
		}
		else
		{
			fColorScale = 1.0f;
		}
		glColor3f(part.vColor.x * fColorScale, part.vColor.y * fColorScale, part.vColor.z * fColorScale);

		glCallList(m_dwTexturedQuadDisplayList);

		glPopMatrix();
	}
}

void SparkEffect::Start()
{
	if (!m_pBatch) 
	{
		return;
	}

	for (int i = 0; i < NUM_PARTICLES_ACTUALLY_USED; i++)
	{
		particle &part		= m_pBatch->parts[i];

		part.pq.pos			= m_vSource;
		part.fMaxEnergy		= GenerateRandomFloat(0.3f) + 0.2f;
		part.fEnergy		= part.fMaxEnergy;
		part.vColor.x		= 1.0f;
		part.vColor.y		= GenerateRandomFloat(1.0f);

		if (GenerateRandomInt(1))
		{
			part.vColor.z = part.vColor.y;
		}
		else
		{
			part.vColor.z = 0.0f;
		}

		float fRot	= GenerateRandomFloat(PI / 4.0f) - PI / 8.0f; //random direction 
		fRot += atan(m_vDir.y / m_vDir.x);

		if (m_vDir.x < 0.0f)
		{
			fRot += PI;
		}

		float fPrecalc = m_fScale * 10.0f * part.fMaxEnergy / 0.5f;
		part.vVelocity.x	= cos(fRot) * fPrecalc;
		part.vVelocity.y	= sin(fRot) * fPrecalc;
	}
}