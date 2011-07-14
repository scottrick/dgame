#include "ActionScene.h"

#include "Background.h"
#include "CollisionManager.h"
#include "ComputerShip.h"
#include "DrawBuffer.h"
#include "EffectManager.h"
#include "EventManager.h"
#include "Game.h"
#include "GameCamera.h"
#include "HumanFiringPlugin.h"
#include "HumanMovementPlugin.h"
#include "Manager.h"
#include "Matrix44.h"
#include "Node.h"
#include "ObjectManager.h"
#include "Obstacle.h"
#include "Player.h"
#include "PlayerShip.h"
#include "Sector.h"
#include "ShaderManager.h"
#include "ShaderProgram.h"
#include "Shield.h"
#include "Text.h"
#include "TextureManager.h"
#include "UIContainer.h"
#include "UnknownManager.h"
#include "utility.h"
#include "VisualManager.h"

ActionScene::ActionScene()
{
	Init();
}

ActionScene::~ActionScene()
{
	SetPlayerObject(0);
	SetSector(0);
}

void ActionScene::DrawPlayerInfo()
{
	if (!m_pPlayerObject)
	{
		return;
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	GLboolean bTextureWasOn = glIsEnabled(GL_TEXTURE_2D);
	GLboolean bLightingWasOn = glIsEnabled(GL_LIGHTING);
	
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D); //disable textures when rendering the text

	float fBarSizeX			= 0.48f;
	float fBarSizeY			= 0.1f;
	float fBarStartX		= -0.99f;
	float fBarStartY		= 0.87f;
	float fBarPctStruct		= m_pPlayerObject->GetStructure() / m_pPlayerObject->GetStructureMax();
	float fBarPctShield		= m_pPlayerObject->GetShields() / m_pPlayerObject->GetShieldsMax();

	glBegin(GL_LINE_LOOP);
	glVertex3f(fBarStartX, fBarStartY, 0.0f);
	glVertex3f(fBarStartX + fBarSizeX, fBarStartY, 0.0f);
	glVertex3f(fBarStartX + fBarSizeX, fBarStartY - (fBarSizeY - 0.01f), 0.0f);
	glVertex3f(fBarStartX, fBarStartY - (fBarSizeY - 0.01f), 0.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex3f(fBarStartX, fBarStartY - fBarSizeY, 0.0f);
	glVertex3f(fBarStartX + fBarSizeX, fBarStartY - fBarSizeY, 0.0f);
	glVertex3f(fBarStartX + fBarSizeX, fBarStartY - (fBarSizeY * 2.0f - 0.01f), 0.0f);
	glVertex3f(fBarStartX, fBarStartY - (fBarSizeY * 2.0f - 0.01f), 0.0f);
	glEnd();

	glColor3f(0.8f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(fBarStartX, fBarStartY, 0.0f);
	glVertex3f(fBarStartX + fBarSizeX * fBarPctStruct, fBarStartY, 0.0f);
	glVertex3f(fBarStartX + fBarSizeX * fBarPctStruct, fBarStartY - (fBarSizeY - 0.01f), 0.0f);
	glVertex3f(fBarStartX, fBarStartY - (fBarSizeY - 0.01f), 0.0f);
	glEnd();
	
	glColor3f(0.0f, 0.0f, 0.8f);
	glBegin(GL_QUADS);
	glVertex3f(fBarStartX, fBarStartY - fBarSizeY, 0.0f);
	glVertex3f(fBarStartX + fBarSizeX * fBarPctShield, fBarStartY - fBarSizeY, 0.0f);
	glVertex3f(fBarStartX + fBarSizeX * fBarPctShield, fBarStartY - (fBarSizeY * 2.0f - 0.01f), 0.0f);
	glVertex3f(fBarStartX, fBarStartY - (fBarSizeY * 2.0f - 0.01f), 0.0f);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	//draw sector name
	glRasterPos2f(-0.99f, 0.92f);
	for (const char *c = m_pSector->GetSectorName(); *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
	}

	const size_t bufferSize = 50;
	char killBuffer[bufferSize];
	sprintf_s(killBuffer, bufferSize, "%d objects destroyed", m_pPlayerObject->GetNumKills());

	//draw kill count!
	glRasterPos2f(-0.99f, 0.6f);
	for (const char *c = killBuffer; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
	}

	//draw # live objects
	sprintf_s(killBuffer, bufferSize, "%d live objects", this->GetNumObjects());
	glRasterPos2f(-0.99f, 0.52f);
	for (const char *c = killBuffer; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
	}

	sprintf_s(killBuffer, bufferSize, "%d post render effects", this->GetNumPostRenderEffects());
	glRasterPos2f(-0.99f, 0.44f);
	for (const char *c = killBuffer; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
	}

	if (bTextureWasOn)
	{
		glEnable(GL_TEXTURE_2D);
	}
	if (bLightingWasOn)
	{
		glEnable(GL_LIGHTING);
	}
}

void ActionScene::Init()
{
	m_pPlayerObject = 0;
	m_pSector		= 0;
}

void ActionScene::Print(int indent) const
{
	printf("%*sActionScene 0x%p\n", indent, " ", this);
	Scene::Print(indent);
}

void ActionScene::RefreshManagers(const float &fDeltaTime)
{
	Scene::RefreshManagers(fDeltaTime);

	gCollisionManager()->Refresh(fDeltaTime);
}

void ActionScene::Render(Manager *pDebugManager)
{
	const VECTOR2_INT *pGameWindowSize		= gGame()->GetGameWindowSize();
	const VECTOR2_INT *pGameWindowStart		= gGame()->GetGameWindowStart();
	bool bPostProcessing					= (GetNumPostRenderEffects() > 0) && (gShaderManager()->IsShadersSupported());

	gShaderManager()->UseDefaultProgram();  //render scene with normal program

	if (bPostProcessing) {
		gGame()->GetSceneBuffer()->Bind();	//set target buffer
	}
	else {
		DrawBuffer::BindDefault();
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	if (pDebugManager)
	{
		RenderDebug(pDebugManager);
	}

	if (bPostProcessing) {
		glViewport(pGameWindowSize->x / 4, 0, pGameWindowSize->x * 3 / 4, pGameWindowSize->y);
	}
	else {
		//game is on the 3/4 right part of the screen
		glViewport(pGameWindowStart->x + pGameWindowSize->x / 4, pGameWindowStart->y, pGameWindowSize->x * 3 / 4, pGameWindowSize->y);
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.0, 0.1, 50.0); //update this so we dont get distortion
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	if (m_pCamera)
	{	
		GLfloat fLightPos[] = { m_pCamera->GetPosQuat().pos.x, m_pCamera->GetPosQuat().pos.y, m_pCamera->GetPosQuat().pos.z, 1.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, fLightPos);
		//printf("X %f, %f, %f, %f\n", m_pCamera->GetPosQuat().pos.x, m_pCamera->GetPosQuat().pos.y, m_pCamera->GetPosQuat().pos.z);

		//rotate to camera orientation
		glMultMatrixf(m_pCamera->GetPosQuat().quat.ToMatrix().ToRotationalInverse().m);
		//translate to the camera position
		glTranslatef(-(m_pCamera->GetPosQuat().pos.x), -(m_pCamera->GetPosQuat().pos.y), -(m_pCamera->GetPosQuat().pos.z));
	}
	else
	{ //no camera, so some default orientation
		gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	}

	RenderAllNodes();
	RenderAllEffects();

	//hack to see health and shields for player...
	if (GetNumPostRenderEffects() > 0) 
	{
		glViewport(0, 0, pGameWindowSize->x, pGameWindowSize->y);
	}
	else 
	{
		glViewport(pGameWindowStart->x, pGameWindowStart->y, pGameWindowSize->x, pGameWindowSize->y);	
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	DrawPlayerInfo();

	if (!bPostProcessing) {
		return;
	}

	gGame()->GetEffectsBuffer()->Bind();								//set effects buffer
	gShaderManager()->GetProgramNormals()->Use();						//user normals program

	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	//Set viewport for the game, which is the right 3/4 of the screen
	glViewport(pGameWindowSize->x / 4, 0, pGameWindowSize->x * 3 / 4, pGameWindowSize->y);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.0, 0.1, 50.0); //update this so we dont get crappy distortion
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	if (m_pCamera)
	{	
		//rotate to camera orientation
		glMultMatrixf(m_pCamera->GetPosQuat().quat.ToMatrix().ToRotationalInverse().m);
		//translate to the camera position
		glTranslatef(-(m_pCamera->GetPosQuat().pos.x), -(m_pCamera->GetPosQuat().pos.y), -(m_pCamera->GetPosQuat().pos.z));
	}
	else
	{ //no camera, so some default orientation
		gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	}

	RenderAllPostProcessEffects();	//draw effects!

	DrawBuffer::BindDefault();
	gShaderManager()->GetProgramWave()->Use();							//set wave program

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glViewport(pGameWindowStart->x, pGameWindowStart->y, pGameWindowSize->x, pGameWindowSize->y);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gGame()->GetSceneBuffer()->GetBufferTexture());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gGame()->GetEffectsBuffer()->GetBufferTexture());
 
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
	glEnd();

	gShaderManager()->UseDefaultProgram();
	glActiveTexture(GL_TEXTURE0);
}

void ActionScene::SceneEnd()
{
	Scene::SceneEnd();

	m_pPlayerObject->SetFiringPlugin(0);
	m_pPlayerObject->SetMovementPlugin(0);

	SetPlayerObject(0);
	SetSector(0);
}

void ActionScene::SceneStart()
{
	Scene::SceneStart();

	SetPlayerObject(gGame()->GetPlayer()->GetPlayerObject());
	SetSector(gGame()->GetPlayer()->GetTargetSector());

	GameCamera *pNewCamera = new GameCamera();
	SetCamera(pNewCamera);
	pNewCamera->Release();

	Background *pBackground = new Background();
	pBackground->SetCamera(GetCamera());
	AddObject(pBackground);
	pBackground->Release();

	if (!m_pSector)
	{	
		float	fTextScale				= 1.5f;
		size_t	BUFFER_SIZE				= 50;
		char	*pBuffer				= (char *) malloc(sizeof(char) * BUFFER_SIZE);
		sprintf_s(pBuffer, BUFFER_SIZE, "No Sector Selected!");

		Text	*pNoSectorText					= new Text(pBuffer, VECTOR3(0.35f, 0.04f, 0.04f));
		pNoSectorText->GetNode()->m_PosQuat.pos.x	-= pNoSectorText->GetTextNode()->GetTextWidth() / 2.0f * fTextScale;
		pNoSectorText->GetNode()->m_PosQuat.pos.y	+= 1.5f;
		pNoSectorText->GetNode()->m_vScale.x		= fTextScale;
		pNoSectorText->GetNode()->m_vScale.y		= fTextScale;
		AddObject(pNoSectorText);
		pNoSectorText->Release();

		return;
	}

	m_pPlayerObject->Regenerate(); //reset structure/shields to max
	m_pPlayerObject->Reset();
	if (m_pPlayerObject->GetPhysicalObjectType() == PhysicalObject::ePLAYER_SHIP)
	{
		((PlayerShip *)m_pPlayerObject)->InitPlayerPosition();
	}
	AddObject(m_pPlayerObject);
	AddObject(m_pPlayerObject->GetShield());

	HumanMovementPlugin *pMove = HumanMovementPlugin::GetPlugin();
	m_pPlayerObject->SetMovementPlugin(pMove);
	pMove->AddControlledObject(m_pPlayerObject);
	AddObject(pMove);

	HumanFiringPlugin *pFire = HumanFiringPlugin::GetPlugin();
	m_pPlayerObject->SetFiringPlugin(pFire);
	pFire->AddControlledObject(m_pPlayerObject);
	AddObject(pFire);

	AddObject(m_pSector);
	m_pSector->SetScene(this);
	m_pSector->Play();
}

void ActionScene::SetPlayerObject(PhysicalObject *pNewObject)
{
	if (m_pPlayerObject != pNewObject)
	{
		if (m_pPlayerObject)
		{
			m_pPlayerObject->Release();
			m_pPlayerObject = 0;
		}

		if (pNewObject)
		{
			m_pPlayerObject = pNewObject;
			m_pPlayerObject->AddRef();
		}
	}
}

void ActionScene::SetSector(Sector *pSector)
{
	if (pSector != m_pSector)
	{
		if (m_pSector)
		{
			m_pSector->Release();
			m_pSector = 0;
		}

		if (pSector)
		{
			m_pSector = pSector;
			m_pSector->AddRef();
		}
	}
}	