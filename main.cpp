//  dGame
//  Written by Scott Atkins
//  1-17-2008
#ifdef NDEBUG //no console on release builds!
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

#include <GL/glew.h>

#include "Block.h"
#include "def.h"
#include "Camera.h"
#include "File.h"
#include "Game.h"
#include "Input.h"
#include "Matrix44.h"
#include "Node.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "ShaderManager.h"
#include "TestNode.h"
#include "TestObject.h"
#include "TestTextureNode.h"
#include "Text.h"
#include "Texture.h"
#include "TextureManager.h"
#include "UnknownManager.h"
#include "utility.h"
#include "Visual.h"
#include "VisualManager.h"

#include <iostream>
#include "Profile.h"	//any files that include windows.h need to be included after the regular includes
#include <windows.h>
#include <GL/glut.h>
using namespace std;

#define		DEBUG_CHAR_LENGTH		50

/*
	----------------------------------------------------------------------------------------------------------
	FUNCTION DECLARATIONS
	----------------------------------------------------------------------------------------------------------
*/
void		DrawFPS();
void		Initialize(int argc, char **argv);
void		InitializeGlut(int argc, char** argv);
void		InitializeDebug();
void		KeyboardSpecialDown(int key, int x, int y);
void		KeyboardSpecialUp(int key, int x, int y);
void		KeyboardStandardDown(unsigned char key, int x, int y);
void		KeyboardStandardUp(unsigned char key, int x, int y);
void		LoopFunction();
int			main(int argc, char **argv);
void		MouseClick(int button, int state, int x, int y);
void		MouseMove(int x, int y);
void		MousePassiveMove(int x, int y);
void		RefreshEverything(int dwDeltaTime);
void		Render();
void		ReshapeDisplay(GLsizei W, GLsizei H);

/*
	----------------------------------------------------------------------------------------------------------
	GLOBAL VARIABLES
	----------------------------------------------------------------------------------------------------------
*/
long			FPSAccumulator						= 0;
long			GameTime							= 0;
int				CurrentFPSCount						= 0;
long			CurrentTime							= 0;
long			DeltaTime							= 0;
int				PreviousFPSCount					= 0;
long			PreviousTime						= 0;
int				LongestRefresh						= 0;

int				STARTING_WINDOW_X					= 800;
int				STARTING_WINDOW_Y					= 600;
int				STARTING_WINDOW_DISPLACEMENT_X		= 64;
int				STARTING_WINDOW_DISPLACEMENT_Y		= 64;
int				MOUSE_X								= 0;
int				MOUSE_Y								= 0;
int				WINDOW_X							= 0;
int				WINDOW_Y							= 0;
bool			DEBUG_FPS_DISPLAY					= false;
bool			PAUSED								= false;
bool			SLOW_MOTION							= false;
bool			FULL_SCREEN							= false;

char			*WINDOW_NAME						= "dGame";
TestNode		*g_pOrigin							= 0;
char			*pDebugChars						= 0;

/*
	----------------------------------------------------------------------------------------------------------
	FUNCTION DEFINITIONS
	----------------------------------------------------------------------------------------------------------
*/
void DrawFPS()
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	GLboolean bTextureWasOn = glIsEnabled(GL_TEXTURE_2D);
	GLboolean bLightingWasOn = glIsEnabled(GL_LIGHTING);
	
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D); //disable textures when rendering the text

	//draw FPS
	memset(pDebugChars, 0, DEBUG_CHAR_LENGTH);
	sprintf_s(pDebugChars, DEBUG_CHAR_LENGTH, "%d fps", PreviousFPSCount);
	glRasterPos2f(-1.0f, -0.99f);
	for (const char *c = pDebugChars; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
	}

	//draw Game Time
	float fGameTime = ((float)(GameTime)) / 1000.0f;
	memset(pDebugChars, 0, DEBUG_CHAR_LENGTH);
	sprintf_s(pDebugChars, DEBUG_CHAR_LENGTH, "%.*f seconds", 3 ,fGameTime);
	glRasterPos2f(-1.0f, -0.94f);
	for (const char *c = pDebugChars; *c != '\0'; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
	}

	//draw Longest Refre`sh
	memset(pDebugChars, 0, DEBUG_CHAR_LENGTH);
	sprintf_s(pDebugChars, DEBUG_CHAR_LENGTH, "%d ms", LongestRefresh);
	glRasterPos2f(-1.0f, -0.89f);
	for (const char *c = pDebugChars; *c != '\0'; c++)
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

