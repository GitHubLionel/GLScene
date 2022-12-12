#include "GLScene.h"
#include <string.h>
#include "Axis.h"
#include "Sphere.h"
#include "Arrow.h"
#include "Cube.h"
#include "Cylinder.h"

#ifdef __WXMSW__
#undef _UNICODE
#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include <wx/clipbrd.h>
#endif

// If defined, minimal sample is running with only TGLScene created
//#define RUN_SAMPLE

// Angle rotation for the arrow keyboard
#define ROTATION_ANGLE	5.0
#define ZOOM_FACTOR_PLUS 1.1
#define ZOOM_FACTOR_MINUS 0.9

/* ================================================================== */
/*                          TGLScene class                            */
/* ================================================================== */

TGLScene::TGLScene()
{
	GLWindow_Exist = false;
	TerminateOnError = false;
	IsRunning = false;
	CanResize = true;
	ObjectCount = 0;
	PickedObject = -1;
	OldPickedObject = -1;
	display_mode = dmRender;
	Center_Translation = TVector3D();
	Projection = glpFrustum;

	SubWinColor.backgroundColor = GLColor_lavender;
	SubWinColor.borderColor = GLColor_blue;
	SubWinColor.borderWidth = 3;
	SubWinColor.foregroundColor = GLColor_black;

	InitialView();

	// The default light
	light = new TLight();
//  camera = new TCamera();
}

//---------------------------------------------------------------------------
TGLScene::~TGLScene()
{
	ClearObject3D(false);
	delete light;
//  delete camera;
	Terminate();
}

//---------------------------------------------------------------------------
void TGLScene::Run()
{
	if (glutGetWindow() > 0)
	{
		IsRunning = true;
		glutMainLoop();
	}
}

//---------------------------------------------------------------------------
void TGLScene::Stop()
{
	if (glutGetWindow() > 0)
	{
		glutLeaveMainLoop();
		IsRunning = false;
	}
}

//---------------------------------------------------------------------------
void TGLScene::Show()
{
	if (glutGetWindow() > 0)
	{
		glutShowWindow();
		glutPostRedisplay();
		glutMainLoopEvent();
	}
}

//---------------------------------------------------------------------------
void TGLScene::Hide()
{
	if (glutGetWindow() > 0) glutHideWindow();
}

//---------------------------------------------------------------------------
void TGLScene::Refresh()
{
	if (GLWindow_Exist && (!IsRunning)) ReDisplay();
}

/**
 * Call glutPostRedisplay for the current window
 * If _redraw_view = true (default), then restaure the initial point of view :
 * Translation, Rotation and Scale
 */
void TGLScene::ReDisplay(bool _redraw_view)
{
	if (glutGetWindow() > 0)
	{
		redraw_view = _redraw_view;
		glutPostRedisplay();
		if (!IsRunning) glutMainLoopEvent();
	}
}

/**
 * Zoom the scene plus if inc > 0
 */
void TGLScene::Zoom(const int inc, bool redisplay)
{
	int i;

	if (inc >= 1)
	{
		for (i = 1; i <= inc; i++)
			mouse_scale *= ZOOM_FACTOR_PLUS;
	}
	else
		if (inc <= 1)
		{
			for (i = 1; i <= -inc; i++)
				mouse_scale *= ZOOM_FACTOR_MINUS;
		}
	if (redisplay)
	  ReDisplay();
}

/**
 * Move the scene up if incZ > 0; down if incZ < 0
 */
void TGLScene::CenterUpDown(const GLfloat incZ, bool redisplay)
{
	Center_Translation.Z += incZ;
	if (redisplay)
	  ReDisplay();
}

/**
 * Rotate the scene up if incY > 0; down if incY < 0
 */
void TGLScene::UpDown(const GLfloat incY, bool redisplay)
{
	mouse_Yrot -= incY;
	if (redisplay)
	  ReDisplay();
}

/**
 * Rotate the scene right if incX > 0; left if incX < 0
 */
void TGLScene::LeftRight(const GLfloat incX, bool redisplay)
{
	mouse_Xrot += incX;
	if (redisplay)
	  ReDisplay();
}

