#ifndef _GLCALLBACK_H
#define	_GLCALLBACK_H

#include <stdbool.h>
#include <stdarg.h>  // for va_list

namespace GLScene
{

class GLCallBack
{
  protected:
    int Window_ID = -1;
    int SubWindow_ID = -1;

  public:
    GLCallBack() { };
    virtual ~GLCallBack() { };

    int Get_Window_ID() { return Window_ID; };
    int Get_SubWindow_ID() { return SubWindow_ID; };

    // Step for the timer in ms. Must be initialized before Create_GLWindow() call
    int TimerStepms = 1000;

    virtual void OnError(const char *fmt, va_list ap) = 0;

    virtual void OnDisplay(void) = 0;
    virtual void OnReshape(int width, int height) = 0;
    virtual void OnClose(void) = 0;
    virtual void OnIdle(void) = 0;
    virtual void OnTimer(int id) = 0;

    /// Events
    // Keyboard events
    virtual void OnkeyEnter(unsigned char key, int x, int y) = 0;
    virtual void OnkeyEnterUp(unsigned char key, int x, int y) = 0;
    virtual void OnSpecialKey(int key, int x, int y) = 0;
    virtual void OnSpecialKeyUp(int key, int x, int y) = 0;
    
    // Mouse events
    virtual void OnMouseEnter(int state) = 0;
    virtual void OnMouseClick(int button, int updown, int x, int y) = 0;
    virtual void OnMouseMove(int x, int y) = 0;
    virtual void OnMousePassiveMove(int x, int y) = 0;
    virtual void OnMouseWheel(int wheel, int direction, int x, int y) = 0;
    
    // Menu events
    virtual void OnMenuEntry(int value) = 0;

    // For sub window
    virtual void OnSubDisplay(void) = 0;

    // The unique GLContext
    static bool GLContext_Exist;
};

void InitCallbacks(GLCallBack* _pGLScene);
void InitSubCallbacks(GLCallBack* _pGLScene);
int InitCallbackMenu(GLCallBack* _pGLScene);

#ifdef USE_GLEW
void InitGlew();
const char *GetGlewVersion();
#endif

} // namespace GLScene

//---------------------------------------------------------------------------
#endif	// _GLCALLBACK_H
