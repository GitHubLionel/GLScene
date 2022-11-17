#ifndef _CUBE_3D_H
#define _CUBE_3D_H

#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>
#include "Vector3D.h"
#include "Vector4D.h"
#include "Object3D.h"

namespace GLScene
{

/**
 * TCube class
 * Params: size of the cube or the 3 size of a parallelepiped over x, y and z
 * Params: position (default (0,0,0))
 * The cube is centered to it's position and initial direction is (0,0,1)
 */
class TCube : public TObject3D
{
  private:
    GLfloat size;
    TVector3D size3D;
    bool IsCube;
    GLfloat angle;
    TVector3D axe_angle;
    TVector3D reduced_pos;
    void ComputeParameters(void);
  protected:

  public:
    TCube(GLfloat _size, TVector3D *pos = NULL);
    TCube(TVector3D _size, TVector3D *pos = NULL);
    TCube(GLfloat _size, const TVector3D &pos);
    TCube(TVector3D _size, const TVector3D &pos);
    virtual ~TCube();

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
#endif // _CUBE_3D_H
