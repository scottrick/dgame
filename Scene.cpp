#include "Scene.h"

#include "Camera.h"
#include "DrawBuffer.h"
#include "Effect.h"
#include "EventManager.h"
#include "Game.h"
#include "Matrix44.h"
#include "Node.h"
#include "Object.h"
#include "ObjectManager.h"
#include "ParticleManager.h"
#include "PhysicalObject.h"
#include "PostProcessEffect.h"
#include "ShaderProgram.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "UIContainer.h"
#include "UnknownManager.h"
#include "VisualManager.h"

#include <GL/glut.h>

Scene::Scene()
{
	Init();
}

Scene::~Scene()
{
	SceneEnd();
}

bool Scene::AddNode(Node *pNode)
{
	if (pNode)
	{
		m_lNodes.push_back(pNode);
		pNode->AddRef();
		return true;
	}
	else 
	{
		return false;
	}
}

bool Scene::AddObject(Object *pObject)
{
	if (pObject)
	{
		switch (pObject->GetObjectType())
		{
		case Object::eTYPE_POST_EFFECT:
			m_lPostProcessEffects.push_back((PostProcessEffect *)pObject);
			break;
		case Object::eTYPE_EFFECT:
			m_lEffects.push_back((Effect *)pObject);
			break;
		default:
			AddNode(pObject->GetNode()); //add the object's node
			m_lObjects.push_back(pObject);
			break;
		}

		pObject->AddRef();
		pObject->SetScene(this);

		if (pObject->GetObjectType() == Object::eTYPE_PHYSICAL_OBJECT)
		{
			if (((PhysicalObject *)pObject)->GetPhysicalObjectType() == PhysicalObject::eCOMPUTER_SHIP)
			{
				++m_dwComputerShipCount;
			}
		}

		return true;
	}
	else
	{
		return false;
	}
}

bool Scene::AddUIObject(UIObject *pUIObject)
{
	if (pUIObject)
	{
		m_lUIObjects.push_back(pUIObject);
		pUIObject->AddRef();

		return true;
	}
	else
	{
		return false;
	}
}

void Scene::HandleInput(const unsigned char &key, const int &x, const int &y, const unsigned int &dwKeyboardFlags)
{ //default implementation;  override!
	if (!m_lUIObjects.empty())
	{ //for now, just send input to the first item in the list...
		(*(m_lUIObjects.begin()))->HandleInput(key, x, y, dwKeyboardFlags);
	}
}

void Scene::HandleMouseMove(const int &dwX, const int &dwY, const int &dwDeltaX, const int &dwDeltaY)
{
	//default implementation;  override in derived class if you wanna handle mouse!
}

void Scene::HandleMouseMovePassive(const int &dwX, const int &dwY, const int &dwDeltaX, const int &dwDeltaY)
{
	//default implementation;  override in derived class if you wanna handle mouse!
}

void Scene::Init()
{
	m_pCamera				= 0;
	m_dwComputerShipCount	= 0;
}

void Scene::Print(int indent) const
{
	list<Node *>::const_iterator iter;
	for (iter = m_lNodes.begin(); iter != m_lNodes.end(); iter++)
	{
		(*iter)->Print(indent + 2);
	}	
}

void Scene::RefreshAll(const float &fDeltaTime)
{
	list<Object *>::iterator iter;
	for (iter = m_lObjects.begin(); iter != m_lObjects.end(); iter++)
	{ //refresh everything!
		(*iter)->Refresh(fDeltaTime);
	}

	list<PostProcessEffect *>::iterator postIter;
	for (postIter = m_lPostProcessEffects.begin(); postIter != m_lPostProcessEffects.end(); postIter++)
	{
		(*postIter)->Refresh(fDeltaTime);
	}	

	list<Effect *>::iterator partIter;
	for (partIter = m_lEffects.begin(); partIter != m_lEffects.end(); partIter++)
	{
		(*partIter)->Refresh(fDeltaTime);
	}

	list<UIObject *>::iterator uiIter;
	for (uiIter = m_lUIObjects.begin(); uiIter != m_lUIObjects.end(); uiIter++)
	{
		(*uiIter)->Refresh(fDeltaTime);
	}
}

void Scene::RefreshManagers(const float &fDeltaTime)
{
	gVisualManager()->Refresh(fDeltaTime);
	gTextureManager()->Refresh(fDeltaTime);
	gUnknownManager()->Refresh(fDeltaTime);
	gEventManager()->Refresh(fDeltaTime);
	gObjectManager()->Refresh(fDeltaTime);
//	gEffectManager()->Refresh(fDeltaTime);
	gParticleManager()->Refresh(fDeltaTime);
}

void Scene::RemoveAllEffects()
{
	Effect *pEffect = 0;
	list<Effect *>::iterator iter;
	for (iter = m_lEffects.begin(); iter != m_lEffects.end(); iter++)
	{
		pEffect = *iter;
		if (pEffect)
		{
			pEffect->Release();
		}
	}
	m_lEffects.clear();
}