/**
 * Add object to the scene and return his index
 * Return -1 if we over MAX_OBJECT
 * If autofree = true (default), the object delete itself when object is suppressed from the scene
 */
int TGLScene::AddObject3D(TObject3D *obj, bool autoFree)
{
	if (ObjectCount == MAX_OBJECT)
	{
		fprintf(stderr, "Error: Too many object\n");
		fflush(stderr);
		return -1;
	}
	obj->AutoFree = autoFree;
	Object3DList.push_back(obj);
	ObjectCount++;
	return ObjectCount - 1;
}

//---------------------------------------------------------------------------
void TGLScene::DeleteObject3D(const int index)
{
	TObject3D *obj = GetObject3D(index);
	if (obj != NULL)
	{
		// Free object itself if autofree
		if (obj->AutoFree) delete obj;
		// Remove object from list
		Object3DList.erase(Object3DList.begin() + index);
		if (PickedObject == index)
			PickedObject = -1;
		if (OldPickedObject == index)
			OldPickedObject = -1;
		ObjectCount--;
	}
}

//---------------------------------------------------------------------------
TObject3D* TGLScene::GetObject3D(const int index)
{
	if ((index < 0) || (index >= ObjectCount)) return NULL;
	return Object3DList[index];
}

//---------------------------------------------------------------------------
TObject3D* TGLScene::GetObject3D(TVector3D position, TObjectType type)
{
	for (int i = 0; i < ObjectCount; i++)
	{
		if ((Object3DList[i]->GetObjectType() == type) && (Object3DList[i]->GetPosition() == position))
			return Object3DList[i];
	}
	return NULL;
}

//---------------------------------------------------------------------------
void TGLScene::ClearObject3D(bool ForceFree)
{
	for (int i = 0; i < ObjectCount; i++)
	{
		if (ForceFree || (Object3DList[i]->AutoFree)) delete Object3DList[i];
	}
	Object3DList.clear();
	ObjectCount = 0;
	PickedObject = -1;
	OldPickedObject = -1;
}

//---------------------------------------------------------------------------
void TGLScene::OnClose(void)
{
	std::cout << "Window closed." << std::endl;

	if (IsRunning)
	{
		glutLeaveMainLoop();
		IsRunning = false;
		// Do latest event
		glutMainLoopEvent();
	}

//  if (glutGetWindow() > 0)
//    glutDestroyWindow(glutGetWindow());

	GLWindow_Exist = false;
	Window_ID = -1;
}

void TGLScene::Get_GLInfo()
{
	std::cout << "OpenGL: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;
#ifdef USE_GLEW
  if (!GLWindow_Exist)
  {
    std::cerr << "GLEW: ERROR: a window is needed." << std::endl;
    return ;
  }
  else
    std::cout << "GLEW: Using GLEW " << GetGlewVersion() << std::endl;
#endif
}

/**
 * Main function that create the scene. Must be overloaded in new TGLScene class.
 * The GLContextWindow must be created before because we use some glut function
 */
void TGLScene::Create_GLScene(void)
{
#ifdef RUN_SAMPLE
	TVector3D p(0.0, 0.0, 0.0);

	// Clear existing object
	ClearObject3D();

  // Restaure initial position
  InitialView();

  // Clean sub windows
  CleanSubDisplay();

	// For example, add an axis and a sphere
	TAxis *axis = new TAxis(5);
	AddObject3D(axis);

	TSphere *sphere = new TSphere(0.5, &p);
	AddObject3D(sphere);

	// Add cylinder and cube
	TCylinder *cylinder = new TCylinder(1, 0.3, { 0.0, 0.0, 1.0 });
	cylinder->SetDirection({ 1.0, 0.0, 0.0 });
	AddObject3D(cylinder);

	TCube *cube = new TCube(1.0, { 0.0, 2.0, 0.0 });
	cube->SetDirection({ 0.0, 1.0, 1.0 });
	AddObject3D(cube);
#endif
}

/**
 * Function that update an existing scene. Must be overloaded in new TGLScene class.
 */