void Initialize(int argc, char **argv)
{
	if (argc > 2) //more than two arguments means we were passed two values, which will be used for the X and Y window size
	{
		STARTING_WINDOW_X = (unsigned int)atoi(argv[1]);
		STARTING_WINDOW_Y = (unsigned int)atoi(argv[2]);
	}

	InitializeRandomGenerator(GetTickCount());

#ifdef _PROFILING_ENABLED_
	Profile::ProfileInit();
#endif

	GLfloat fWhiteLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//GLfloat fModelAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };

	glLightfv(GL_LIGHT0, GL_DIFFUSE, fWhiteLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, fWhiteLight);
	glLightModelf(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE); //so lighting works!
	glEnable(GL_TEXTURE_2D); //textures on by default

	gShaderManager()->LoadShaders();	//load all shaders in "\data\shaders"
	gTextureManager()->LoadTextures();	//load up all textures in "\data\textures"
	gVisualManager()->LoadVisuals();	//loads all visuals in "\data\visuals"

	InitializeDebug();

 	CurrentTime = GetTickCount();
}	

void InitializeGlut(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(STARTING_WINDOW_DISPLACEMENT_X, STARTING_WINDOW_DISPLACEMENT_Y);
	glutInitWindowSize(STARTING_WINDOW_X, STARTING_WINDOW_Y);
	glutCreateWindow(WINDOW_NAME);
#ifdef NDEBUG
	glutSetCursor(GLUT_CURSOR_NONE);
#endif
	glewInit();

	printf("OpenGL Version %s\n%s\n%s\nShader Version %s\n\n", glGetString(GL_VERSION), glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_SHADING_LANGUAGE_VERSION));

	//register the callback functions
	glutIdleFunc(LoopFunction);
	glutDisplayFunc(Render);
	glutReshapeFunc(ReshapeDisplay);
	glutKeyboardFunc(KeyboardStandardDown);
	glutKeyboardUpFunc(KeyboardStandardUp);
	glutSpecialFunc(KeyboardSpecialDown);
	glutSpecialUpFunc(KeyboardSpecialUp);
	glutPassiveMotionFunc(MousePassiveMove);
	glutMouseFunc(MouseClick);
	glutMotionFunc(MouseMove);
}

void InitializeDebug()
{
	pDebugChars		= (char *)malloc(sizeof(char) * DEBUG_CHAR_LENGTH);

	/*
	g_pOrigin					= new TestNode();
	g_pOrigin->RaiseNodeFlags(NODE_DO_NOT_RENDER);
	gGame()->GetCurrentScene()->AddNode(g_pOrigin); //add the debug origin to the scene, but invisible initially
	*/
}