void Scene::RemoveAllNodes()
{
	Node *pNode = 0;
	list<Node *>::iterator iter;
	for (iter = m_lNodes.begin(); iter != m_lNodes.end(); iter++)
	{ //release all nodes
		pNode = *iter;
		
		if (pNode)
		{
			pNode->Release();
		}
	}
	m_lNodes.clear();
}

void Scene::RemoveAllObjects()
{
	Object *pObject = 0;
	list<Object *>::iterator iter;
	for (iter = m_lObjects.begin(); iter != m_lObjects.end(); iter++)
	{
		pObject = *iter;
		if (pObject)
		{
			pObject->Release();
		}
	}
	m_lObjects.clear();
}	

void Scene::RemoveAllPostProcessEffects()
{
	PostProcessEffect *pPost = 0;
	list<PostProcessEffect *>::iterator iter;
	for (iter = m_lPostProcessEffects.begin(); iter != m_lPostProcessEffects.end(); iter++)
	{
		pPost = *iter;
		if (pPost)
		{
			pPost->Release();
		}
	}
	m_lPostProcessEffects.clear();
}

void Scene::RemoveAllUIObjects()
{
	list<UIObject *>::iterator iter = m_lUIObjects.begin();
	while (iter != m_lUIObjects.end())
	{
		UIObject *pCurrent = *iter;
		if (pCurrent)
		{
			pCurrent->Release();
			pCurrent = 0;
		}

		iter++;
	}
	m_lUIObjects.clear();
}	

bool Scene::RemoveEffect(Effect *pEffect)
{
	bool bReturn = false;

	if (pEffect)
	{

		list<Effect *>::iterator iter = m_lEffects.begin();
		while (iter != m_lEffects.end())
		{
			if (pEffect == *iter)
			{
				(*iter)->Release();
				iter = m_lEffects.erase(iter);
				bReturn = true;
			}
			else
			{
				iter++;
			}
		}
	}

	return bReturn;
}

bool Scene::RemoveNode(Node *pNode)
{
	bool bReturn = false;  //returns false if nothing was removed

	if (pNode)
	{
		list<Node *>::iterator iter = m_lNodes.begin();
		while (iter != m_lNodes.end())
		{
			if (*iter == pNode)
			{
				(*iter)->Release();				//release the node in the list
				iter = m_lNodes.erase(iter);	//remove it from the list!
				bReturn = true;
			}
			else
			{
				iter++;
			}
		}
	}

	return bReturn;
}

bool Scene::RemoveObject(Object *pObject)
{
	bool bReturn = false;  //returns false if nothing was removed

	if (pObject)
	{
		if (pObject->GetObjectType() == Object::eTYPE_POST_EFFECT)
		{
			RemovePostProcessEffect((PostProcessEffect *) pObject);
		}
		else if (pObject->GetObjectType() == Object::eTYPE_EFFECT)
		{
			RemoveEffect((Effect *) pObject);
		}
		else
		{
			list<Object *>::iterator iter = m_lObjects.begin();

			while (iter != m_lObjects.end())
			{
				if (*iter == pObject)
				{
					(*iter)->Release();				//release the object in the list
					iter = m_lObjects.erase(iter);	//remove it from the list!
					bReturn = true;
				}
				else
				{
					iter++;
				}
			}

			//try to remove the objects node
			RemoveNode(pObject->GetNode());
		}

		if (pObject->GetObjectType() == Object::eTYPE_PHYSICAL_OBJECT)
		{
			if (((PhysicalObject *)pObject)->GetPhysicalObjectType() == PhysicalObject::eCOMPUTER_SHIP)
			{
				--m_dwComputerShipCount;
			}
		}
	}

	return bReturn;
}

bool Scene::RemovePostProcessEffect(PostProcessEffect *pPost)
{
	bool bReturn = false;  //returns false if nothing was removed

	if (pPost)
	{
		list<PostProcessEffect *>::iterator iter = m_lPostProcessEffects.begin();
		while (iter != m_lPostProcessEffects.end())
		{
			if (*iter == pPost)
			{
				(*iter)->Release();							//release the node in the list
				iter = m_lPostProcessEffects.erase(iter);	//remove it from the list!
				bReturn = true;
			}
			else
			{
				iter++;
			}
		}
	}

	return bReturn;	
}

