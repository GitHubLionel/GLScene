
#include "GLScene.h"
#include "Axis.h"
#include "Sphere.h"

using namespace GLScene;

// Si on définie RUN_SAMPLE dans le projet, on aura l'exemple de base de TGLScene

/**
 * Une scène toute simple avec juste des sphères
 */
class MPScene : public TGLScene
{
  private:
  
  protected:

  public:
    MPScene() {}
    virtual ~MPScene() {}
    virtual void Create_GLScene(void);
    virtual void OnMenuEntry(int value);
};

// Les sphères positionnées au hasard
void MPScene::Create_GLScene(void)
{
  TVector3D p;

  // Clear existing object
  ClearObject3D();

  // Restaure initial position
  InitialView();

  // Clean sub windows
  CleanSubDisplay();

  // Un axe
  TAxis *axis = new TAxis(5);
  AddObject3D(axis);
  
  // Des sphères au hasard
  for (int i=0; i<1000; i++)
  {
  	p = {GLRand(20.0), GLRand(20.0), GLRand(20.0)};
  	TSphere *sphere = new TSphere(0.2, &p);
  	AddObject3D(sphere);
  }
}

// Un petit menu tout simple
void MPScene::OnMenuEntry(int value __attribute__((unused)))
{
  int i;
  TVector3D p;
  TObject3D *obj;

  for (i=0; i<ObjectCount; i++)
  {
    if (i % 2)
    {
      obj = GetObject3D(i);
      obj->Visible = !obj->Visible;
    }
  }
  ReDisplay(false);
}

// *************************** Main ************************
int main()
{
	// Création de la scène
#ifdef RUN_SAMPLE
	TGLScene *TheScene = new TGLScene();  // l'exemple intégré dans TGLScene
#else
	MPScene *TheScene = new MPScene();
#endif

	// Création du contexte
	TheScene->Create_GLContextWindow();
	// Création de la scène
	TheScene->Create_GLScene();
	// Pour le timer pour une animation
	TheScene->TimerStepms = 500;

  // Création de la windows
  TheScene->Create_GLWindow("GLScene", true, GLColor_teal);

  // Création du Menu
  TheScene->Create_GLMenu({"Menu 1", "Menu 2"});

  // Création de la sous window
  TheScene->Create_GLSubWindow(200, 50, 4, 200, glwBottomAll);

  // Quelques infos
  TheScene->Get_GLInfo();

  // Affichage
  TheScene->Show();

  TheScene->Run();

  return 0;
}
