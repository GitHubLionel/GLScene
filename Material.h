#ifndef _MATERIAL_3D_H
#define _MATERIAL_3D_H

#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>
#include "FaceProperties.h"
#include "Vector4D.h"

namespace GLScene
{
	
	

/**
 * TMaterial class
 * Manage material properties
 */
class TMaterial
{
  private:
		TGLFaceProperties BackProperties;
		TGLFaceProperties FrontProperties;
  protected:

  public:
    TMaterial();
    virtual ~TMaterial();

    void ApplyMaterial(void);

		TVector4D GetBackProperties(TMaterialSource source);
		TVector4D GetFrontProperties(TMaterialSource source);

		virtual void SetDefaultColor();
		void SetColor(const TMaterialFace _face, const TVector4D _ambiant_diffuse, GLfloat _alpha = 1.0);
		void SetColor(const TMaterialFace _face, const TMaterialSource _mat, const TVector4D _color);
		void SetColor(const TMaterialFace _face, const TMaterialSource _mat, const GLfloat _inc);

		void SetAlpha(const TMaterialFace _face, const TMaterialSource _mat, const GLfloat _alpha);

};

} // namespace GLScene

//---------------------------------------------------------------------------
#endif // _MATERIAL_3D_H