void TGLScene::Update_GLScene(bool Repaint __attribute__((unused)))
{
#ifdef RUN_SAMPLE
	// For example, just move the sphere (index = 1)
	TObject3D *obj = GetObject3D(1);
	if ((obj != NULL) && (obj->GetObjectType() == otSphere))
		obj->SetPosition( { GLRand() * 5, GLRand() * 3, GLRand() * 3 });

	// false because we don't need to rescale the scene (we just move a sphere)
	if (Repaint) ReDisplay(false);
#endif
}

//---------------------------------------------------------------------------
void TGLScene::OnTimer(int id __attribute__((unused)))
{
#ifdef RUN_SAMPLE
	// For example, redraw the scene
	Update_GLScene();
#endif
}

/* ================================================================== */
/*                          Miscellaneous method                      */
/* ================================================================== */

void TGLScene::OnError(const char *fmt, va_list ap)
{
	printf("Error callback:\n");

	// print warning message
	vprintf(fmt, ap);
	printf("\n");
	fflush(stdout);

	if (TerminateOnError) exit(1);
}

//---------------------------------------------------------------------------
/*
 * This function will be call only if we are in "Run" mode because we need glutMainLoop active
 */
void TGLScene::OnIdle(void)
{

}

//---------------------------------------------------------------------------
/*
 * Should be redefined accordingly the menu we have.
 */
void TGLScene::OnMenuEntry(int value)
{
	int menu = glutGetMenu();
	int window = glutGetWindow();
	printf("Menu %d MenuCallback for menu opened in Window %d - value is %d\n", menu, window, value);
	fflush(stdout);
}

/* ================================================================== */
/*                          Display method                            */
/* ================================================================== */

void TGLScene::SetFullScreen(bool on)
{
	FullScreen = on;
	if (on)
		glutFullScreen();
	else
		glutLeaveFullScreen();
}

//---------------------------------------------------------------------------
// Adjust the projection you want : glFrustum, gluPerspective, glOrtho, gluLookAt, ...
void TGLScene::SetProjection(int width, int height)
{
	GLdouble ar = (GLdouble)width / (GLdouble)height;

	switch (Projection)
	{
		case glpFrustum: glFrustum(-ar, ar, -1.0, 1.0, 10.0, 100.0); break;
		case glpPerspective: gluPerspective(45.0f, ar, 1.0, 300.0f); break;
		case glpOrtho: glOrtho(-ar, ar, -1.0, 1.0, 0.1, 1000.0); break;
		case glpLookAt: gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); break;
		default: ;
	}
}

//---------------------------------------------------------------------------
void TGLScene::InitialView(void)
{
	mouse_dragging = false;
	mouse_x = 0, mouse_y = 0;
	mouse_x0 = 0, mouse_y0 = 0;
	// First orientation : (1, 1, 1) en face
	mouse_Xrot = -135.0, mouse_Yrot = -45.0;
	mouse_scale = 1.0;
	Center_Translation = {0.0, 0.0, 0.0};
	display_mode = dmRender;
	redraw_view = true;
}

//---------------------------------------------------------------------------
void TGLScene::OnReshape(int width, int height)
{
	// Prevent any resize
	if (!CanResize)
	{
		glutReshapeWindow(MainWin.width, MainWin.height);
		return;
	}

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	SetProjection(width, height);
	ReDisplay();
	MainWin.height = height;
	MainWin.width = width;

	// Sub window
	if (SubWindow_ID >= 0)
	{
		SetSubWinSize();
		glutSetWindow(SubWindow_ID);
		glutPositionWindow(SubWin.posX, SubWin.posY);
		glutReshapeWindow(SubWin.width, SubWin.height);
		glutPostRedisplay();
		glutSetWindow(Window_ID);
	}
}

