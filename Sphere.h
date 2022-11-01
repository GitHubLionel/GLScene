#ifndef _SPHERE_3D_H
#define _SPHERE_3D_H

#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>
#include "Vector3D.h"
#include "Vector4D.h"
#include "Object3D.h"

namespace GLScene
{

/**
 * TSphere class
 * Params: radius of the sphere or the 3 radius of the ellipsoïd over x, y and z
 * Params: position (default (0,0,0))
 * The sphere/ellipsoïd is centered to it's position
 */
class TSphere : public TObject3D
{
  private:
    GLfloat radius;
    TVector3D radius3D;
    bool IsSphere;
  protected:

  public:
    TSphere(GLfloat _radius, TVector3D *pos = NULL);
    TSphere(TVector3D _radius, TVector3D *pos = NULL);
    virtual ~TSphere();

    void SetRadius(GLfloat _radius)
    {
    	radius = _radius;
    	IsSphere = true;
    }

    void SetRadius(TVector3D _radius)
    {
    	radius3D = _radius;
    	IsSphere = false;
    }

    void virtual DoDisplay(TDisplayMode mode = dmRender);
};

} // namespace GLScene

//---------------------------------------------------------------------------
#endif // _SPHERE_3D_H
