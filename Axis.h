#ifndef _AXIS_3D_H
#define _AXIS_3D_H

#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>
#include "Vector3D.h"
#include "Vector4D.h"
#include "Object3D.h"

namespace GLScene
{

/**
 * TAxis class
 * Params: l length of the axe
 */
class TAxis : public TObject3D
{
  private:
    GLfloat size;
    GLfloat colorRed[4] = {1.0, 0.0, 0.0, 1.0};
    GLfloat colorGreen[4] = {0.0, 1.0, 0.0, 1.0};
    GLfloat colorBlue[4] = {0.0, 0.0, 1.0, 1.0};
    float font_height;
    float font_width;

  protected:

  public:
    TAxis(GLfloat l = 10);
    virtual ~TAxis();

    void virtual DoDisplay(TDisplayMode mode = dmRender);
};

} // namespace GLScene

//---------------------------------------------------------------------------
#endif // _AXIS_3D_H
