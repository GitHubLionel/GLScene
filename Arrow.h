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
    void ComputeParameters(bool normalize = true);
  protected:

  public:
    TArrow(GLfloat _len, GLfloat _thickness, TVector3D *pos = NULL);
    virtual ~TArrow();

    void SetPosition(const TVector3D *pos, bool normalize = true) { position = *pos; ComputeParameters(normalize); }
    void SetPosition(GLfloat pos[], bool normalize = true) { position = pos; ComputeParameters(normalize); }
    void SetPosition(std::initializer_list<GLfloat> list, bool normalize = true);
    void SetPosition(GLfloat x, GLfloat y, GLfloat z, bool normalize = true) { position = {x, y, z}; ComputeParameters(normalize); }

    void SetDirection(const TVector3D *pos, bool normalize = true) { direction = *pos; ComputeParameters(normalize); }
    void SetDirection(GLfloat pos[], bool normalize = true) { direction = pos; ComputeParameters(normalize); }
    void SetDirection(std::initializer_list<GLfloat> list, bool normalize = true);
    void SetDirection(GLfloat x, GLfloat y, GLfloat z, bool normalize = true) { direction = {x, y, z}; ComputeParameters(normalize); }

    void DoDisplay(TDisplayMode mode = dmRender);
};

} // namespace GLScene

//---------------------------------------------------------------------------
#endif // _ARROW_3D_H
