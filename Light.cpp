/**
 * https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glLight.xml
 */
#include "Light.h"
#include <math.h>


TLight::TLight(GLenum _light, TLightStyle _style, TVector3D *pos) : TObject3D(pos)
{
  num_light = _light;
  SetStyle(_style);

  Changed = true;
  enabled = true;

  position = {50.0, 50.0, 100.0};

  ambient = {0.2, 0.2, 0.2, 1.0};
  diffuse = {0.8, 0.8, 0.8, 1.0};
  specular = {0.0, 0.0, 0.0, 1.0}; // Not brilliant

  model_ambient = {0.2, 0.2, 0.2, 1.0};
  local_view = {0.0, 0.0, 0.0, 0.0};
}

// Render light
void TLight::Render()
{
  if (!Changed || !enabled)
  {
    Changed = false;
    return;
  }

  Changed = false;

  // Position
  glLightfv(num_light, GL_POSITION, TVector4D(position, 0).Array());

  // Style
  glLightfv(num_light, GL_AMBIENT, ambient.Array());
  glLightfv(num_light, GL_DIFFUSE, diffuse.Array());
  glLightfv(num_light, GL_SPECULAR, specular.Array());

  glLightfv(num_light, GL_CONSTANT_ATTENUATION, &const_at);
  glLightfv(num_light, GL_LINEAR_ATTENUATION, &lin_at);
  glLightfv(num_light, GL_QUADRATIC_ATTENUATION, &quad_at);

  if (style == Spot)
  {
    glLightfv(num_light, GL_SPOT_DIRECTION, spot_direction.Array());
    glLightf(num_light, GL_SPOT_CUTOFF, spot_cutoff);
    glLightf(num_light, GL_SPOT_EXPONENT, spot_expo);
  }

  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient.Array());
  glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view.Array());

  if (enabled)
    glEnable(num_light);

  // Activation
  glEnable(GL_LIGHTING);
}

/**
 * The default color for GL_LIGHT0
 * Note the default position is {0.0, 0.0, 1.0, 0.0}. Light is directional, parallel to z axis
 */
void TLight::SetDefaultColor()
{
  ambient = {0.0, 0.0, 0.0, 1.0};
  diffuse = {1.0, 1.0, 1.0, 1.0};
  specular = {1.0, 1.0, 1.0, 1.0};
  model_ambient = {0.2, 0.2, 0.2, 1.0};
  local_view = {0.0, 0.0, 0.0, 0.0};   // only the first parameter is used
  Changed = true;
}

/**
 * Change the color of the light
 */
void TLight::SetLightColor(const TLightColor _light, const TVector4D _color)
{
  Changed = true;
  switch (_light)
  {
    case lcAmbient: ambient = _color; break;
    case lcDiffuse: diffuse = _color; break;
    case lcSpecular: specular = _color; break;
    case lcModelAmbient: model_ambient = _color; break;
    default : Changed = false;
  }
}

bool TLight::CheckIncrement(GLfloat x, GLfloat _inc)
{
  x += _inc;
  return ((x >= 0.0) && (x <= 1.0));
}

void TLight::SetLightColor(const TLightColor _light, const GLfloat _inc)
{
  Changed = true;
  switch (_light)
  {
    case lcAmbient: if (CheckIncrement(ambient.X, _inc)) ambient += _inc; break;
    case lcDiffuse: if (CheckIncrement(diffuse.X, _inc)) diffuse += _inc; break;
    case lcSpecular: if (CheckIncrement(specular.X, _inc)) specular += _inc; break;
    case lcModelAmbient: if (CheckIncrement(model_ambient.X, _inc)) model_ambient += _inc; break;
    default : Changed = false;
  }
}

/**
 * Change the type of the light
 */
void TLight::SetStyle(TLightStyle _style)
{
  style = _style;

  const_at = 1;
  lin_at = 0;
  quad_at = 0;

  if (style == Spot)
  {
    lin_at = 0.4;
    quad_at = 0.1;
    direction = {-1.0, -1.0, 0.0};

    spot_cutoff = CUTOFF;
    spot_expo = EXPO;
  }

  Changed = true;
};

void TLight::Disable()
{
  Changed = true;
  enabled = false;
  glDisable(num_light);
}

void TLight::Enable()
{
  Changed = true;
  enabled = true;
  glEnable(num_light);
}

void TLight::DoDisplay(TDisplayMode mode)
{
	if (mode == dmSelect)
	{
		// nothing to display
	}
}

// ********************************************************************************
// End of file
// ********************************************************************************
