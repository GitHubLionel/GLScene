#include "GLCallback.h"
#ifdef USE_GLEW
#include <GL/glew.h> // IMPORTANT, must be declared before freeglut !
#endif
#include <GL/freeglut.h>
#include <stdio.h>

using namespace GLScene;

// The GLScene entry point for the GLUT callback events
GLCallBack* _pGLCallBack = NULL;

// The unique GLContext
bool GLCallBack::GLContext_Exist = false;

// Error
static void OnError(const char *fmt, va_list ap)
{
  _pGLCallBack->OnError(fmt, ap);
}

/// Display events
static void OnDisplay(void)
{
  _pGLCallBack->OnDisplay();
}

static void OnReshape(int width, int height)
{
  _pGLCallBack->OnReshape(width, height);
}

static void OnClose(void)
{
  _pGLCallBack->OnClose();
}

static void OnIdle(void)
{
  _pGLCallBack->OnIdle();
}

static void OnTimer(int id)
{
  _pGLCallBack->OnTimer(id);
  glutTimerFunc(_pGLCallBack->TimerStepms, OnTimer, id);
}

/// IHM events
static void OnkeyEnter(unsigned char key, int x, int y)
{
  _pGLCallBack->OnkeyEnter(key, x, y);
}

static void OnkeyEnterUp(unsigned char key, int x, int y)
{
  _pGLCallBack->OnkeyEnterUp(key, x, y);
}

static void OnSpecialKey(int key, int x, int y)
{
  _pGLCallBack->OnSpecialKey(key, x, y);
}

static void OnSpecialKeyUp(int key, int x, int y)
{
  _pGLCallBack->OnSpecialKeyUp(key, x, y);
}

static void OnMouseEnter(int state)
{
  _pGLCallBack->OnMouseEnter(state);
}

static void OnMouseClick(int button, int updown, int x, int y)
{
  _pGLCallBack->OnMouseClick(button, updown, x, y);
}

static void OnMouseMove(int x, int y)
{
  _pGLCallBack->OnMouseMove(x, y);
}

static void OnMousePassiveMove(int x, int y)
{
  _pGLCallBack->OnMousePassiveMove(x, y);
}

static void OnMouseWheel(int wheel, int direction, int x, int y)
{
  _pGLCallBack->OnMouseWheel(wheel, direction, x, y);
}

static void OnMenuEntry(int value)
{
  _pGLCallBack->OnMenuEntry(value);
}

// Callback for the sub window
static void OnSubDisplay(void)
{
  _pGLCallBack->OnSubDisplay();
}

/**
 * Forwarding the GLUT callback
 */
void GLScene::InitCallbacks(GLCallBack* _pGLScene)
{
  _pGLCallBack = _pGLScene;

  // Error
  glutInitErrorFunc(OnError);

  // Display events
  glutDisplayFunc(OnDisplay);
  glutReshapeFunc(OnReshape);
  glutCloseFunc(OnClose);
  glutIdleFunc(OnIdle);

  // Keyboard events
  glutKeyboardFunc(OnkeyEnter);
  glutKeyboardUpFunc(OnkeyEnterUp);
  glutSpecialFunc(OnSpecialKey);
  glutSpecialUpFunc(OnSpecialKeyUp);

  // Mouse events
  glutEntryFunc(OnMouseEnter);
  glutMouseFunc(OnMouseClick);
  glutMotionFunc(OnMouseMove);
  glutPassiveMotionFunc(OnMousePassiveMove);
  glutMouseWheelFunc(OnMouseWheel);

  glutTimerFunc(_pGLScene->TimerStepms, OnTimer, _pGLScene->Get_Window_ID());
}

void GLScene::InitSubCallbacks(GLCallBack* _pGLScene)
{
  _pGLCallBack = _pGLScene;

  // Error
  glutInitErrorFunc(OnError);

  // Display events
  glutDisplayFunc(OnSubDisplay);
//  glutReshapeFunc(OnSubReshape);

  // Keyboard events
//  glutKeyboardFunc(OnSubkeyEnter);
//  glutKeyboardUpFunc(OnSubkeyEnter);
//  glutSpecialFunc(OnSubSpecialKey);
//  glutSpecialUpFunc(OnSubSpecialKey);

  // Mouse events
//  glutEntryFunc(OnSubMouseEnter);
//  glutMouseFunc(OnSubMouseClick);
//  glutMotionFunc(OnSubMouseMove);
//  glutPassiveMotionFunc(OnSubMousePassiveMove);
//  glutMouseWheelFunc(OnSubMouseWheel);
}

int GLScene::InitCallbackMenu(GLCallBack* _pGLScene)
{
  _pGLCallBack = _pGLScene;

  return glutCreateMenu(OnMenuEntry);
}

 /**
  * GLEW Initialization
  */
#ifdef USE_GLEW
 void GLScene::InitGlew()
 {
   static bool glewInitialized = false;
   if (!glewInitialized)
   {
     glewInitialized = true;

     glewExperimental = true;
     GLenum err = glewInit();
     if (GLEW_OK != err) {
       // Problem: glewInit failed, something is seriously wrong !
       fprintf(stderr, "GLEW: ERROR: GLEW initialization failed! We're going to keep going anyways, but we will most likely crash.\n");
       fprintf(stderr, "GLEW: ERROR: %s\n", glewGetErrorString(err));
     }
     fprintf(stdout, "GLEW: Using GLEW %s\n", glewGetString(GLEW_VERSION));
   }
 }

 const char *GLScene::GetGlewVersion()
 {
   return (const char *)glewGetString(GLEW_VERSION);
 }
#endif

// ********************************************************************************
// End of file
// ********************************************************************************