void KeyboardStandardDown(unsigned char key, int x, int y)
{
	//printf("%d\n", key);
	gInput()->InputDown(key);

	unsigned int	dwKeyboardFlags		= 0;
	int				dwGlutModifiers		= glutGetModifiers();

	if (dwGlutModifiers & GLUT_ACTIVE_CTRL) {
		dwKeyboardFlags |= KEYBOARD_CONTROL_ACTIVE;
	}

	if (dwGlutModifiers & GLUT_ACTIVE_ALT) {
		dwKeyboardFlags |= KEYBOARD_ALT_ACTIVE;
	}

	if (dwGlutModifiers & GLUT_ACTIVE_SHIFT) {
		dwKeyboardFlags |= KEYBOARD_SHIFT_ACTIVE;
	}

	if (key == 27) //listen for esc
	{
		exit(1);
	}
	else if ((key == 13) && (dwKeyboardFlags & KEYBOARD_ALT_ACTIVE)) //ENTER key + ALT
	{
		if (FULL_SCREEN)
		{
			 glutReshapeWindow(STARTING_WINDOW_X, STARTING_WINDOW_Y);
			 glutPositionWindow(STARTING_WINDOW_DISPLACEMENT_X, STARTING_WINDOW_DISPLACEMENT_Y);
		}
		else
		{
			glutFullScreen();
		}

		FULL_SCREEN = !FULL_SCREEN;
		return;
	}
	else if ((key == 'p') || (key == 'P'))
	{
		PAUSED = !PAUSED;
	}
	else if ((key == 'o') || (key == 'O'))
	{
		SLOW_MOTION = !SLOW_MOTION;
	}
	else if ((key == 'u') || (key == 'U'))
	{
		gUnknownManager()->Print(); //unknown manager debug info
	}
	else if ((key == '8') || (key == 'w') || (key == 'W'))
	{
		gGame()->RaiseInputFlags(INPUT_MOVE_FORWARDS);
	}
	else if ((key == '5') || (key == 's') || (key == 'S'))
	{
		gGame()->RaiseInputFlags(INPUT_MOVE_BACKWARDS);
	}
	else if ((key == '4') || (key == 'a') || (key == 'A'))
	{	
		gGame()->RaiseInputFlags(INPUT_STRAFE_LEFT);
	}
	else if ((key == '6') || (key == 'd') || (key == 'D'))
	{
		gGame()->RaiseInputFlags(INPUT_STRAFE_RIGHT);
	}
	else if ((key == '[') || (key == '{'))
	{
		gGame()->RaiseInputFlags(INPUT_TILT_LEFT);
	}
	else if ((key == ']') || (key == '}'))
	{
		gGame()->RaiseInputFlags(INPUT_TILT_RIGHT);
	}
	else if (key == 14) //CTRL + N
	{
		if (g_pOrigin)
		{
			if (g_pOrigin->GetNodeFlags() & NODE_DO_NOT_RENDER)
			{	
				g_pOrigin->RemoveNodeFlags(NODE_DO_NOT_RENDER);
			}
			else
			{
				g_pOrigin->RaiseNodeFlags(NODE_DO_NOT_RENDER);
			}
		}
	}
	else if (key == 6) //CTRL + F
	{
		DEBUG_FPS_DISPLAY = !DEBUG_FPS_DISPLAY;	
	}
	
	gGame()->HandleInput(key, x, y, dwKeyboardFlags); //send the input off to the game
}

void KeyboardStandardUp(unsigned char key, int x, int y)
{
	gInput()->InputUp(key);
		
	if ((key == '8') || (key == 'w') || (key == 'W'))
	{
		gGame()->RemoveInputFlags(INPUT_MOVE_FORWARDS);
	}
	else if ((key == '5') || (key == 's') || (key == 'S'))
	{
		gGame()->RemoveInputFlags(INPUT_MOVE_BACKWARDS);
	}
	else if ((key == '4') || (key == 'a') || (key == 'A'))
	{	
		gGame()->RemoveInputFlags(INPUT_STRAFE_LEFT);
	}
	else if ((key == '6') || (key == 'd') || (key == 'D'))
	{
		gGame()->RemoveInputFlags(INPUT_STRAFE_RIGHT);
	}
	else if ((key == '[') || (key == '{'))
	{
		gGame()->RemoveInputFlags(INPUT_TILT_LEFT);
	}
	else if ((key == ']') || (key == '}'))
	{
		gGame()->RemoveInputFlags(INPUT_TILT_RIGHT);
	}
}

void KeyboardSpecialDown(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
	{
		gGame()->RaiseInputFlags(INPUT_LOOK_UP);
	}
	else if (key == GLUT_KEY_DOWN)
	{	
		gGame()->RaiseInputFlags(INPUT_LOOK_DOWN);
	}
	else if (key == GLUT_KEY_RIGHT)
	{
		gGame()->RaiseInputFlags(INPUT_TURN_RIGHT);
	}
	else if (key == GLUT_KEY_LEFT)
	{
		gGame()->RaiseInputFlags(INPUT_TURN_LEFT);
	}
	else if ((key == GLUT_KEY_F5) || (key == GLUT_KEY_F6) || (key == GLUT_KEY_F7) || (key == GLUT_KEY_F8) || (key == GLUT_KEY_F10))
	{
		gGame()->DebugSetScene(key);
	}
}	

