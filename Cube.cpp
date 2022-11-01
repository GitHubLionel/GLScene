#include "Cube.h"
#include <math.h>

TCube::TCube(GLfloat _size, TVector3D *pos) : TObject3D(pos)
{
  ObjectType = otCube;
  size = _size;
  IsCube = true;

  ambient = GLColor_blue;
  diffuse = GLColor_blue;
  emission = {0.1, 0.1, 0.1, 1.0};
  specular = {1.0, 1.0, 1.0, 1.0};
  shininess = {50.0, 0.0, 0.0, 0.0};
  direction = {0.0, 0.0, 1.0};

  ComputeParameters();
}

TCube::TCube(TVector3D _size, TVector3D *pos) : TObject3D(pos)
{
  ObjectType = otCuboid;
  size = 1;
  size3D = _size;
  IsCube = false;

  ambient = GLColor_blue;
  diffuse = GLColor_blue;
  emission = {0.1, 0.1, 0.1, 1.0};
  specular = {1.0, 1.0, 1.0, 1.0};
  shininess = {50.0, 0.0, 0.0, 0.0};
  direction = {0.0, 0.0, 1.0};

  ComputeParameters();
}

TCube::~TCube()
{

}

void TCube::ComputeParameters(void)
{
  reduced_pos = direction;
  reduced_pos.Normalize();
  axe_angle = {-reduced_pos.Y, reduced_pos.X, 0.0};
  angle = RADtoDEG * acos(reduced_pos.Z / size);
}

void TCube::SetPosition(std::initializer_list<GLfloat> list)
{
  int count = 0;
  for (auto element : list)
  {
    position.Set(count, element);
    ++count;
  }
  ComputeParameters();
}

void TCube::SetDirection(std::initializer_list<GLfloat> list)
{
  int count = 0;
  for (auto element : list)
  {
    direction.Set(count, element);
    ++count;
  }
  ComputeParameters();
}

void TCube::DoDisplay(TDisplayMode mode __attribute__((unused)))
{
  glTranslated(position.X, position.Y, position.Z);
  if (!axe_angle.IsNull())
    glRotated(angle, axe_angle.X, axe_angle.Y, axe_angle.Z);
	// Parallelepiped case, we scale the cube
	if (!IsCube)
		glScalef(size3D.X, size3D.Y, size3D.Z);
  glutSolidCube(size);
}

// ********************************************************************************
// End of file
// ********************************************************************************
