#include "SplashEffect.h"

#include "Block.h"
#include "Matrix44.h"
#include "ParticleManager.h"
#include "TextureManager.h"
#include "utility.h"

#include <GL/glut.h>

SplashEffect::SplashEffect()
{
	Init();
}

SplashEffect::~SplashEffect()
{
	batch *pBatch = 0;

	for (int i = 0; i < NUM_BATCHES; i++)
	{
		pBatch = m_pBatches[i];

		if (pBatch)
		{
			gParticleManager()->ReturnBatch(pBatch);
			pBatch = 0;
		}
	}
}

void SplashEffect::FromBlock(Block *pBlock)
{

}

void SplashEffect::Init()
{
	m_pBatches = new batch*[NUM_BATCHES];	

	for (int i = 0; i < NUM_BATCHES; i++)
	{
		m_pBatches[i] = gParticleManager()->RequestBatch();
	}

	m_fFireDelay			= TIME_TO_FIRE_ALL / (float)(NUM_BATCHES * NUM_PARTICLES_PER_BATCH);
	m_fTimeSinceLastLaunch	= 0.0f;
	m_fSessionTimeLeft		= 0.4f;
	m_bClockwise			= false;
	m_bShooting				= false;

	Randomize();
}

void SplashEffect::Randomize()
{
	//randomize launch point movement speed
	m_fLaunchPointSpeed		= 1.0f + GenerateRandomFloat(12.0f);

	//randomize its rotation
	if (GenerateRandomInt(1))
	{
		m_bClockwise		= true;
	}
	else
	{
		m_bClockwise		= false;
	}

	//randomize launch points' starting location
	m_dwLoc					= GenerateRandomInt(3) + 1;
	switch (m_dwLoc)
	{
	case TOP:
		m_vLaunchPoint = VECTOR3(GenerateRandomFloat(10.0f) - 5.0f, 8.0f, 0.0f);
		break;

	case BOTTOM:
		m_vLaunchPoint = VECTOR3(GenerateRandomFloat(10.0f) - 5.0f, -8.0f, 0.0f);
		break;

	case LEFT:
		m_vLaunchPoint = VECTOR3(-10.0f, GenerateRandomFloat(8.0f) - 4.0f, 0.0f);
		break;

	case RIGHT:
		m_vLaunchPoint = VECTOR3(10.0f, GenerateRandomFloat(8.0f) - 4.0f, 0.0f);
		break;

	default:
		printf("ALERT: BAD LOC!  SplashEffect::Init()\n");
		break;
	}

}

