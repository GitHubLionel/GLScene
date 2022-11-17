#include "Object3D.h"

/**
 * Constructor
 * pos : position
 * rot : rotation
 * sca : scale
 */
TObject3D::TObject3D(TVector3D *pos, TVector3D *rot, TVector3D *sca)
{
	defaultSettings();
	if (pos) SetPosition(pos);
	if (rot) SetRotation(rot);
	if (sca) SetScale(sca);
}

TObject3D::TObject3D(const TVector3D &pos)
{
	defaultSettings();
	position = pos;
}

TObject3D::~TObject3D()
{

}

void TObject3D::defaultSettings()
{
	ObjectType = otNone;
	// By default, you must freeing manualy the object
	AutoFree = false;
	// Position to center world
	position = { 0.0, 0.0, 0.0 };
	// Direction : Z axis
	direction = { 1.0, 0.0, 0.0 };
	// No axis rotation
	rotation = { 0.0, 0.0, 0.0 };
	// No scale
	scale = { 1.0, 1.0, 1.0 };

	UserPointer = NULL;
	Tag = 0;

	Lighted = true;
	Visible = true;
	Changed = true;

	levelOfDetail = 12;
	selected = false;
}

// SETTERS
//-------------------------------------------------

void TObject3D::SetPosition(std::initializer_list<GLfloat> list)
{
	int count = 0;
	for (auto element : list)
	{
		position.Set(count, element);
		++count;
	}
	Changed = true;
}

void TObject3D::SetDirection(std::initializer_list<GLfloat> list)
{
	int count = 0;
	for (auto element : list)
	{
		direction.Set(count, element);
		++count;
	}
	Changed = true;
}

void TObject3D::SetRotation(TVector3D *rot)
{
	rotation = *rot;
	Changed = true;
}

void TObject3D::SetScale(TVector3D *sca)
{
	scale = *sca;
	Changed = true;
}

// set level of detail
void TObject3D::SetLevelOfDetail(float _levelOfDetail)
{
	if (_levelOfDetail > 0 && _levelOfDetail < 50)
	{
		levelOfDetail = _levelOfDetail;
		Changed = true;
	}
}

void TObject3D::ChangeFrontEmission(GLfloat val)
{
	material.SetColor(mfFront, msEmission, val);
}

// DRAW
//-------------------------------------------------
void TObject3D::Display(int id, TDisplayMode mode)
{
	if (!Visible) return;

	glPushMatrix();

	// translation
	if (DoPosition) glTranslatef(position.X, position.Y, position.Z);

	// rotation
	if (DoRotation)
	{
		glRotatef(rotation.X, 1.0, 0.0, 0.0);
		glRotatef(rotation.Y, 0.0, 1.0, 0.0);
		glRotatef(rotation.Z, 0.0, 0.0, 1.0);
	}

	// scale
	if (DoScale) glScalef(scale.X, scale.Y, scale.Z);

	if (DoUseMaterial && (mode == dmRender))
	{
		material.ApplyMaterial();
	}

	// Set a name in select mode
	if (mode == dmSelect)
	  glLoadName(id);
	DoDisplay(mode);
	glPopMatrix();
}

/**
 * Move the center of the object to the 'position' and make the rotation to
 * be in 'direction' for drawing an object with 'len' size in the direction
 * The original object is over the 'z_direction'
 * Don't forget glPushMatrix(); before call and glPopMatrix(); after draw object
 */
//void TObject3D::PrepareRotate(GLfloat len)
//{
//  TVector3D a, b; // (the two points you want to draw between : low and high)
//  TVector3D p, t;
//  TVector3D d = direction;
//  d.Normalize();
//  d *= (len/2.0);
//
//  a = position - d;
//  b = position + d;
//
//  // Get diff between two points you want cylinder along
//  p = (b - a);
//
//  // Get CROSS product (the axis of rotation)
//  t = z_direction.Vectorial(p);
//
//  // Get angle. LENGTH is magnitude of the vector
//  GLfloat angle = DEGtoRAD * acos(z_direction.Dot(p) / p.Length());
//
//  glTranslated(a.X, a.Y, a.Z);
//  glRotated(angle, t.X, t.Y, t.Z);
//}
void TObject3D::PrepareRotate(GLfloat len)
{
	TVector3D a, p, t;
	TVector3D d = direction;
	d.Normalize();

	p = d * len;
	d *= (len / 2.0);
	a = position - d;
	t = { -p.Y, p.X, 0.0 };
	GLfloat angle = RADtoDEG * acos(p.Z / len);

	glTranslated(a.X, a.Y, a.Z);
	glRotated(angle, t.X, t.Y, t.Z);
}

// ********************************************************************************
// Random functions
// ********************************************************************************

/**
 * Return a random number between [-1, 1]
 */
GLfloat GLScene::GLRand(void)
{
	return (GLfloat) ((1.0 * rand() / RAND_MAX - 0.5) * 2.0);
}

/**
 * Return a random number between [0, max]
 */
GLfloat GLScene::GLRand(GLfloat max)
{
	return (GLfloat)(rand())/(RAND_MAX*1.0) * max;
}

// ********************************************************************************
// End of file
// ********************************************************************************
