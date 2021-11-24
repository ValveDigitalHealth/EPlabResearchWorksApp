//---------------------------------------------------------------------------
/*

MIT LICENSE Copyright (c) 2021 Pawel Kuklik

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the SOFTWARE.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. */
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#include <math.h>

#ifndef vector_3dH
#define vector_3dH
//---------------------------------------------------------------------------

class Vector3d
{
	public:

	Vector3d();

	Vector3d(double sx,double sy,double sz) { x = sx; y=sy; z=sz; }

	double x, y, z;

	// Constructors as well as getters/setters omitted for brevity!!
	// Only important methods kept necessary for this tutorial.
	// The original class contains many more methods...

	Vector3d add(Vector3d a) {
		x += a.x;
		y += a.y;
		z += a.z;

		return Vector3d(x,y,z);
	}

	//--------------------------------------------------------
	Vector3d set(Vector3d v)	{
		x = v.x;
		y = v.y;
		z = v.z;

		return Vector3d(x,y,z);
	}

	//--------------------------------------------------------
	Vector3d subAndAssign(Vector3d a, Vector3d b) {
/*
		x = a.x - b.x;
		y = a.y - b.y;
		z = a.z - b.z;
*/
		x = b.x - a.x;
		y = b.y - a.y;
		z = b.z - a.z;

		return Vector3d(x,y,z);
	}

	//--------------------------------------------------------
	/**
	 * Returns the length of the vector, also called L2-Norm or Euclidean Norm.
	 */
	double l2Norm() {
		return (double) sqrt(x*x+y*y+z*z);
	}

	//--------------------------------------------------------
	double dotproduct(Vector3d a, Vector3d b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	//--------------------------------------------------------
	Vector3d dotAndAssign(Vector3d a, Vector3d b) {
		double tempX = a.x * b.x;
		double tempY = a.y * b.y;
		double tempZ = a.z * b.z;

		x = tempX;
		y = tempY;
		z = tempZ;

		return Vector3d(x,y,z);
	}

	//--------------------------------------------------------
	Vector3d crossAndAssign(Vector3d a, Vector3d b) {
		double tempX = a.y * b.z - a.z * b.y;
		double tempY = a.z * b.x - a.x * b.z;
		double tempZ = a.x * b.y - a.y * b.x;

		x = tempX;
		y = tempY;
		z = tempZ;

		return Vector3d(x,y,z);
	}

	//--------------------------------------------------------
	Vector3d scale(double scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;

		return Vector3d(x,y,z);
	}

	//--------------------------------------------------------
	Vector3d normalize() {
		double length = l2Norm();
		x /= length;
		y /= length;
		z /= length;

		return Vector3d(x,y,z);
	}
};

#endif
