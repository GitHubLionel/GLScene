#include "Sphere.h"

TSphere::TSphere(GLfloat _radius, TVector3D *pos) : TObject3D(pos, NULL, NULL)
{
  ObjectType = otSphere;
  radius = _radius;
  levelOfDetail = 20;
  IsSphere = true;

  // Yellow sphere
//  ambient = {0.8, 0.8, 0.2, 1.0};
  ambient = GLColor_yellow;
  diffuse = GLColor_yellow;
  emission = {0.1, 0.1, 0.1, 1.0};
  specular = {0.1, 0.1, 0.1, 1.0};
  shininess = {0.0, 0.0, 0.0, 0.0};

  DoPosition = true;
}

TSphere::TSphere(TVector3D _radius, TVector3D *pos) : TObject3D(pos, NULL, NULL)
{
  ObjectType = otEllipsoid;
  radius3D = _radius;
  levelOfDetail = 25;
  IsSphere = false;
  radius = 1;

  // Yellow ellipsoïd
//  ambient = {0.8, 0.8, 0.2, 1.0};
  ambient = GLColor_yellow;
  diffuse = GLColor_yellow;
  emission = {0.1, 0.1, 0.1, 1.0};
  specular = {0.1, 0.1, 0.1, 1.0};
  shininess = {0.0, 0.0, 0.0, 0.0};

  DoPosition = true;
}

TSphere::~TSphere()
{

}

void TSphere::DoDisplay(TDisplayMode mode)
{
	// Ellipsoïd case, we scale the sphere
	if (!IsSphere)
		glScalef(radius3D.X, radius3D.Y, radius3D.Z);

	if (mode == dmSelect)
		glutSolidSphere(radius, 10, 10); // Low resolution
	else
		glutSolidSphere(radius, levelOfDetail, levelOfDetail);
}

// ********************************************************************************
// End of file
// ********************************************************************************
