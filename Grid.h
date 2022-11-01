#ifndef _GRID_3D_H
#define _GRID_3D_H

#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>
#include "Vector3D.h"
#include "Vector4D.h"
#include "Object3D.h"

namespace GLScene
{

/**
 * TGrid class
 * Params: l length of the axe
 */
class TGrid: public TObject3D
{
	private:
		TVector3D size;
		TVector3D Center;
		int sizeX, sizeY, sizeZ;
		GLfloat CenterX, CenterY, CenterZ;
		GLfloat colorBlack[4] = { 0.0, 0.0, 0.0, 1.0 };
		GLfloat colorRed[4] = { 1.0, 0.0, 0.0, 1.0 };
		GLfloat colorGreen[4] = { 0.0, 1.0, 0.0, 1.0 };
		GLfloat colorBlue[4] = { 0.0, 0.0, 1.0, 1.0 };
		bool gridX;
		bool gridY;
		bool gridZ;
		int planZ;
		bool Node;
		bool Centered;
		int GridStep;

		void ComputeCenterGrid(void);

	protected:

	public:
		TGrid(TVector3D _size);
		virtual ~TGrid();

		void virtual DoDisplay(TDisplayMode mode = dmRender);

		void ShowGridX(bool show)
		{
			gridX = show;
		}
		void ShowGridY(bool show)
		{
			gridY = show;
		}
		void ShowGridZ(bool show)
		{
			gridZ = show;
		}
		/**
		 * Show only z plane number num
		 * num = -1 : show all planes
		 */
		void ShowPlanZ(int num)
		{
			planZ = num;
		}
		void ShowNode(bool node)
		{
			Node = node;
		}

		TVector3D GetCenter(void)
		{
			return Center;
		}

		void SetCenterGrid(bool center);

		void SetGridStep(int step);
};

} // namespace GLScene

//---------------------------------------------------------------------------
#endif // _GRID_3D_H
