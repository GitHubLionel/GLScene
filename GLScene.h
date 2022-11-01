#ifndef _SCENE_3D_H
#define _SCENE_3D_H

#include <math.h>
#include <stdio.h>
#include <initializer_list> // for std::initializer_list
#include <vector>  // for std::vector
#include "GLCallback.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Object3D.h"
#include "Light.h"
#include "Camera.h"

/**
 * GLScene is a small OpenGL implementation to create simple object
 */
namespace GLScene
{

#define MAX_OBJECT 20000
#define SUB_WIN_MARGIN 0

typedef enum __gl_Location_Type {
	glwLeftCenter,
	glwTopLeft,
	glwTopCenter,
	glwTopRight,
	glwTopAll,
	glwRightCenter,
	glwBottomLeft,
	glwBottomCenter,
	glwBottomRight,
	glwBottomAll,
	glwNone
} glWinLocation;

typedef enum __gl_Projection {
	glpFrustum,
	glpPerspective,
	glpOrtho,
	glpLookAt
} glProjection;

typedef struct {
		int borderWidth;
		TVector4D borderColor;
		TVector4D backgroundColor;
		TVector4D foregroundColor;
} glWinColor;

typedef struct {
		GLuint posX;
		GLuint posY;
		GLuint width;
		GLuint height;
		glWinLocation loc;
} glWinSize;

typedef std::vector<TObject3D *> TObject3DVector;

class TGLScene: virtual public GLCallBack {
	private:
		bool GLContext_Exist;
		bool GLWindow_Exist;
		bool IsRunning;
		bool CanResize;
		TVector3D Center_Translation;

	protected:
		TDisplayMode display_mode;
		// For mouse move
		bool mouse_dragging = false;
		int mouse_x = 0, mouse_y = 0;
		int mouse_x0 = 0, mouse_y0 = 0;
		GLfloat mouse_Xrot = 0.0, mouse_Yrot = 0.0;
		GLfloat mouse_scale = 0.0;

		bool redraw_view = false;
		bool display_running = false;
		bool FullScreen = false;
		void ReDisplay(bool _redraw_view = true);
		void PickList(GLuint *buffer, GLint hits);

	public:
		TGLScene();
		virtual ~TGLScene();

		/// If an OpenGL is raised then terminate the program. Default false.
		bool TerminateOnError;

		TLight *light;
//    TCamera *camera;

		int ObjectCount;
		TObject3DVector Object3DList;

		GLint PickedObject;
		GLint OldPickedObject;
		glWinSize MainWin;
		glWinSize SubWin;
		glWinColor SubWinColor;

		glProjection Projection;

		// Basic actions
		void Run();
		void Stop();
		void Show();
		void Hide();
		void Refresh();

		void SetResizable(bool resizable = true)
		{
			CanResize = resizable;
		}

		void Zoom(const int inc = 1);
		void CenterUpDown(const GLfloat incZ);
		void UpDown(const GLfloat incY);
		void LeftRight(const GLfloat incX);

		int GetWidth()
		{
			return MainWin.width;
		}

		int GetHeight()
		{
			return MainWin.height;
		}

		// Printf
		void ScenePrintf(int row, int col, int border, const char *fmt, ...);

		// Object gestion
		int AddObject3D(TObject3D *obj, bool autoFree = true);
		void DeleteObject3D(const int index);
		TObject3D* GetObject3D(const int index);
		TObject3D* GetObject3D(TVector3D position, TObjectType type);
		void ClearObject3D(bool ForceFree = true);

		// Scene gestion
		virtual void Create_GLScene(void);
		virtual void Update_GLScene(bool Repaint = true);
		virtual void SetProjection(int width, int height);
		void SetFullScreen(bool on);

		// Picking
		void DisplayMousePicking(int x, int y);
		virtual void DoPicking(int mod);
		virtual bool OnPicking(int mod, TObject3D *Old, TObject3D *New);

		// Callback for OpenGL
		virtual void OnError(const char *fmt, va_list ap);

		virtual void OnDisplay(void);

		virtual void OnReshape(int width, int height);
		virtual void OnClose(void);
		virtual void OnIdle(void);
		virtual void OnTimer(int id);

		/// Events
		virtual void OnkeyEnter(unsigned char key, int x, int y);
		virtual void OnkeyEnterUp(unsigned char key, int x, int y);
		virtual void OnSpecialKey(int key, int x, int y);
		virtual void OnSpecialKeyUp(int key, int x, int y);

		virtual void OnMouseEnter(int state);
		virtual void OnMouseClick(int button, int updown, int x, int y);
		virtual void OnMouseMove(int x, int y);
		virtual void OnMousePassiveMove(int x, int y);
		virtual void OnMouseWheel(int wheel, int direction, int x, int y);

		// Menu events
		virtual void OnMenuEntry(int value);

		// Screenshot : need wxWidgets
#ifdef __WXMSW__
		void Screenshot(void);
#endif

		/// Sub window gestion
		virtual void SubDisplay(void);
		/// Callback for OpenGL for display sub window
		virtual void OnSubDisplay(void);

		/// Creation of the Context for the OpenGL Window
		void Create_GLContextWindow(int width = 800, int height = 600, int posX = 150, int posY = 100);
		/// Creation of a new OpenGL Window
		void Create_GLWindow(const char *name, bool blending = false);
		/// Creation of a new sub Window
		void Create_GLSubWindow(int width, int height, int posX, int posY, glWinLocation loc = glwNone);

		/// Return true if the OpenGL window exist
		bool Is_Window_Exist(void)
		{
			return GLWindow_Exist;
		}

		void SetSubWinSize(int width, int height, int posX, int posY, glWinLocation loc = glwNone);
		void SetSubWinSize();

		void SetSubWinColor(glWinColor color);

		// Creation of a menu
		void Create_GLMenu(std::initializer_list<const char*> menu);
		// Get info
		void Get_GLInfo();
		// Add additionnal property to the window
		virtual void AddWindowProperties();

#ifdef USE_GLEW
    friend const char *GetGlewVersion();
#endif
};

} // namespace GLScene

//---------------------------------------------------------------------------
#endif // _SCENE_3D_H
