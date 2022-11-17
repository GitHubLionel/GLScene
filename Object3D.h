#ifndef _OBJECT_3D_H
#define _OBJECT_3D_H

#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>
#include "Vector3D.h"
#include "Vector4D.h"
#include "Material.h"
#include <initializer_list> // for std::initializer_list
#include "GLColor.h"

namespace GLScene
{

typedef enum {
	otAxis, otCube, otCuboid, otCylinder, otSphere, otEllipsoid, otArrow, otGrid, otNone
} TObjectType;

typedef enum {
	dmRender, dmSelect
} TDisplayMode;

/**
 * TObject3D class
 * A base class for all the object
 * Params: position (default (0,0,0))
 * Params: rotation (default (0,0,0))
 * Params: scale (default (1.0,1.0,1.0))
 * By default, none of the position, rotation and scale are doing
 */
class TObject3D {
	private:
		void defaultSettings();
		void Draw();

	protected:
		TVector3D z_direction = TVector3D(0.0, 0.0, 1.0);

		bool Lighted;
		bool Changed;

	protected:
		TObjectType ObjectType;

		// Manipulation
		TVector3D position;
		TVector3D direction;
		TVector3D rotation;
		TVector3D scale;
		bool DoPosition = false;
		bool DoRotation = false;
		bool DoScale = false;

		// Material
		TMaterial material = TMaterial();
//		TVector4D ambient;
//		TVector4D diffuse;
//		TVector4D specular;
//		TVector4D emission;
//		TVector4D shininess;
		bool DoUseMaterial = true;

		float levelOfDetail;
		bool selected;

	public:
		TObject3D(TVector3D *pos = NULL, TVector3D *rot = NULL, TVector3D *sca = NULL);
		TObject3D(const TVector3D &pos);
		virtual ~TObject3D();

		/// A tag for the user
		int Tag;
		/// Auto free when object is removed from the list
		bool AutoFree;
		/// Visibility of the object
		bool Visible;
		/// A pointer that can be used by user to attach what he want
		void *UserPointer;

		void Display(int id, TDisplayMode mode = dmRender);
		void virtual DoDisplay(TDisplayMode mode = dmRender) = 0;

		void Select()
		{
			selected = true;
		}
		void Deselect()
		{
			selected = false;
		}

		// setters
		void virtual SetPosition(const TVector3D *pos)
		{
			position = *pos;
			Changed = true;
		}
		void virtual SetPosition(GLfloat pos[])
		{
			position = pos;
			Changed = true;
		}
		void virtual SetPosition(std::initializer_list<GLfloat> list);
		void virtual SetPosition(GLfloat x, GLfloat y, GLfloat z)
		{
			position = { x, y, z };
			Changed = true;
		}

		void virtual SetDirection(const TVector3D *pos)
		{
			direction = *pos;
			Changed = true;
		}
		void virtual SetDirection(GLfloat pos[])
		{
			direction = pos;
			Changed = true;
		}
		void virtual SetDirection(std::initializer_list<GLfloat> list);
		void virtual SetDirection(GLfloat x, GLfloat y, GLfloat z)
		{
			direction = { x, y, z };
			Changed = true;
		}

		void SetRotation(TVector3D *rot);
		void SetScale(TVector3D *sca);
		void SetLevelOfDetail(float);

		TMaterial &GetMaterial(void)
		{
			return material;
		}

		void ChangeFrontEmission(GLfloat val);

		// getters
		inline TObjectType GetObjectType()
		{
			return ObjectType;
		}

		inline TVector3D GetPosition()
		{
			return position;
		}
		inline TVector3D GetDirection()
		{
			return direction;
		}
		inline TVector3D GetRotation()
		{
			return rotation;
		}
		inline TVector3D GetScale()
		{
			return scale;
		}

		inline float GetLevelOfDetail()
		{
			return levelOfDetail;
		}

		void PrepareRotate(GLfloat len);
};

// Random functions
GLfloat GLRand(void);
GLfloat GLRand(GLfloat max);

} // namespace GLScene

//---------------------------------------------------------------------------
#endif // _OBJECT_3D_H