//---------------------------------------------------------------------------
// Callback display for the main window. This function is called by glutPostRedisplay.
void TGLScene::OnDisplay(void)
{
	if (display_running) return;
	display_running = true;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	// Light if needed
	light->Render();

	//camera->draw();

	if (redraw_view)
	{
		glLoadIdentity();
		glTranslated(0.0, 0.0, -50.0);

		glRotatef(mouse_Yrot, 1.0, 0.0, 0.0);
		glRotatef(mouse_Xrot, 0.0, 0.0, 1.0);

		glScaled(mouse_scale, mouse_scale, mouse_scale);

		glTranslatef(Center_Translation.X, Center_Translation.Y, Center_Translation.Z);
	}

	// Display objects
	for (int i = 0; i < ObjectCount; i++)
	{
		Object3DList[i]->Display(i, display_mode);
	}

	glutSwapBuffers();

	redraw_view = false;
	display_running = false;
}

//---------------------------------------------------------------------------
// Callback display for the sub window
void TGLScene::OnSubDisplay(void)
{
	TVector4D c;
	GLfloat x, y;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	c = SubWinColor.borderColor;
	glClearColor(c.X, c.Y, c.Z, c.alpha);

	x = (GLfloat)(SubWin.width - 2.0 * SubWinColor.borderWidth) / SubWin.width;
	y = (GLfloat)(SubWin.height - 2.0 * SubWinColor.borderWidth) / SubWin.height;
	c = SubWinColor.backgroundColor;
	glColor4f(c.X, c.Y, c.Z, c.alpha);
	glBegin(GL_QUADS);
	{
		glVertex3f(-x, y, 0.0f);   // Top Left
		glVertex3f(x, y, 0.0f);    // Top Right
		glVertex3f(x, -y, 0.0f);   // Bottom Right
		glVertex3f(-x, -y, 0.0f);  // Bottom Left
	}
	glEnd();

	c = SubWinColor.foregroundColor;
	glColor4f(c.X, c.Y, c.Z, c.alpha);
	SubDisplay();
	glutSwapBuffers();
}

//---------------------------------------------------------------------------
void TGLScene::CleanSubDisplay(void)
{
	// Refresh sub window
	if (SubWindow_ID >= 0)
	{
		glutSetWindow(SubWindow_ID);
		if (glutGetWindow() > 0)
		{
			glutPostWindowRedisplay(SubWindow_ID);
		}

		// Back to main window
		glutSetWindow(Window_ID);
	}
}

//---------------------------------------------------------------------------
void TGLScene::SubDisplay(void)
{
	ScenePrintf(1, 1, SubWinColor.borderWidth, "Object: %d", PickedObject);
}

//---------------------------------------------------------------------------
// On picking action. Here we redraw sub window
void TGLScene::DoPicking(int mod)
{
	bool DoDefault = true;

	// Callback to do other operations
	DoDefault = OnPicking(mod, GetObject3D(OldPickedObject), GetObject3D(PickedObject));

	// Default behaviour :
	// Reduce emission on picked object
	if (DoDefault)
	{
		if (OldPickedObject > 0) GetObject3D(OldPickedObject)->ChangeFrontEmission(-0.2);
		if (PickedObject > 0) GetObject3D(PickedObject)->ChangeFrontEmission(0.2);
	}

	// Refresh sub window
	glutSetWindow(SubWindow_ID);
	if (glutGetWindow() > 0)
	{
		glutPostWindowRedisplay(SubWindow_ID);
	}

	// Refresh main window
	glutSetWindow(Window_ID);
	ReDisplay(false);
}

//---------------------------------------------------------------------------
/**
 * Action to do when an object is picked. To be defined by user.
 * By default, we just reduce emission on picked object
 */
bool TGLScene::OnPicking(int mod __attribute__((unused)), TObject3D *Old __attribute__((unused)),
		TObject3D *New __attribute__((unused)))
{
	// Nothing to do here
	return true;
}

