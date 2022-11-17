#include "Arrow.h"
#include <math.h>

TArrow::TArrow(GLfloat _len, GLfloat _thickness, TVector3D *pos) : TObject3D(pos)
{
  ObjectType = otArrow;
  len = _len;
  len2 = _len/2.0;
  thickness = _thickness;
  levelOfDetail = 10;

  material.SetColor(mfFront, msAmbient, GLColor_red);
  material.SetColor(mfFront, msDiffuse, GLColor_red);

  direction = {1.0, 1.0, 1.0};

  ComputeParameters();
}

TArrow::~TArrow()
{

}

void TArrow::ComputeParameters(void)
{
  reduced_pos = direction;
  reduced_pos.Normalize();
  reduced_pos *= len2;
  axe_angle = {-reduced_pos.Y, reduced_pos.X, 0.0};
  angle = RADtoDEG * acos(reduced_pos.Z / len2);
}

void TArrow::SetPosition(std::initializer_list<GLfloat> list)
{
  int count = 0;
  for (auto element : list)
  {
    position.Set(count, element);
    ++count;
  }
  ComputeParameters();
}

void TArrow::SetDirection(std::initializer_list<GLfloat> list)
{
  int count = 0;
  for (auto element : list)
  {
    direction.Set(count, element);
    ++count;
  }
  ComputeParameters();
}

void TArrow::DoDisplay(TDisplayMode mode)
{
  TVector3D a;
  float detail;

  if (mode == dmSelect)
  	detail = 10.0;
  else detail = levelOfDetail;

  a = position - reduced_pos;

  glPushMatrix();
    glTranslated(a.X, a.Y, a.Z);
    glRotated(angle, axe_angle.X, axe_angle.Y, axe_angle.Z);
    glutSolidCylinder(thickness, len2, detail, detail);
  glPopMatrix();

  a = position;

  glPushMatrix();
    glTranslated(a.X, a.Y, a.Z);
    glRotated(angle, axe_angle.X, axe_angle.Y, axe_angle.Z);
    glutSolidCone(thickness*4, len2, detail, detail);
  glPopMatrix();
}

// ********************************************************************************
// End of file
// ********************************************************************************
