#include "ExplosionEffect.h"

#include "Block.h"
#include "Matrix44.h"
#include "Node.h"
#include "ParticleManager.h"
#include "TextureManager.h"
#include "utility.h"

#include <GL/glut.h>

int ExplosionEffect::m_dwNumSmokeParticles = NUM_PARTICLES_PER_BATCH / 4;

ExplosionEffect::ExplosionEffect(const float &fScale, Node *pSourceNode)
{
	Init();

	m_fScale = fScale;
	SetSourceNode(pSourceNode);
	SetBatch(gParticleManager()->RequestBatch());

	Start();
}

ExplosionEffect::~ExplosionEffect()
{
	SetSourceNode(0);
}

void ExplosionEffect::FromBlock(Block *pBlock)
{
	
}

void ExplosionEffect::Init()
{
	m_pSourceNode	= 0;
	m_vWind			= VECTOR3(7.0f, 28.0f, 0.0f);
}

void ExplosionEffect::Render()
{
	if (!m_pBatch)
	{
		return;
	}
	
	static int dwFireTexture1	= gTextureManager()->GetTextureByFileName("fire_1.tga");
	static int dwFireTexture2	= gTextureManager()->GetTextureByFileName("fire_2.tga");
	static int dwFireTexture3	= gTextureManager()->GetTextureByFileName("fire_3.tga");
	static int dwSmokeTexture1	= gTextureManager()->GetTextureByFileName("smoke_1.tga");
	static int dwSmokeTexture2	= gTextureManager()->GetTextureByFileName("smoke_2.tga");
	static int dwSmokeTexture3	= gTextureManager()->GetTextureByFileName("smoke_3.tga");

	int	dwCurrentTexture		= -1;
	int dwNumFirePerTexture		= (NUM_PARTICLES_PER_BATCH - m_dwNumSmokeParticles) / 3;
	int dwNumSmokePerTexture	= m_dwNumSmokeParticles / 3;
	int i;

	glColor3f(1.0f, 1.0f, 1.0f);

	//draw fire particles first...
	for (i = 0; i < NUM_PARTICLES_PER_BATCH - m_dwNumSmokeParticles; i++)
	{
		if (i > dwNumFirePerTexture * 2)
		{
			if (dwCurrentTexture != dwFireTexture3)
			{
				dwCurrentTexture = dwFireTexture3;
				glBindTexture(GL_TEXTURE_2D, dwCurrentTexture);
			}
		}
		else if (i > dwNumFirePerTexture)
		{
			if (dwCurrentTexture != dwFireTexture2)
			{
				dwCurrentTexture = dwFireTexture2;
				glBindTexture(GL_TEXTURE_2D, dwCurrentTexture);
			}
		}
		else if (dwCurrentTexture != dwFireTexture1)
		{
			dwCurrentTexture = dwFireTexture1;
			glBindTexture(GL_TEXTURE_2D, dwCurrentTexture);
		}

		particle &part = m_pBatch->parts[i];

		if (part.fEnergy > 0.0f)
		{
			glPushMatrix();

			//translate to correct position
			glTranslatef(part.pq.pos.x, part.pq.pos.y, part.pq.pos.z);

			//rotate the particle correctly
			glMultMatrixf(part.pq.quat.ToMatrix().m);

			float fPartScale = 0.4f * part.fMaxEnergy; //temp storage so we dont have to calc twice
			if (part.fEnergy < fPartScale)
			{
				fPartScale = part.fEnergy / fPartScale;	
				fPartScale *= m_fScale;
			}
			else
			{
				fPartScale = m_fScale;
			}
			glScalef(fPartScale, fPartScale, fPartScale);

			glCallList(m_dwTexturedQuadDisplayList);

			glPopMatrix();
		}
	}

	int k = i;

	//draw smoke particles...
	for (; i < NUM_PARTICLES_PER_BATCH; i++)
	{
		if (i - k > dwNumSmokePerTexture * 2)
		{
			if (dwCurrentTexture != dwSmokeTexture3)
			{
				dwCurrentTexture = dwSmokeTexture3;
				glBindTexture(GL_TEXTURE_2D, dwCurrentTexture);
			}
		}
		else if (i - k > dwNumSmokePerTexture)
		{
			if (dwCurrentTexture != dwSmokeTexture2)
			{
				dwCurrentTexture = dwSmokeTexture2;
				glBindTexture(GL_TEXTURE_2D, dwCurrentTexture);
			}
		}
		else if (dwCurrentTexture != dwSmokeTexture1)
		{
			dwCurrentTexture = dwSmokeTexture1;
			glBindTexture(GL_TEXTURE_2D, dwCurrentTexture);
		}

		particle &part = m_pBatch->parts[i];

		if (part.fEnergy > 0.0f)
		{
			glPushMatrix();

			float fSmokeScale = m_fScale * 1.8f;

			//translate to correct position
			glTranslatef(part.pq.pos.x, part.pq.pos.y, part.pq.pos.z);

			//rotate the particle correctly
			glMultMatrixf(part.pq.quat.ToMatrix().m);

			//scale particle correctly
			glScalef(fSmokeScale, fSmokeScale, fSmokeScale);

			float fPartScale = 0.4f * part.fMaxEnergy; //temp storage so we dont have to calc twice
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

void ExplosionEffect::SetSourceNode(Node *pSourceNode)
{
	if (m_pSourceNode != pSourceNode)
	{
		if (m_pSourceNode)
		{
			m_pSourceNode->Release();
			m_pSourceNode = 0;
		}

		if (pSourceNode)
		{
			m_pSourceNode = pSourceNode;
			m_pSourceNode->AddRef();
		}
	}
}

void ExplosionEffect::Start()
{	
	if (!m_pBatch) 
	{
		return;
	}

	int i;

	//fire particles...
	for (i = 0; i < NUM_PARTICLES_PER_BATCH - m_dwNumSmokeParticles; i++)
	{
		particle &part = m_pBatch->parts[i];

		part.pq.pos			= m_pSourceNode->m_PosQuat.pos;
		float fMax			= GenerateRandomFloat(3.0f) + 3.0f;
		part.vVelocity.x	= GenerateRandomFloat(6.0f);
		part.vVelocity.y	= fMax - part.vVelocity.x;
		part.fMaxEnergy		= GenerateRandomFloat(0.65f) + 0.15f;
		part.fSecondsPerRot	= GenerateRandomFloat(1.6f) + 0.6f;

		part.vVelocity.x	*= this->m_fScale;
		part.vVelocity.y	*= this->m_fScale;

		if (GenerateRandomInt(1))
		{
			part.fSecondsPerRot	= -part.fSecondsPerRot;
		}
		if (GenerateRandomInt(1))
		{
			part.vVelocity.x	= - part.vVelocity.x;
		}
		if (GenerateRandomInt(1))
		{
			part.vVelocity.y	= - part.vVelocity.y;
		}

		part.fEnergy		= part.fMaxEnergy;
		part.vAcceleration	= m_vWind;
	}

	//smoke particles...
	for (; i < NUM_PARTICLES_PER_BATCH; i++)
	{
		particle &part = m_pBatch->parts[i];

		part.pq.pos			= m_pSourceNode->m_PosQuat.pos;
		float fMax			= GenerateRandomFloat(3.0f) + 3.0f;
		part.vVelocity.x	= GenerateRandomFloat(6.0f);
		part.vVelocity.y	= fMax - part.vVelocity.x;
		part.fMaxEnergy		= GenerateRandomFloat(0.6f) + 0.5f;
		part.fSecondsPerRot	= GenerateRandomFloat(2.0f) + 1.0f;

		part.vVelocity.x	*= this->m_fScale;
		part.vVelocity.y	*= this->m_fScale;

		if (GenerateRandomInt(1))
		{
			part.fSecondsPerRot	= -part.fSecondsPerRot;
		}
		if (GenerateRandomInt(1))
		{
			part.vVelocity.x	= - part.vVelocity.x;
		}
		if (GenerateRandomInt(1))
		{
			part.vVelocity.y	= - part.vVelocity.y;
		}

		part.fEnergy		= part.fMaxEnergy;
		part.vAcceleration	= m_vWind;
	}
}

Block *ExplosionEffect::ToBlock()
{
	return 0;
}