//---------------------------------------------------------------------------
// Special display for the picking
void TGLScene::DisplayMousePicking(int x, int y)
{
#define BUFFER_SIZE 256
	GLuint buff[BUFFER_SIZE] = { 0 };
	GLint hits, view[4];

	// Choose the buffer where store the values for the selection data
	glSelectBuffer(BUFFER_SIZE, buff);

	// Get info about the viewport
	glGetIntegerv(GL_VIEWPORT, view);

	// Switching in selection mode
	glRenderMode(GL_SELECT);
	display_mode = dmSelect;

	// Clearing and init the name's stack
	glInitNames();

	// Fill the stack with one element (or glLoadName will generate an error)
	glPushName(0);

	// Modify the vieving volume and restricting selection area around the cursor
  glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	{
		glLoadIdentity();

		// Restrict the viewing volume
		gluPickMatrix(x, view[3] - y, 5.0, 5.0, view);
		// Set same projection as normal view
		SetProjection(view[2], view[3]);

		// Draw the objects onto the screen in select mode
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);

		// Display objects
		for (int i = 0; i < ObjectCount; i++)
		{
			Object3DList[i]->Display(i, display_mode);
		}

		// DO NOT swap buffer !!
		// Go back to projection mode
		glMatrixMode(GL_PROJECTION);
	}
	glPopMatrix();
	glFlush();

	// Get number of objects drawed in that area and return to render mode
	hits = glRenderMode(GL_RENDER);
	display_mode = dmRender;

	// Check picking object
	PickList(buff, hits);
}

//---------------------------------------------------------------------------
void TGLScene::PickList(GLuint *buffer, GLint hits)
{
	/**
	 * For each hit in the buffer are allocated 4 bytes:
	 * 1. Number of hits selected (always one, because when we draw each object
	 * we use glLoadName, so we replace the previous name in the stack)
	 * 2. Min Z
	 * 3. Max Z
	 * 4. Name of all the hits (give by glLoadName). Here we have only one.
	 */
	GLint id_min = -1;
	if (hits > 0)
	{
		GLuint *pBuffer = &buffer[0];
		GLuint z_min = UINT_MAX;

		for (int i = 0; i < hits; i++)
		{
			if (*(++pBuffer) < z_min)
			{
				z_min = *pBuffer;
				id_min = *(pBuffer + 2);
			}
			pBuffer += 3;
		}
	}

//	std::cout << "Selected object: " << id_min << std::endl;
	OldPickedObject = PickedObject;
	PickedObject = id_min;
}

//---------------------------------------------------------------------------
void TGLScene::ScenePrintf(int row, int col, int border, const char *fmt, ...)
{
	static char buf[256];
	int viewport[4];
	void *font = GLUT_BITMAP_9_BY_15;
	va_list args;

	va_start(args, fmt);
#if defined(WIN32) && !defined(__CYGWIN__)
	(void)_vsnprintf(buf, sizeof(buf), fmt, args);
#else
    (void) vsnprintf(buf, sizeof(buf), fmt, args);
#endif
	va_end(args);

	glGetIntegerv(GL_VIEWPORT, viewport);

	glPushMatrix();
	{
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		{
			glLoadIdentity();
			glOrtho(0, viewport[2], 0, viewport[3], -1, 1);
			glRasterPos2i(glutBitmapWidth(font, ' ') * col, viewport[3] - glutBitmapHeight(font) * row - border);
			glutBitmapString(font, (unsigned char*)buf);
		}
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}
	glPopMatrix();
}

/* ================================================================== */
/*                          Events method                             */
/* ================================================================== */

void TGLScene::OnkeyEnter(unsigned char key __attribute__((unused)), int x __attribute__((unused)),
		int y __attribute__((unused)))
{
//	std::cout << "Key enter : " << key << " pressed at (" << x << ", " << y << ")" << std::endl;
}

//---------------------------------------------------------------------------
void TGLScene::OnkeyEnterUp(unsigned char key, int x __attribute__((unused)), int y __attribute__((unused)))
{
//	std::cout << "Key enter up : " << key << " pressed at (" << x << ", " << y << ")" << std::endl;

	switch (key)
	{
		case 27 : // Escape char
			if (FullScreen) SetFullScreen(false);
			break;
		case 'l':
		case 'L':
			light->SetLightColor(lcModelAmbient, 0.1);
			ReDisplay(false);
			break;
		case 'm':
		case 'M':
			light->SetLightColor(lcModelAmbient, -0.1);
			ReDisplay(false);
			break;
		case 'u':
		case 'U':
			CenterUpDown(0.5);
			break;
		case 'd':
		case 'D':
			CenterUpDown(-0.5);
			break;
		default: ;
	}
}

