#include "Axis.h"
#include <string.h>

/**
 * TAxis class
 * Params: l length of the axe
 */
TAxis::TAxis(GLfloat l) : TObject3D()
{
  ObjectType = otAxis;
  size = l;
  float textsize = 0.5;
  // To do that, GLContext must be defined
  if (glutGet(GLUT_INIT_STATE))
  {
    font_height = textsize / glutStrokeHeight(GLUT_STROKE_MONO_ROMAN);
    font_width  = textsize / glutStrokeLength(GLUT_STROKE_MONO_ROMAN, (unsigned char *)"X");
  }

  ambient = {0.0, 0.0, 0.0, 1.0};
  diffuse = {0.5, 0.5, 1.0, 1.0};
  specular = {0.0, 0.0, 0.0, 1.0};
  emission = {0.2, 0.2, 0.2, 1.0};
  shininess = {10.0, 0.0, 0.0, 0.0};
}

TAxis::~TAxis()
{

}

void TAxis::DoDisplay(TDisplayMode mode)
{
	// Don't draw axing in select mode
	if (mode == dmSelect)
		return;

  glLineWidth(3.0f);
  glBegin(GL_LINES);
    // X axis red
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colorRed);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(size, 0.0, 0.0);

    // Y axis green
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colorGreen);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, size, 0.0);

    // Z axis blue
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colorBlue);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, size);
  glEnd();

  glPushMatrix();
    glTranslatef(size, 0.0, 0.0);
    glScalef(font_width, font_height, font_width);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colorRed);
    glRotatef(90, 1.0, 0.0, 0.0);
    glutStrokeString(GLUT_STROKE_MONO_ROMAN, (unsigned char *)"X");
  glPopMatrix();

  glPushMatrix();
    glTranslatef(0.0, size, 0.0);
    glScalef(font_width, font_height, font_width);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colorGreen);
    glRotatef(90, 1.0, 0.0, 0.0);
    glRotatef(90, 0.0, 1.0, 0.0);
    glutStrokeString(GLUT_STROKE_MONO_ROMAN, (unsigned char *)"Y");
  glPopMatrix();

  glPushMatrix();
    glTranslatef(0.0, 0.0, size);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colorBlue);
    glScalef(font_width, font_height, font_width);
    glRotatef(90, 1.0, 0.0, 0.0);
    glRotatef(90, 0.0, 1.0, 0.0);
    glutStrokeString(GLUT_STROKE_MONO_ROMAN, (unsigned char *)"Z");
  glPopMatrix();
}

// ********************************************************************************
// End of file
// ********************************************************************************
