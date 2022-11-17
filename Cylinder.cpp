#include "Cylinder.h"
#include <math.h>

TCylinder::TCylinder(GLfloat _height, GLfloat _radius, TVector3D *pos) : TObject3D(pos)
{
  ObjectType = otCylinder;
  height = _height;
  height2 = _height/2.0;
  radius = _radius;
  levelOfDetail = 20;

  direction = {0.0, 0.0, 1.0};

	material.SetColor(mfFront, msAmbient, {0.8, 0.1, 0.1, 1.0});
	material.SetColor(mfFront, msDiffuse, GLColor_red);

	material.SetColor(mfBack, msAmbient, {0.2, 0.2, 0.2, 1.0});
	material.SetColor(mfBack, msDiffuse, {0.8, 0.8, 0.8, 1.0});

	ComputeParameters();
}

TCylinder::TCylinder(GLfloat _height, GLfloat _radius, const TVector3D &pos) : TCylinder(_height, _radius)
{
	position = pos;
	ComputeParameters();
}

TCylinder::~TCylinder()
{

}

void TCylinder::ComputeParameters(void)
{
  reduced_pos = direction;
  reduced_pos.Normalize();
  reduced_pos *= height2;
  axe_angle = {-reduced_pos.Y, reduced_pos.X, 0.0};
  angle = RADtoDEG * acos(reduced_pos.Z / height2);
  reduced_pos = position - reduced_pos;
}

void TCylinder::SetPosition(std::initializer_list<GLfloat> list)
{
  int count = 0;
  for (auto element : list)
  {
    position.Set(count, element);
    ++count;
  }
  ComputeParameters();
}

void TCylinder::SetDirection(std::initializer_list<GLfloat> list)
{
  int count = 0;
  for (auto element : list)
  {
    direction.Set(count, element);
    ++count;
  }
  ComputeParameters();
}

void TCylinder::DoDisplay(TDisplayMode mode)
{
  glTranslated(reduced_pos.X, reduced_pos.Y, reduced_pos.Z);
  glRotated(angle, axe_angle.X, axe_angle.Y, axe_angle.Z);
  if (mode == dmSelect)
  	glutSolidCylinder(radius, height, 10, 10);  // Low resolution
  else
  glutSolidCylinder(radius, height, levelOfDetail, levelOfDetail);
}

// ********************************************************************************
// End of file
// ********************************************************************************