//---------------------------------------------------------------------------
void TGLScene::OnSpecialKey(int key __attribute__((unused)), int x __attribute__((unused)),
		int y __attribute__((unused)))
{
//	std::cout << "Special key " << key << " pressed at (" << x << ", " << y << ")" << std::endl;
}

//---------------------------------------------------------------------------
void TGLScene::OnSpecialKeyUp(int key, int x __attribute__((unused)), int y __attribute__((unused)))
{
//	std::cout << "Special key Up " << key << " pressed at (" << x << ", " << y << ")" << std::endl;

	switch (key)
	{
		case GLUT_KEY_UP:
			CenterUpDown(0.5);
			break;
		case GLUT_KEY_DOWN:
			CenterUpDown(-0.5);
			break;
		case GLUT_KEY_LEFT:
			LeftRight(-ROTATION_ANGLE);
			break;
		case GLUT_KEY_RIGHT:
			LeftRight(ROTATION_ANGLE);
			break;
		case GLUT_KEY_PAGE_UP:
			Zoom(1);
			break;
		case GLUT_KEY_PAGE_DOWN:
			Zoom(-1);
			break;
		default: ;
	}
}

//---------------------------------------------------------------------------
void TGLScene::OnMouseEnter(int state __attribute__((unused)))
{

}

//---------------------------------------------------------------------------
// Same event when click down and release (up)
void TGLScene::OnMouseClick(int button, int updown, int x, int y)
{
//	std::cout << "Mouse button " << button << " pressed at (" << x << ", " << y << ") updown " << updown << std::endl;

	if (button == GLUT_LEFT_BUTTON)
	{
		// We maybe begin a dragging
		if ((mouse_dragging = (updown == GLUT_DOWN)) == true)
		{
			mouse_x0 = mouse_x = x;
			mouse_y0 = mouse_y = y;
		}
		else // We just click on an object
		{
			if ((mouse_x0 == x) && (mouse_y0 == y))
			{
				DisplayMousePicking(x, y);
				DoPicking(glutGetModifiers());
			}
		}
	}
}

//---------------------------------------------------------------------------
void TGLScene::OnMouseMove(int x, int y)
{
	if (mouse_dragging)
	{
		mouse_Xrot += (x - mouse_x);
		mouse_Yrot += (y - mouse_y);
		mouse_x = x;
		mouse_y = y;
		ReDisplay();
	}
}

//---------------------------------------------------------------------------
void TGLScene::OnMousePassiveMove(int x __attribute__((unused)), int y __attribute__((unused)))
{

}

//---------------------------------------------------------------------------
void TGLScene::OnMouseWheel(int wheel __attribute__((unused)), int direction, int x __attribute__((unused)),
		int y __attribute__((unused)))
{
	double factor = (direction > 0) ? ZOOM_FACTOR_PLUS : ZOOM_FACTOR_MINUS;

	mouse_scale *= factor;
	ReDisplay();
}

// ********************************************************************************
// Screenshot : need wxWidgets
// ********************************************************************************
#ifdef __WXMSW__
void TGLScene::Screenshot(void)
{
	struct viewport_params
	{
			GLint originx;
			GLint originy;
			GLint x;
			GLint y;
	} viewport;

	// Build image from the 3D buffer
//   wxWindowUpdateLocker noUpdates( this );
	glGetIntegerv( GL_VIEWPORT, (GLint*)&viewport);

	unsigned char *pixelbuffer = (unsigned char*)malloc(viewport.x * viewport.y * 3);
	unsigned char *alphabuffer = (unsigned char*)malloc(viewport.x * viewport.y);

	glPixelStorei( GL_PACK_ALIGNMENT, 1);
	glReadBuffer( GL_BACK_LEFT);
	glReadPixels(viewport.originx, viewport.originy, viewport.x, viewport.y,
								GL_RGB, GL_UNSIGNED_BYTE, pixelbuffer);
	glReadPixels(viewport.originx, viewport.originy, viewport.x, viewport.y,
								GL_ALPHA, GL_UNSIGNED_BYTE, alphabuffer);

	wxImage image(viewport.x, viewport.y);
	image.SetData(pixelbuffer);
	image.SetAlpha(alphabuffer);
	image = image.Mirror(false);
	wxBitmap bitmap(image);

	wxBitmapDataObject *dobjBmp = new wxBitmapDataObject;
	dobjBmp->SetBitmap(bitmap);

	if (wxTheClipboard->Open())
	{
		if (!wxTheClipboard->SetData(dobjBmp))
			wxMessageBox(_("Failed to copy image to clipboard"));

		wxTheClipboard->Flush(); /* the data in clipboard will stay
		                          * available after the application exits */
		wxTheClipboard->Close();
	}

	// DO NOT free the buffer !!
//	free(pixelbuffer);
//	free(alphabuffer);
}
#endif