void SplashEffect::Refresh(const float &fDeltaTime)
{
	m_fSessionTimeLeft -= fDeltaTime;

	if (m_fSessionTimeLeft <= 0.0f)
	{
		if (m_bShooting)
		{ //insert a small delay for coolness's sake
			m_fSessionTimeLeft = 0.5f + GenerateRandomFloat(2.0f);
			m_bShooting = false;
			m_bClockwise = !m_bClockwise; //reverse direction...
		}
		else
		{
			m_fSessionTimeLeft = 0.5f + GenerateRandomFloat(12.0f);
			m_bShooting = true;
		}
	}

	int dwNumToLaunch = 0;
	m_fTimeSinceLastLaunch += fDeltaTime;

	while (m_fTimeSinceLastLaunch > m_fFireDelay)
	{
		++dwNumToLaunch;
		m_fTimeSinceLastLaunch -= m_fFireDelay;
	}

	batch *pBatch = 0;
	for (int dwBatchNum = 0; dwBatchNum < NUM_BATCHES; dwBatchNum++)
	{
		pBatch = m_pBatches[dwBatchNum];

		if (pBatch)
		{
			for (int i = 0; i < NUM_PARTICLES_PER_BATCH; i++)
			{
				particle &part = pBatch->parts[i];

				if (part.fEnergy > 0.0f)
				{
					float fDistSquared = part.pq.pos.x * part.pq.pos.x + part.pq.pos.y * part.pq.pos.y;

					if (fDistSquared < 10.0f)
					{
						float fPercentOfForce = (1.0f - (fDistSquared / 10.0f)) * 45.0f;
						VECTOR3 vDir = VECTOR3(part.pq.pos.x, part.pq.pos.y, 0.0f);
						NormalizeVECTOR3(vDir);

						vDir.x *= fPercentOfForce;
						vDir.y *= fPercentOfForce;

						part.vAcceleration = vDir;
					}

					part.Update(fDeltaTime);
				}
				else if (dwNumToLaunch && m_bShooting)
				{
					part.vColor.x	= GenerateRandomFloat(1.0f);
					part.vColor.y	= GenerateRandomFloat(1.0f);
					part.vColor.z	= GenerateRandomFloat(1.0f);

					part.fMaxEnergy = TIME_TO_FIRE_ALL * 0.98f * (1.0f - GenerateRandomFloat(0.2f));
					part.fEnergy	= part.fMaxEnergy;
					
					part.pq.pos		= m_vLaunchPoint;
					
					VECTOR3	vDir	= VECTOR3(-m_vLaunchPoint.x, -m_vLaunchPoint.y, 0.0f);
					NormalizeVECTOR3(vDir);

					float fRot	= GenerateRandomFloat(PI / 25.0f) - PI / 50.0f; //small random angle
					fRot += atan(vDir.y / vDir.x);

					if (vDir.x <= 0.0f)
					{
						fRot += PI;
					}

					part.vVelocity.x	= cos(fRot) * 21.0f;
					part.vVelocity.y	= sin(fRot) * 21.0f;

					--dwNumToLaunch;
				}
			}
		}
	}

	switch (m_dwLoc)
	{
	case TOP:
		if (m_bClockwise)
		{
			m_vLaunchPoint.x += m_fLaunchPointSpeed * fDeltaTime;
			if (m_vLaunchPoint.x > 10.0f)
			{
				m_dwLoc = RIGHT;
			}
		}
		else
		{
			m_vLaunchPoint.x -= m_fLaunchPointSpeed * fDeltaTime;
			if (m_vLaunchPoint.x < -10.0f)
			{
				m_dwLoc = LEFT;
			}
		}	
		break;

	case BOTTOM:
		if (!m_bClockwise)
		{
			m_vLaunchPoint.x += m_fLaunchPointSpeed * fDeltaTime;
			if (m_vLaunchPoint.x > 10.0f)
			{
				m_dwLoc = RIGHT;
			}
		}
		else
		{
			m_vLaunchPoint.x -= m_fLaunchPointSpeed * fDeltaTime;
			if (m_vLaunchPoint.x < -10.0f)
			{
				m_dwLoc = LEFT;
			}
		}	
		break;		break;

	case LEFT:
		if (m_bClockwise)
		{
			m_vLaunchPoint.y += m_fLaunchPointSpeed * fDeltaTime;
			if (m_vLaunchPoint.y > 7.5f)
			{
				m_dwLoc = TOP;
			}
		}
		else
		{
			m_vLaunchPoint.y -= m_fLaunchPointSpeed * fDeltaTime;
			if (m_vLaunchPoint.y < -7.5f)
			{
				m_dwLoc = BOTTOM;
			}
		}	
		break;

	case RIGHT:
		if (!m_bClockwise)
		{
			m_vLaunchPoint.y += m_fLaunchPointSpeed * fDeltaTime;
			if (m_vLaunchPoint.y > 7.5f)
			{
				m_dwLoc = TOP;
			}
		}
		else
		{
			m_vLaunchPoint.y -= m_fLaunchPointSpeed * fDeltaTime;
			if (m_vLaunchPoint.y < -7.5f)
			{
				m_dwLoc = BOTTOM;
			}
		}	
		break;

	default:
		printf("ALERT: BAD LOC!  SplashEffect::Refresh()\n");
		break;
	}
}

void SplashEffect::Render()
{
	static int dwPuffTexture	= gTextureManager()->GetTextureByFileName("puff.tga");
	glBindTexture(GL_TEXTURE_2D, dwPuffTexture);

	batch *pBatch = 0;
	for (int dwBatchNum = 0; dwBatchNum < NUM_BATCHES; dwBatchNum++)
	{
		pBatch = m_pBatches[dwBatchNum];

		if (pBatch)
		{
			for (int i = 0; i < NUM_PARTICLES_PER_BATCH; i++)
			{
				particle &part = pBatch->parts[i];

				if (part.fEnergy > 0.0f)
				{
					glPushMatrix();

					float fColorScale = 1.0f;
					if (part.fEnergy < 0.8f * part.fMaxEnergy)
					{
						fColorScale = part.fEnergy / (0.8f * part.fMaxEnergy);
					}
					glColor3f(part.vColor.x * fColorScale, part.vColor.y * fColorScale, part.vColor.z * fColorScale);

					//translate to correct position
					glTranslatef(part.pq.pos.x, part.pq.pos.y, part.pq.pos.z);

					//rotate the particle correctly
					glMultMatrixf(part.pq.quat.ToMatrix().m);

					glCallList(m_dwTexturedQuadDisplayList);

					glPopMatrix();
				}
			}
		}
	}
}

Block *SplashEffect::ToBlock()
{
	return 0;
}