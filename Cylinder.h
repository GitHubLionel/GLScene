#ifndef _CYLINDER_3D_H
#define _CYLINDER_3D_H

#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>
#include "Vector3D.h"
#include "Vector4D.h"
#include "Object3D.h"

namespace GLScene
{

/**
 * TCylinder class
 * Params: height
 * Params: radius
 * Params: position (default (0,0,0))
 * The cylinder is centered to it's position and initial direction is (0,0,1)
 */
class TCylinder : public TObject3D
{
  private:
    GLfloat height;
    GLfloat height2;
    GLfloat radius;
    GLfloat angle;
    TVector3D axe_angle;
    TVector3D reduced_pos;
    void ComputeParameters(void);
  protected:

  public:
    TCylinder(GLfloat _height, GLfloat _radius, TVector3D *pos = NULL);
    virtual ~TCylinder();

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
#endif // _CYLINDER_3D_H