// ********************************************************************************
// OpenGL window construction
// ********************************************************************************
/**
 * Create the Context for the OpenGL Window
 * The context will be created only one time for all the life of the program.
 * Even if the window is closed when we are in "glutMainLoop()", we do not recreate
 * the context for a new window.
 * Default width = 800, height = 600, posX = 150, posY = 100
 */
void TGLScene::Create_GLContextWindow(int width, int height, int posX, int posY)
{
	if (GLContext_Exist) return;

	char *myargv[1];
	int myargc = 1;
	myargv[0] = strdup("GLScene app");

	MainWin.width = width;
	MainWin.height = height;
	MainWin.posX = posX;
	MainWin.posY = posY;
	glutInitWindowSize(width, height);
	glutInitWindowPosition(posX, posY);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA);
	try
	{
		glutInit(&myargc, myargv);
	}
	catch (...)
	{
		fprintf(stderr, "%s : Can not create OpenGL context. There probably already exist.\n", __FUNCTION__);
		fflush(stderr);
	}
	GLContext_Exist = true;
}

/**
 * Create a new OpenGL Window and assign the callback
 * Normally, the GLContext is already created
 */
void TGLScene::Create_GLWindow(const char *name, bool blending, TVector4D backcolor)
{
	// We only want one window
	if (GLWindow_Exist) return;

	if (!GLContext_Exist)
	{
		fprintf(stderr, "%s : OpenGL context undefined. Call to Create_GLContextWindow() before. I do it for you.\n",
				__FUNCTION__);
		fflush(stderr);
		Create_GLContextWindow();
	}

	// Create the window
	Window_ID = glutCreateWindow(name);
	// We don't want to close the appication when we close the window
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	// Initialize Glew to get information about OpenGL
#ifdef USE_GLEW
  InitGlew();
#endif

	// Enable back faces
	glEnable(GL_CULL_FACE);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Enable blending
	if (blending)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	// Usefull ?
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH, GL_NICEST);

	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH, GL_NICEST);

	// speedups ?
	glEnable(GL_DITHER);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	glClearDepth(1.0);

	// Default : White
	glClearColor(backcolor.R, backcolor.G, backcolor.B, backcolor.Alpha);

	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

	// Add user properties
	AddWindowProperties();

	// Initialize and assign the GLUT callback to the GLScene
	InitCallbacks(this);

	// Enable light
	light->Enable();

	GLWindow_Exist = true;
}

/**
 * Terminate the window
 */
void TGLScene::Terminate(void)
{
//	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_EXIT);
//	glutSetWindow(Window_ID);
//	glutLeaveMainLoop();
//	OnClose();
	glutDestroyWindow(Window_ID);
	glutMainLoopEvent(); // Call OnClose()
}

//---------------------------------------------------------------------------
void TGLScene::Create_GLSubWindow(int width, int height, int posX, int posY, glWinLocation loc)
{
	if (Window_ID >= 0)
	{
		SetSubWinSize(width, height, posX, posY, loc);
		SubWindow_ID = glutCreateSubWindow(Window_ID, SubWin.posX, SubWin.posY, SubWin.width, SubWin.height);
		InitSubCallbacks(this);
		glClearColor(0.7f, 0.7f, 0.7f, 1);
	}
}

