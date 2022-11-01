#ifndef _ARROW_3D_H
#define _ARROW_3D_H

#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>
#include "Vector3D.h"
#include "Vector4D.h"
#include "Object3D.h"

namespace GLScene
{

/**
 * TArrow class
 * Params: length
 * Params: thickness
 * Params: position (default (0,0,0))
 * The arrow is centered to it's position and initial direction is (1,1,1)
 */
class TArrow : public TObject3D
{
  private:
    GLfloat len;
    GLfloat len2;
    GLfloat thickness;
    GLfloat angle;
    TVector3D axe_angle;
    TVector3D reduced_pos;
    void ComputeParameters(void);
  protected:

  public:
    TArrow(GLfloat _len, GLfloat _thickness, TVector3D *pos = NULL);
    virtual ~TArrow();

    void SetPosition(const TVector3D *pos) { position = *pos; ComputeParameters(); }
    void SetPosition(GLfloat pos[]) { position = pos; ComputeParameters(); }
    void SetPosition(std::initializer_list<GLfloat> list);
    void SetPosition(GLfloat x, GLfloat y, GLfloat z) { position = {x, y, z}; ComputeParameters(); }

    void SetDirection(const TVector3D *pos) { direction = *pos; ComputeParameters(); }
    void SetDirection(GLfloat pos[]) { direction = pos; ComputeParameters(); }
    void SetDirection(std::initializer_list<GLfloat> list);
    void SetDirection(GLfloat x, GLfloat y, GLfloat z) { direction = {x, y, z}; ComputeParameters(); }

    void DoDisplay(TDisplayMode mode = dmRender);
};

} // namespace GLScene

//---------------------------------------------------------------------------
#endif // _ARROW_3D_H
