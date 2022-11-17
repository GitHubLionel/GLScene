#include "Sphere.h"

TSphere::TSphere(GLfloat _radius, TVector3D *pos) : TObject3D(pos, NULL, NULL)
{
  ObjectType = otSphere;
  radius = _radius;
  levelOfDetail = 20;
  IsSphere = true;

  // Yellow sphere
	material.SetColor(mfFront, msAmbient, {0.647059, 0.164706, 0.164706, 1.0});
	material.SetColor(mfFront, msDiffuse, GLColor_yellow);

	material.SetColor(mfBack, msAmbient, {0.2, 0.2, 0.2, 1.0});
	material.SetColor(mfBack, msDiffuse, {0.8, 0.8, 0.8, 1.0});

  DoPosition = true;
}

TSphere::TSphere(GLfloat _radius, const TVector3D &pos) : TSphere(_radius)
{
	position = pos;
}

TSphere::TSphere(TVector3D _radius, TVector3D *pos) : TObject3D(pos, NULL, NULL)
{
  ObjectType = otEllipsoid;
  radius3D = _radius;
  levelOfDetail = 25;
  IsSphere = false;
  radius = 1;

  // Yellow sphere
	material.SetColor(mfFront, msAmbient, {0.647059, 0.164706, 0.164706, 1.0});
	material.SetColor(mfFront, msDiffuse, GLColor_yellow);

	material.SetColor(mfBack, msAmbient, {0.2, 0.2, 0.2, 1.0});
	material.SetColor(mfBack, msDiffuse, {0.8, 0.8, 0.8, 1.0});

  DoPosition = true;
}

TSphere::TSphere(TVector3D _radius, const TVector3D &pos) : TSphere(_radius)
{
	position = pos;
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