//---------------------------------------------------------------------------
void TGLScene::SetSubWinSize(int width, int height, int posX, int posY, glWinLocation loc)
{
	GLint w, h;

	if (FullScreen)
	{
		w = glutGet(GLUT_SCREEN_WIDTH);
		h = glutGet(GLUT_SCREEN_HEIGHT);
	}
	else
	{
		w = MainWin.width;
		h = MainWin.height;
	}

	SubWin.loc = loc;
	switch (loc)
	{
		case glwNone:
		{
			SubWin.posX = posX;
			SubWin.posY = posY;
			SubWin.width = width;
			SubWin.height = height;
			break;
		}
		case glwLeftCenter:
		{
			SubWin.posX = SUB_WIN_MARGIN;
			SubWin.posY = (h - height) / 2;
			SubWin.width = width;
			SubWin.height = height;
			break;
		}
		case glwRightCenter:
		{
			SubWin.posX = w - width - SUB_WIN_MARGIN;
			SubWin.posY = (h - height) / 2;
			SubWin.width = width;
			SubWin.height = height;
			break;
		}
		case glwTopLeft:
		{
			SubWin.posX = SUB_WIN_MARGIN;
			SubWin.posY = SUB_WIN_MARGIN;
			SubWin.width = width;
			SubWin.height = height;
			break;
		}
		case glwTopCenter:
		{
			SubWin.posX = (w - width) / 2;
			SubWin.posY = SUB_WIN_MARGIN;
			SubWin.width = width;
			SubWin.height = height;
			break;
		}
		case glwTopRight:
		{
			SubWin.posX = w - width - SUB_WIN_MARGIN;
			SubWin.posY = SUB_WIN_MARGIN;
			SubWin.width = width;
			SubWin.height = height;
			break;
		}
		case glwTopAll:
		{
			SubWin.posX = SUB_WIN_MARGIN;
			SubWin.posY = SUB_WIN_MARGIN;
			SubWin.width = w - 2 * SUB_WIN_MARGIN;
			SubWin.height = height;
			break;
		}
		case glwBottomLeft:
		{
			SubWin.posX = SUB_WIN_MARGIN;
			SubWin.posY = h - height - SUB_WIN_MARGIN;
			SubWin.width = width;
			SubWin.height = height;
			break;
		}
		case glwBottomCenter:
		{
			SubWin.posX = (w - width) / 2;
			SubWin.posY = h - height - SUB_WIN_MARGIN;
			SubWin.width = width;
			SubWin.height = height;
			break;
		}
		case glwBottomRight:
		{
			SubWin.posX = w - width - SUB_WIN_MARGIN;
			SubWin.posY = h - height - SUB_WIN_MARGIN;
			SubWin.width = width;
			SubWin.height = height;
			break;
		}
		case glwBottomAll:
		{
			SubWin.posX = SUB_WIN_MARGIN;
			SubWin.posY = h - height - SUB_WIN_MARGIN;
			SubWin.width = w - 2 * SUB_WIN_MARGIN;
			SubWin.height = height;
			break;
		}
	}
}

//---------------------------------------------------------------------------
void TGLScene::SetSubWinSize()
{
	SetSubWinSize(SubWin.width, SubWin.height, SubWin.posX, SubWin.posY, SubWin.loc);
}

//---------------------------------------------------------------------------
void TGLScene::SetSubWinColor(glWinColor color)
{
	SubWinColor = color;
}

/**
 * Create a simple menu for the CURRENT window
 * Must be call before creating a sub window if it's for the main windows
 * Index start to 0
 */
void TGLScene::Create_GLMenu(std::initializer_list<const char*> menu)
{
	int menuID = InitCallbackMenu(this);

	// Create menu entry
	int count = 0;
	for (auto element : menu)
	{
		glutAddMenuEntry(element, count++);
	}

	// Change font for this menu for fun
	glutSetMenuFont(menuID, GLUT_BITMAP_HELVETICA_12);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//---------------------------------------------------------------------------
void TGLScene::AddWindowProperties()
{
#ifdef __WXMSW__
	// For Screenshot
	glReadBuffer(GL_BACK);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
#endif
}

// ********************************************************************************
// End of file
// ********************************************************************************