void Scene::Render(Manager *pDebugManager)
{
	const VECTOR2_INT *pGameWindowSize		= gGame()->GetGameWindowSize();
	const VECTOR2_INT *pGameWindowStart		= gGame()->GetGameWindowStart();
	bool bPostProcessing					= (GetNumPostRenderEffects() > 0) && (gShaderManager()->IsShadersSupported());

	gShaderManager()->UseDefaultProgram(); //render scene with fixed pipeline
	
	if (bPostProcessing) {
		gGame()->GetSceneBuffer()->Bind();	//need post processing, so render to scene buffer
	}
	else {
		DrawBuffer::BindDefault(); //no post processing, so just render normally
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	if (pDebugManager)
	{
		RenderDebug(pDebugManager);
	}

	if (bPostProcessing) {
		glViewport(0, 0, pGameWindowSize->x, pGameWindowSize->y);
	}
	else {
		glViewport(pGameWindowStart->x, pGameWindowStart->y, pGameWindowSize->x, pGameWindowSize->y);
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)pGameWindowSize->x / (double)pGameWindowSize->y, 0.1, 50.0); //update this so we dont get crappy distortion
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	if (m_pCamera)
	{	
		GLfloat fLightPos[] = { m_pCamera->GetPosQuat().pos.x, m_pCamera->GetPosQuat().pos.y, m_pCamera->GetPosQuat().pos.z, 1.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, fLightPos);

		//use camera orientation and position
		glMultMatrixf(m_pCamera->GetPosQuat().quat.ToMatrix().ToRotationalInverse().m);
		glTranslatef(-(m_pCamera->GetPosQuat().pos.x), -(m_pCamera->GetPosQuat().pos.y), -(m_pCamera->GetPosQuat().pos.z));
	}
	else
	{ //no camera, so some default orientation
		gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	}

	RenderAllNodes();
	RenderAllEffects();

	RenderUI();

	if (!bPostProcessing) {
		return;
	}

	gGame()->GetEffectsBuffer()->Bind();								//set effects buffer
	gShaderManager()->GetProgramNormals()->Use();						//use normals program

	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

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

void Scene::RenderAllEffects()
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//glBlendFunc(GL_ONE, GL_ONE);
	glNormal3f(0.0f, 0.0f, 1.0f); //normal always towards the camera

	Effect						*pCurrent	= 0;
	list<Effect *>::iterator	iter;
	for (iter = m_lEffects.begin(); iter != m_lEffects.end(); iter++)
	{
		pCurrent = *iter;
		if (pCurrent)
		{
			pCurrent->Render();
		}
	}

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void Scene::RenderAllNodes()
{
	Node					*pCurrent = 0;
	list<Node *>::iterator	iter;
	for (iter = m_lNodes.begin(); iter != m_lNodes.end(); iter++)
	{
		pCurrent = *iter;
		if (pCurrent)
		{
			pCurrent->Render();
		}		
	}
}	

void Scene::RenderAllPostProcessEffects()
{
	GLboolean bTextureWasOn = glIsEnabled(GL_TEXTURE_2D);
	GLboolean bLightingWasOn = glIsEnabled(GL_LIGHTING);
	
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D); //disable textures when rendering the text

	PostProcessEffect						*pCurrent = 0;
	list<PostProcessEffect *>::iterator		iter;
	for (iter = m_lPostProcessEffects.begin(); iter != m_lPostProcessEffects.end(); iter++)
	{
		pCurrent = *iter;

		if (pCurrent)
		{
			pCurrent->PostRender();
		}
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

void Scene::RenderDebug(Manager *pManager)
{
	const VECTOR2_INT *pGameWindowSize		= gGame()->GetGameWindowSize();
	const VECTOR2_INT *pGameWindowStart		= gGame()->GetGameWindowStart();
	bool bPostProcessing					= (GetNumPostRenderEffects() > 0) && (gShaderManager()->IsShadersSupported());

	//setup viewport for debug rendering, which is the left 1/4 of the screen
	if (bPostProcessing) {
		glViewport(0, 0, pGameWindowSize->x / 4, pGameWindowSize->y);
	}
	else {
		glViewport(pGameWindowStart->x, pGameWindowStart->y, pGameWindowSize->x / 4, pGameWindowSize->y);
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 0.33333333, 0.1, 50.0); //update this so we dont get crappy distortion
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glPushMatrix();
	gluLookAt(0.0, 0.0, 1.75, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	pManager->DebugInfoRender(0.33333333f, m_PosQuat);
	glPopMatrix();
}

void Scene::RenderUI()
{
	//RENDER UI stuff
	list<UIObject *>::iterator	iter			= m_lUIObjects.begin();

	while (iter != m_lUIObjects.end())
	{		
		(*iter)->Render();
		iter++;
	}
}

void Scene::SceneEnd()
{
	SetCamera(0);

	RemoveAllEffects();
	RemoveAllNodes();
	RemoveAllObjects();
	RemoveAllPostProcessEffects();
	RemoveAllUIObjects();

	m_dwComputerShipCount = 0;
}

void Scene::SceneStart()
{

}

void Scene::SetCamera(Camera *pNewCamera)
{
	if (pNewCamera != m_pCamera)
	{
		if (m_pCamera)
		{
			m_pCamera->Release();
			m_pCamera = 0;
		}

		if (pNewCamera)
		{
			m_pCamera = pNewCamera;
			m_pCamera->AddRef();
		}	
	}
}

void Scene::ToggleBoundingBoxes()
{	
	list<Node *>::iterator iter;
	Node *pNode = 0;
	for (iter = m_lNodes.begin(); iter != m_lNodes.end(); iter++)
	{
		pNode = *iter;
		
		if (pNode)
		{
			if (pNode->GetNodeFlags() & NODE_RENDER_COLLISION_DEBUG)
			{
				pNode->RemoveNodeFlags(NODE_RENDER_COLLISION_DEBUG);
			}
			else
			{
				pNode->RaiseNodeFlags(NODE_RENDER_COLLISION_DEBUG);
			}
		}
	}
}