void KeyboardSpecialUp(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
	{
		gGame()->RemoveInputFlags(INPUT_LOOK_UP);
	}
	else if (key == GLUT_KEY_DOWN)
	{	
		gGame()->RemoveInputFlags(INPUT_LOOK_DOWN);
	}
	else if (key == GLUT_KEY_RIGHT)
	{
		gGame()->RemoveInputFlags(INPUT_TURN_RIGHT);
	}
	else if (key == GLUT_KEY_LEFT)
	{
		gGame()->RemoveInputFlags(INPUT_TURN_LEFT);
	}	
}	

void LoopFunction()
{
	/*
		First refresh, and then render
	*/

#ifdef _PROFILING_ENABLED_
	Profile::g_fStartProfile = Profile::GetTime();
	Profile::ProfileBegin("MAIN LOOP");
#endif

	PreviousTime			= CurrentTime;
	CurrentTime				= GetTickCount();
	DeltaTime				= CurrentTime - PreviousTime;
	FPSAccumulator			+= DeltaTime;

	if (LongestRefresh < DeltaTime)
	{
		LongestRefresh = DeltaTime;
	}
	
	if (FPSAccumulator >= 1000) {
		PreviousFPSCount	= CurrentFPSCount;
		CurrentFPSCount		= 0;

		FPSAccumulator		-= 1000;
	}

	if (!PAUSED)
	{ 
		if (SLOW_MOTION)
		{
			DeltaTime = DeltaTime / 10;
		}

		GameTime			+= DeltaTime;
		RefreshEverything(DeltaTime);
	}

	//finally, draw the screen
	glutPostRedisplay();

	CurrentFPSCount++;

#ifdef _PROFILING_ENABLED_
	Profile::ProfileEnd("MAIN LOOP");
	Profile::ProfileDumpOutputToBuffer();
#endif
}

int main(int argc, char **argv)
{
	printf("dGame\nWritten by Scott Atkins\n1-17-2008\n\n");	

	InitializeGlut(argc, argv);
	Initialize(argc, argv);
	
	glutMainLoop();

	return 0;
}	

void MouseClick(int button, int state, int x, int y)
{

}

void MouseMove(int x, int y)
{
	gGame()->HandleMouseMove(x, y, x - MOUSE_X, y - MOUSE_Y);

	MOUSE_X = x;
	MOUSE_Y = y;
}

void MousePassiveMove(int x, int y)
{
	gGame()->HandleMouseMovePassive(x, y, x - MOUSE_X, y - MOUSE_Y);

	MOUSE_X = x;
	MOUSE_Y = y;	
}

void RefreshEverything(int dwDeltaTime)
{
	if (dwDeltaTime)
	{
		gGame()->Refresh(dwDeltaTime); //refresh everything in the game
	}
}

/*
	Where the actual drawing happens!
*/
void Render()
{
	//should already be in Modelview mode
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	
	int *pgarbage = 0;

	gGame()->Render();  //RENDER the game!

	if (DEBUG_FPS_DISPLAY)
	{ //set viewport to be the whole screen, and then draw FPS in the bottom left corner
		glViewport(0, 0, WINDOW_X, WINDOW_Y);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(GAME_FOV, 1.0, 0.1, 50.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glPushMatrix();
		glLoadIdentity();
		gluLookAt(0.0, 0.0, 1.75, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		glDisable(GL_DEPTH_TEST);
		DrawFPS();
		glEnable(GL_DEPTH_TEST);
		glPopMatrix();
	}

	glutSwapBuffers();

#ifdef _DEBUG
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		printf("OpenGL Error!\n");

		const GLubyte *pErrorStr = gluErrorString(err); 
		if (pErrorStr)
		{
			printf("  %s\n", pErrorStr);
		}
		else
		{
			printf("  Unknown error (%d)\n", err);
		}
	}
#endif
}

void ReshapeDisplay(GLsizei W, GLsizei H)
{
	WINDOW_X = W;
	WINDOW_Y = H;
	gGame()->SetNewScreenDimensions(W, H);
}
