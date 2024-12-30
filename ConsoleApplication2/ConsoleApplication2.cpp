#include "stdafx.h"
#include <Windows.h>
#include <math.h>
#include <iostream>
#include <glut.h>
#include <vector>

using namespace std;
#define PI 3.14159265358979323846
#define	COLORNUM 14
#define		DEG2RAD(x) (x*PI)/180.0f

class Point3
{
public:
	float x, y, z;
	void set(float dx, float dy, float dz)
	{
		x = dx; y = dy; z = dz;
	}
	void set(Point3& p)
	{
		x = p.x; y = p.y; z = p.z;
	}
	Point3() { x = y = z = 0; }
	Point3(float dx, float dy, float dz)
	{
		x = dx; y = dy; z = dz;
	}

};
class Color3
{
public:
	float r, g, b;
	void set(float red, float green, float blue)
	{
		r = red; g = green; b = blue;
	}
	void set(Color3& c)
	{
		r = c.r; g = c.g; b = c.b;
	}
	Color3() { r = g = b = 0; }
	Color3(float red, float green, float blue)
	{
		r = red; g = green; b = blue;
	}

};

class Vector3
{
public:
	float	x, y, z;
	void set(float dx, float dy, float dz)
	{
		x = dx; y = dy; z = dz;
	}
	void set(Vector3& v)
	{
		x = v.x; y = v.y; z = v.z;
	}
	void flip()
	{
		x = -x; y = -y; z = -z;
	}
	void normalize();
	Vector3() { x = y = z = 0; }
	Vector3(float dx, float dy, float dz)
	{
		x = dx; y = dy; z = dz;
	}
	Vector3(Vector3& v)
	{
		x = v.x; y = v.y; z = v.z;
	}
	Vector3 cross(Vector3 b);
	float dot(Vector3 b);
};

Vector3 Vector3::cross(Vector3 b)
{
	Vector3 c(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x);
	return c;
}
float Vector3::dot(Vector3 b)
{
	return x*b.x + y*b.y + z*b.z;
}

void Vector3::normalize()
{
	float temp = sqrt(x*x + y*y + z*z);
	x = x / temp;
	y = y / temp;
	z = z / temp;
}

class VertexID
{
public:
	int	vertIndex;
	int	colorIndex;
};

class Face
{
public:
	int	nVerts;
	VertexID* vert;
	Vector3 norm;
	bool flag = false;
	Face()
	{
		nVerts = 0;
		vert = NULL;
		flag = false;
	}
	~Face()
	{
		if (vert != NULL)
		{
			delete[] vert;
			vert = NULL;
		}
		nVerts = 0;
	}
};

class Mesh
{
public:
	int	numVerts;
	Point3*	pt;

	int	numFaces;
	Face* face;
public:
	Mesh()
	{
		numVerts = 0;
		pt = NULL;
		numFaces = 0;
		face = NULL;
	}
	~Mesh()
	{
		if (pt != NULL)
		{
			delete[] pt;
		}
		if (face != NULL)
		{
			delete[] face;
		}
		numVerts = 0;
		numFaces = 0;
	}
	void reset() {
		if (pt != NULL)
		{
			delete[] pt;
		}
		if (face != NULL)
		{
			delete[] face;
		}
		numVerts = 0;
		numFaces = 0;
	}
	void DrawWireframe(float x, float y, float z);
	void DrawColor(int color);
	void calculateNorm();
	void DrawNorm(bool flag, float ambient_default[4], float diffuse_default[4], float specular_default[4], float shine_default, float ambient[4], float diffuse[4], float specular[4], float shine);
};

class Cuboid : public Mesh {
private:
	float fsizeX;
	float fsizeY;
	float fsizeZ;
public:
	Cuboid(float x, float y, float z) {
		this->fsizeX = x;
		this->fsizeY = y;
		this->fsizeZ = z;
	}

	float getX() { return this->fsizeX; }
	float getY() { return this->fsizeY; }
	float getZ() { return this->fsizeZ; }
	void createShape();
};

class Shape1 : public Mesh {
private:
	float x, y, z;
	float length, width, Length, Width, height1, height2, height3;
public:
	Shape1(float x = 0, float y = 0, float z = 0,
		float length = 0, float width = 0,
		float Length = 0, float Width = 0,
		float height1 = 0, float height2 = 0, float height3 = 0) {
		this->x = x;
		this->y = y;
		this->z = z;

		this->length = length;
		this->width = width;
		this->Length = Length;
		this->Width = Width;

		this->height1 = height1;
		this->height2 = height2;
		this->height3 = height3;
	}
	void creatShape();
};

class Shape3 : public Mesh {
private:
	float width, y;
	float y1, y2;
	float z1, z2;
public:
	Shape3(float width, float y, float y1, float y2, float z1, float z2) {
		this->width = width;
		this->y = y;
		this->y1 = y1;
		this->y2 = y2;
		this->z1 = z1;
		this->z2 = z2;
	}
	void creatShape() {
		numVerts = 16;
		pt = new Point3[numVerts];

		pt[0].set(width / 2, y + y1, z1 / 2);
		pt[1].set(-width / 2, y + y1, z1 / 2);
		pt[2].set(width / 2, y + y1, -z1 / 2);
		pt[3].set(-width / 2, y + y1, -z1 / 2);

		pt[4].set(width / 2, y + y2, z2 / 2);
		pt[5].set(-width / 2, y + y2, z2 / 2);
		pt[6].set(width / 2, y + y2, -z2 / 2);
		pt[7].set(-width / 2, y + y2, -z2 / 2);

		pt[8].set(width / 2, y - y1, z1 / 2);
		pt[9].set(-width / 2, y - y1, z1 / 2);
		pt[10].set(width / 2, y - y1, -z1 / 2); 
		pt[11].set(-width / 2, y - y1, -z1 / 2);

		pt[12].set(width / 2, y - y2, z2 / 2);
		pt[13].set(-width / 2, y - y2, z2 / 2);
		pt[14].set(width / 2, y - y2, -z2 / 2);
		pt[15].set(-width / 2, y - y2, -z2 / 2);

		numFaces = 12;
		face = new Face[numFaces];

		face[0].nVerts = 4;
		face[0].vert = new VertexID[4];
		face[0].vert[0].vertIndex = 0;
		face[0].vert[1].vertIndex = 1;
		face[0].vert[2].vertIndex = 3;
		face[0].vert[3].vertIndex = 2;

		face[1].nVerts = 4;
		face[1].vert = new VertexID[4];
		face[1].vert[0].vertIndex = 0;
		face[1].vert[1].vertIndex = 2;
		face[1].vert[2].vertIndex = 10;
		face[1].vert[3].vertIndex = 8;

		face[2].nVerts = 4;
		face[2].vert = new VertexID[4];
		face[2].vert[0].vertIndex = 1;
		face[2].vert[1].vertIndex = 3;
		face[2].vert[2].vertIndex = 11;
		face[2].vert[3].vertIndex = 9;

		face[3].nVerts = 4;
		face[3].vert = new VertexID[4];
		face[3].vert[0].vertIndex = 8;
		face[3].vert[1].vertIndex = 9;
		face[3].vert[2].vertIndex = 11;
		face[3].vert[3].vertIndex = 10;

		//duong
		face[4].nVerts = 4;
		face[4].vert = new VertexID[4];
		face[4].vert[0].vertIndex = 0;
		face[4].vert[1].vertIndex = 1;
		face[4].vert[2].vertIndex = 5;
		face[4].vert[3].vertIndex = 4;

		face[5].nVerts = 4;
		face[5].vert = new VertexID[4];
		face[5].vert[0].vertIndex = 8;
		face[5].vert[1].vertIndex = 9;
		face[5].vert[2].vertIndex = 13;
		face[5].vert[3].vertIndex = 12;

		face[6].nVerts = 4;
		face[6].vert = new VertexID[4];
		face[6].vert[0].vertIndex = 0;
		face[6].vert[1].vertIndex = 4;
		face[6].vert[2].vertIndex = 12;
		face[6].vert[3].vertIndex = 8;

		face[7].nVerts = 4;
		face[7].vert = new VertexID[4];
		face[7].vert[0].vertIndex = 1;
		face[7].vert[1].vertIndex = 5;
		face[7].vert[2].vertIndex = 13;
		face[7].vert[3].vertIndex = 9;

		//am
		face[8].nVerts = 4;
		face[8].vert = new VertexID[4];
		face[8].vert[0].vertIndex = 2;
		face[8].vert[1].vertIndex = 3;
		face[8].vert[2].vertIndex = 7;
		face[8].vert[3].vertIndex = 6;

		face[9].nVerts = 4;
		face[9].vert = new VertexID[4];
		face[9].vert[0].vertIndex = 10;
		face[9].vert[1].vertIndex = 11;
		face[9].vert[2].vertIndex = 15;
		face[9].vert[3].vertIndex = 14;

		face[10].nVerts = 4;
		face[10].vert = new VertexID[4];
		face[10].vert[0].vertIndex = 2;
		face[10].vert[1].vertIndex = 6;
		face[10].vert[2].vertIndex = 14;
		face[10].vert[3].vertIndex = 10;

		face[11].nVerts = 4;
		face[11].vert = new VertexID[4];
		face[11].vert[0].vertIndex = 3;
		face[11].vert[1].vertIndex = 7;
		face[11].vert[2].vertIndex = 15;
		face[11].vert[3].vertIndex = 11;

		calculateNorm();
	}
};
class Shape4 : public Mesh {
private:
	float length, Radius, x, y, z;
	int numVerts;

public:
	Shape4(float length, float Radius, float x, float y, float z) {
		this->length = length;
		this->Radius = Radius;
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void creatShape() {
		numVerts = 2 * 20 + 2;
		pt = new Point3[numVerts];

		float angleStep = 2 * PI / 20;

		pt[0] = Point3(x + length / 2, y, z);
		pt[1] = Point3(x - length / 2, y, z);

		for (int i = 0; i < 20; ++i) {
			float angle = i * angleStep;
			pt[i + 2] = Point3(x + length / 2, y + Radius * cos(angle), z + Radius * sin(angle));
		}

		for (int i = 0; i < 20; ++i) {
			float angle = i * angleStep;
			pt[i + 22] = Point3(x - length / 2, y + Radius * cos(angle), z + Radius * sin(angle));
		}

		numFaces = 20 * 3;
		face = new Face[numFaces];

		for (int i = 0; i < 20; i++) {
			face[i].nVerts = 3;
			face[i].vert = new VertexID[3];
			face[i].vert[0].vertIndex = 0;
			face[i].vert[1].vertIndex = i + 2;
			face[i].vert[2].vertIndex = (i + 1) % 20 + 2;
		}

		for (int i = 20; i < 40; i++) {
			face[i].nVerts = 3;
			face[i].vert = new VertexID[3];
			face[i].vert[0].vertIndex = 1;
			face[i].vert[1].vertIndex = i + 2;
			face[i].vert[2].vertIndex = (i + 1) % 20 + 22;
		}

		int faceIndex = 40;
		for (int i = 0; i < 20; i++) {
			face[faceIndex].nVerts = 4;
			face[faceIndex].vert = new VertexID[4];

			face[faceIndex].vert[0].vertIndex = i + 2;
			face[faceIndex].vert[1].vertIndex = (i + 1) % 20 + 2;
			face[faceIndex].vert[2].vertIndex = (i + 1) % 20 + 22;
			face[faceIndex].vert[3].vertIndex = i + 22;

			faceIndex++;
		}

		calculateNorm();
	}
};
class Shape5 : public Mesh {
private:
	float x, y, z;
	float dx, dy, dz;
public:
	Shape5(float x, float y, float z, float dx, float dy, float dz) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->dx = dx;
		this->dy = dy;
		this->dz = dz;
	}

	void creatShape() {
		numVerts = 8;
		pt = new Point3[numVerts];

		pt[0] = Point3(x + dx / 2, y + dy / 2, z + dz / 2);
		pt[1] = Point3(x - dx / 2, y + dy / 2, z + dz / 2);
		pt[2] = Point3(x - dx / 2, y - dy / 2, z + dz / 2);
		pt[3] = Point3(x + dx / 2, y - dy / 2, z + dz / 2);
		pt[4] = Point3(x + dx / 2, y + dy / 2, z - dz / 2);
		pt[5] = Point3(x - dx / 2, y + dy / 2, z - dz / 2);
		pt[6] = Point3(x - dx / 2, y - dy / 2, z - dz / 2);
		pt[7] = Point3(x + dx / 2, y - dy / 2, z - dz / 2);

		numFaces = 6;
		face = new Face[numFaces];

		face[0].nVerts = 4;
		face[0].vert = new VertexID[4];
		face[0].vert[0].vertIndex = 0;
		face[0].vert[1].vertIndex = 1;
		face[0].vert[2].vertIndex = 2;
		face[0].vert[3].vertIndex = 3;

		face[1].nVerts = 4;
		face[1].vert = new VertexID[4];
		face[1].vert[0].vertIndex = 4;
		face[1].vert[1].vertIndex = 5;
		face[1].vert[2].vertIndex = 6;
		face[1].vert[3].vertIndex = 7;

		face[2].nVerts = 4;
		face[2].vert = new VertexID[4];
		face[2].vert[0].vertIndex = 0;
		face[2].vert[1].vertIndex = 1;
		face[2].vert[2].vertIndex = 5;
		face[2].vert[3].vertIndex = 4;

		face[3].nVerts = 4;
		face[3].vert = new VertexID[4];
		face[3].vert[0].vertIndex = 3;
		face[3].vert[1].vertIndex = 2;
		face[3].vert[2].vertIndex = 6;
		face[3].vert[3].vertIndex = 7;

		face[4].nVerts = 4;
		face[4].vert = new VertexID[4];
		face[4].vert[0].vertIndex = 1;
		face[4].vert[1].vertIndex = 2;
		face[4].vert[2].vertIndex = 6;
		face[4].vert[3].vertIndex = 5;

		face[5].nVerts = 4;
		face[5].vert = new VertexID[4];
		face[5].vert[0].vertIndex = 0;
		face[5].vert[1].vertIndex = 3;
		face[5].vert[2].vertIndex = 7;
		face[5].vert[3].vertIndex = 4;
		calculateNorm();
	}
};
class Shape6 : public Mesh {
private:
	float length, Radius, x, y, z;
	int numVerts;

public:
	Shape6(float length, float Radius, float x, float y, float z) {
		this->length = length;
		this->Radius = Radius;
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void creatShape() {
		numVerts = 2 * 40 + 2;
		pt = new Point3[numVerts];

		float angleStep = 2 * PI / 40;
		pt[0] = Point3(x, y, z + length / 2);
		pt[1] = Point3(x, y, z - length / 2);

		for (int i = 0; i < 40; ++i) {
			float angle = i * angleStep;
			pt[i + 2] = Point3(x + Radius * cos(angle), y + Radius * sin(angle), z + length / 2);
		}
		for (int i = 0; i < 40; ++i) {
			float angle = i * angleStep;
			pt[i + 42] = Point3(x + Radius * cos(angle), y + Radius * sin(angle), z - length / 2);
		}

		numFaces = 40 * 3 + 20 * 3 + 20 * 4;
		face = new Face[numFaces];

		for (int i = 0; i < 40; i++) {
			face[i].nVerts = 3;
			face[i].vert = new VertexID[3];
			face[i].vert[0].vertIndex = 0;
			face[i].vert[1].vertIndex = i + 2;
			face[i].vert[2].vertIndex = (i + 1) % 40 + 2;
		}

		for (int i = 40; i < 80; i++) {
			face[i].nVerts = 3;
			face[i].vert = new VertexID[3];
			face[i].vert[0].vertIndex = 1;
			face[i].vert[1].vertIndex = i + 2;
			face[i].vert[2].vertIndex = (i + 1) % 40 + 42;
		}

		int faceIndex = 80;
		for (int i = 0; i < 40; i++) {
			face[faceIndex].nVerts = 4;
			face[faceIndex].vert = new VertexID[4];

			face[faceIndex].vert[0].vertIndex = i + 2;
			face[faceIndex].vert[1].vertIndex = (i + 1) % 40 + 2;
			face[faceIndex].vert[2].vertIndex = (i + 1) % 40 + 42;
			face[faceIndex].vert[3].vertIndex = i + 42;

			faceIndex++;
		}

		calculateNorm();
	}
};
class Wheel : public Mesh {
private:
	float x, y, z;
	float R1, R2, R3, R4;
	float fsize;
public:
	Wheel(float x, float y, float z, float R1, float R2, float R3, float R4, float fsize) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->R1 = R1;
		this->R2 = R2;
		this->R3 = R3;
		this->R4 = R4;
		this->fsize = fsize;
	}
	void creatShape() {
		numVerts = 40 * 4 + 80 * 4 + 40;
		pt = new Point3[numVerts];

		float angle12 = 2 * PI / 40;
		float angle34 = 2 * PI / 80;

		for (int i = 0; i < 40; ++i) {
			float angle = i * angle12;
			pt[i] = Point3(x + fsize / 2, y + R1 * cos(angle), z + R1 * sin(angle));
		}
		for (int i = 40; i < 80; ++i) {
			float angle = i * angle12;
			pt[i] = Point3(x - fsize / 2, y + R1 * cos(angle), z + R1 * sin(angle));
		}
		for (int i = 80; i < 120; ++i) {
			float angle = i * angle12;
			pt[i] = Point3(x + fsize / 2, y + R2 * cos(angle), z + R2 * sin(angle));
		}
		for (int i = 120; i < 160; ++i) {
			float angle = i * angle12;
			pt[i] = Point3(x - fsize / 2, y + R2 * cos(angle), z + R2 * sin(angle));
		}
		for (int i = 160; i < 240; ++i) {
			float angle = i * angle34;
			pt[i] = Point3(x + fsize / 2, y + R3 * cos(angle), z + R3 * sin(angle));
		}
		for (int i = 240; i < 320; ++i) {
			float angle = i * angle34;
			pt[i] = Point3(x - fsize / 2, y + R3 * cos(angle), z + R3 * sin(angle));
		}
		for (int i = 320; i < 400; ++i) {
			float angle = i * angle34;
			pt[i] = Point3(x + fsize / 2, y + R4 * cos(angle), z + R4 * sin(angle));
		}
		for (int i = 400; i < 480; ++i) {
			float angle = i * angle34;
			pt[i] = Point3(x - fsize / 2, y + R4 * cos(angle), z + R4 * sin(angle));
		}
		int ptIndex = 480;
		for (int i = 0; i < 5; ++i) {
			float angle = i * 2 * PI / 5;

			for (int j = 0; j < 2; ++j) {
				float xSmall = x + (j == 0 ? fsize / 2 : -fsize / 2);
				float ySmall = y + R2 * cos(angle - fsize / 1.5 / R2);
				float zSmall = z + R2 * sin(angle - fsize / 1.5 / R2);

				float xsmall = x + (j == 0 ? fsize / 2 : -fsize / 2);
				float ysmall = y + R2 * cos(angle + fsize / 1.5 / R2);
				float zsmall = z + R2 * sin(angle + fsize / 1.5 / R2);

				float xLarge = x + (j == 0 ? fsize / 2 : -fsize / 2);
				float yLarge = y + (R3 + 0.02) * cos(angle - fsize / 1.5 / R3);
				float zLarge = z + (R3 + 0.02) * sin(angle - fsize / 1.5 / R3);

				float xlarge = x + (j == 0 ? fsize / 2 : -fsize / 2);
				float ylarge = y + (R3 + 0.02)* cos(angle + fsize / 1.5 / R3);
				float zlarge = z + (R3 + 0.02) * sin(angle + fsize / 1.5 / R3);

				pt[ptIndex++] = Point3(xSmall, ySmall, zSmall); // Điểm dưới của cây nối
				pt[ptIndex++] = Point3(xsmall, ysmall, zsmall);
				pt[ptIndex++] = Point3(xLarge, yLarge, zLarge); // Điểm trên của cây nối
				pt[ptIndex++] = Point3(xlarge, ylarge, zlarge);
			}
		}
		numFaces = 40 * 4 + 80 * 4 + 4 * 5;
		face = new Face[numFaces];
		int faceIndex = 0;
		for (int i = 0; i < 40; i++) {
			face[faceIndex].nVerts = 4;
			face[faceIndex].vert = new VertexID[4];
			face[faceIndex].vert[0].vertIndex = i;
			face[faceIndex].vert[1].vertIndex = (i + 1) % 40;
			face[faceIndex].vert[2].vertIndex = (i + 1) % 40 + 40;
			face[faceIndex].vert[3].vertIndex = i + 40;
			faceIndex++;
		}
		for (int i = 0; i < 40; i++) {
			face[faceIndex].nVerts = 4;
			face[faceIndex].vert = new VertexID[4];
			face[faceIndex].vert[0].vertIndex = i + 80;
			face[faceIndex].vert[1].vertIndex = (i + 1) % 40 + 80;
			face[faceIndex].vert[2].vertIndex = (i + 1) % 40 + 120;
			face[faceIndex].vert[3].vertIndex = i + 120;
			faceIndex++;
		}
		for (int i = 0; i < 80; i++) {
			face[faceIndex].nVerts = 4;
			face[faceIndex].vert = new VertexID[4];
			face[faceIndex].vert[0].vertIndex = i + 160;
			face[faceIndex].vert[1].vertIndex = (i + 1) % 80 + 160;
			face[faceIndex].vert[2].vertIndex = (i + 1) % 80 + 240;
			face[faceIndex].vert[3].vertIndex = i + 240;
			faceIndex++;
		}
		for (int i = 0; i < 80; i++) {
			face[faceIndex].nVerts = 4;
			face[faceIndex].vert = new VertexID[4];
			face[faceIndex].vert[0].vertIndex = i + 320;
			face[faceIndex].vert[1].vertIndex = (i + 1) % 80 + 320;
			face[faceIndex].vert[2].vertIndex = (i + 1) % 80 + 400;
			face[faceIndex].vert[3].vertIndex = i + 400;
			faceIndex++;
		}

		for (int i = 0; i < 40; i++) {
			face[faceIndex].nVerts = 4;
			face[faceIndex].vert = new VertexID[4];
			face[faceIndex].vert[0].vertIndex = i;
			face[faceIndex].vert[1].vertIndex = (i + 1) % 40;
			face[faceIndex].vert[2].vertIndex = (i + 1) % 40 + 80;
			face[faceIndex].vert[3].vertIndex = i + 80;
			faceIndex++;
		}
		for (int i = 0; i < 40; i++) {
			face[faceIndex].nVerts = 4;
			face[faceIndex].vert = new VertexID[4];
			face[faceIndex].vert[0].vertIndex = i + 40;
			face[faceIndex].vert[1].vertIndex = (i + 1) % 40 + 40;
			face[faceIndex].vert[2].vertIndex = (i + 1) % 40 + 120;
			face[faceIndex].vert[3].vertIndex = i + 120;
			faceIndex++;
		}
		for (int i = 0; i < 80; i++) {
			face[faceIndex].nVerts = 4;
			face[faceIndex].vert = new VertexID[4];
			face[faceIndex].vert[0].vertIndex = i + 160;
			face[faceIndex].vert[1].vertIndex = (i + 1) % 80 + 160;
			face[faceIndex].vert[2].vertIndex = (i + 1) % 80 + 320;
			face[faceIndex].vert[3].vertIndex = i + 320;
			faceIndex++;
		}for (int i = 0; i < 80; i++) {
			face[faceIndex].nVerts = 4;
			face[faceIndex].vert = new VertexID[4];
			face[faceIndex].vert[0].vertIndex = i + 240;
			face[faceIndex].vert[1].vertIndex = (i + 1) % 80 + 240;
			face[faceIndex].vert[2].vertIndex = (i + 1) % 80 + 400;
			face[faceIndex].vert[3].vertIndex = i + 400;
			faceIndex++;
		}

		for (int i = 0; i < 40; i += 4) {
			face[faceIndex].nVerts = 4;
			face[faceIndex].vert = new VertexID[4];
			face[faceIndex].vert[0].vertIndex = i + 480;
			face[faceIndex].vert[1].vertIndex = i + 481;
			face[faceIndex].vert[2].vertIndex = i + 483;
			face[faceIndex].vert[3].vertIndex = i + 482;
			faceIndex++;
		}
		for (int i = 0; i < 40; i += 8) {
			face[faceIndex].nVerts = 4;
			face[faceIndex].vert = new VertexID[4];
			face[faceIndex].vert[0].vertIndex = i + 480;
			face[faceIndex].vert[1].vertIndex = i + 482;
			face[faceIndex].vert[2].vertIndex = i + 486;
			face[faceIndex].vert[3].vertIndex = i + 484;
			faceIndex++;
			face[faceIndex].nVerts = 4;
			face[faceIndex].vert = new VertexID[4];
			face[faceIndex].vert[0].vertIndex = i + 481;
			face[faceIndex].vert[1].vertIndex = i + 483;
			face[faceIndex].vert[2].vertIndex = i + 487;
			face[faceIndex].vert[3].vertIndex = i + 485;
			faceIndex++;
		}
		calculateNorm();
	}
};
class Slider : public Mesh {
private:
	float x, y, z;
	float dx, dy1, dy2, dz1, dz2;
public:
	Slider(float x, float y, float z, float dx, float dy1, float dy2, float dz1, float dz2) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->dx = dx;
		this->dy1 = dy1;
		this->dy2 = dy2;
		this->dz1 = dz1;
		this->dz2 = dz2;
	}
	void creatShape() {
		numVerts = 16;
		pt = new Point3[numVerts];
		pt[0] = Point3(x + dx / 2, y + dy1 / 2, z + dz1 / 2);
		pt[1] = Point3(x + dx / 2, y + dy1 / 2, z - dz1 / 2);
		pt[2] = Point3(x + dx / 2, y - dy1 / 2, z + dz1 / 2);
		pt[3] = Point3(x + dx / 2, y - dy1 / 2, z - dz1 / 2);
		pt[4] = Point3(x + dx / 2, y + dy2 / 2, z + dz2 / 2);
		pt[5] = Point3(x + dx / 2, y + dy2 / 2, z - dz2 / 2);
		pt[6] = Point3(x + dx / 2, y - dy2 / 2, z + dz2 / 2);
		pt[7] = Point3(x + dx / 2, y - dy2 / 2, z - dz2 / 2);
		pt[8] = Point3(x - dx / 2, y + dy1 / 2, z + dz1 / 2);
		pt[9] = Point3(x - dx / 2, y + dy1 / 2, z - dz1 / 2);
		pt[10] = Point3(x - dx / 2, y - dy1 / 2, z + dz1 / 2);
		pt[11] = Point3(x - dx / 2, y - dy1 / 2, z - dz1 / 2);
		pt[12] = Point3(x - dx / 2, y + dy2 / 2, z + dz2 / 2);
		pt[13] = Point3(x - dx / 2, y + dy2 / 2, z - dz2 / 2);
		pt[14] = Point3(x - dx / 2, y - dy2 / 2, z + dz2 / 2);
		pt[15] = Point3(x - dx / 2, y - dy2 / 2, z - dz2 / 2);

		numFaces = 16;
		face = new Face[numFaces];
		int numIndex = 0;
		for (int i = 0; i < 4; i += 2) {
			face[numIndex].nVerts = 4;
			face[numIndex].vert = new VertexID[4];
			face[numIndex].vert[0].vertIndex = i;
			face[numIndex].vert[1].vertIndex = i + 1;
			face[numIndex].vert[2].vertIndex = i + 5;
			face[numIndex].vert[3].vertIndex = i + 4;
			numIndex++;
		}
		for (int i = 0; i < 2; i++) {
			face[numIndex].nVerts = 4;
			face[numIndex].vert = new VertexID[4];
			face[numIndex].vert[0].vertIndex = i;
			face[numIndex].vert[1].vertIndex = i + 2;
			face[numIndex].vert[2].vertIndex = i + 6;
			face[numIndex].vert[3].vertIndex = i + 4;
			numIndex++;
		}
		for (int i = 0; i < 4; i += 2) {
			face[numIndex].nVerts = 4;
			face[numIndex].vert = new VertexID[4];
			face[numIndex].vert[0].vertIndex = i + 8;
			face[numIndex].vert[1].vertIndex = i + 9;
			face[numIndex].vert[2].vertIndex = i + 13;
			face[numIndex].vert[3].vertIndex = i + 12;
			numIndex++;
		}
		for (int i = 0; i < 2; i++) {
			face[numIndex].nVerts = 4;
			face[numIndex].vert = new VertexID[4];
			face[numIndex].vert[0].vertIndex = i + 8;
			face[numIndex].vert[1].vertIndex = i + 10;
			face[numIndex].vert[2].vertIndex = i + 14;
			face[numIndex].vert[3].vertIndex = i + 12;
			numIndex++;
		}
		for (int i = 0; i < 4; i += 2) {
			face[numIndex].nVerts = 4;
			face[numIndex].vert = new VertexID[4];
			face[numIndex].vert[0].vertIndex = i;
			face[numIndex].vert[1].vertIndex = i + 1;
			face[numIndex].vert[2].vertIndex = i + 9;
			face[numIndex].vert[3].vertIndex = i + 8;
			numIndex++;
		}
		for (int i = 0; i < 2; i++) {
			face[numIndex].nVerts = 4;
			face[numIndex].vert = new VertexID[4];
			face[numIndex].vert[0].vertIndex = i;
			face[numIndex].vert[1].vertIndex = i + 2;
			face[numIndex].vert[2].vertIndex = i + 10;
			face[numIndex].vert[3].vertIndex = i + 8;
			numIndex++;
		}
		for (int i = 0; i < 4; i += 2) {
			face[numIndex].nVerts = 4;
			face[numIndex].vert = new VertexID[4];
			face[numIndex].vert[0].vertIndex = i + 4;
			face[numIndex].vert[1].vertIndex = i + 5;
			face[numIndex].vert[2].vertIndex = i + 13;
			face[numIndex].vert[3].vertIndex = i + 12;
			numIndex++;
		}
		for (int i = 0; i < 2; i++) {
			face[numIndex].nVerts = 4;
			face[numIndex].vert = new VertexID[4];
			face[numIndex].vert[0].vertIndex = i + 4;
			face[numIndex].vert[1].vertIndex = i + 6;
			face[numIndex].vert[2].vertIndex = i + 14;
			face[numIndex].vert[3].vertIndex = i + 12;
			numIndex++;
		}
		calculateNorm();
	}
};
void drawRectangle(float x, float y, float z, float dx, float dz, float radius) {
	glBegin(GL_LINE_STRIP);
	for (float i = dx; i < dz; i += 0.01f) {
		float x1 = x + radius * cos(i);
		float z1 = z + radius * sin(i);
		glVertex3f(x1, y, z1);
	}
	glEnd();
}
void FLoorTiles(float x, float y,float z,float dx,float dz) {

	float angle = 2 * PI / 16;
	for (int i = 0; i < 16; i++) {
		glBegin(GL_POLYGON);
		glColor3f(0.6f, 0.3f, 0.0f);
		glVertex3f(x, y, z);
		glVertex3f(x + dx / 4 * sin(angle*i), y, z + dz / 4 * cos(angle*i));
		i++;
		glVertex3f(x + dx / 5 * sin(angle*i), y, z + dz / 5 * cos(angle*i));
		glEnd();
	}

	glColor3f(0.6f, 0.3f, 0.0f);
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glVertex3f(x - dx / 2, y, z - dz / 2); // Đỉnh 0
	glVertex3f(x + dx / 2, y, z - dz / 2); // Đỉnh 1

	glVertex3f(x + dx / 2, y, z + dz / 2); // Đỉnh 2
	glVertex3f(x - dx / 2, y, z + dz / 2); // Đỉnh 3

	glVertex3f(x + dx / 2, y, z + dz / 2); // Đỉnh 2
	glVertex3f(x + dx / 2, y, z - dz / 2); // Đỉnh 3

	glVertex3f(x - dx / 2, y, z + dz / 2); // Đỉnh 2
	glVertex3f(x - dx / 2, y, z - dz / 2); // Đỉnh 3
	glEnd();

	glColor3f(0.5f, 0.5f, 0.5f);
	glLineWidth(5.0f);
	glBegin(GL_LINE_LOOP);
	angle = 2 * PI / 16;
	for (int i = 0; i < 16; i++) {
		if (i % 2 == 0) {
			glVertex3f(x + dx / 3.7 * sin(angle*i), y, z + dz / 3.7 * cos(angle*i));
		}
		else {
			glVertex3f(x + dx / 4.7 * sin(angle*i), y, z + dz / 4.7 * cos(angle*i));
		}
	}
	glEnd();

	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_POLYGON);
	glVertex3f(x + dx / 2, y, z + dz / 2.8); // Đỉnh 0
	glVertex3f(x + dx / 2, y, z + dz / 3.2); // Đỉnh 1
	glVertex3f(x + dx / 3.2, y, z + dz / 2); // Đỉnh 2
	glVertex3f(x + dx / 2.8, y, z + dz / 2); // Đỉnh 3
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(x + dx / 2, y, z - dz / 2.8); // Đỉnh 0
	glVertex3f(x + dx / 2, y, z - dz / 3.2); // Đỉnh 1
	glVertex3f(x + dx / 3.2, y, z - dz / 2); // Đỉnh 2
	glVertex3f(x + dx / 2.8, y, z - dz / 2); // Đỉnh 3
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(x - dx / 2, y, z + dz / 2.8); // Đỉnh 0
	glVertex3f(x - dx / 2, y, z + dz / 3.2); // Đỉnh 1
	glVertex3f(x - dx / 3.2, y, z + dz / 2); // Đỉnh 2
	glVertex3f(x - dx / 2.8, y, z + dz / 2); // Đỉnh 3
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(x - dx / 2, y, z - dz / 2.8); // Đỉnh 0
	glVertex3f(x - dx / 2, y, z - dz / 3.2); // Đỉnh 1
	glVertex3f(x - dx / 3.2, y, z - dz / 2); // Đỉnh 2
	glVertex3f(x - dx / 2.8, y, z - dz / 2); // Đỉnh 3
	glEnd();

	glColor3f(0.6f, 0.3f, 0.0f);
	glLineWidth(20.0f);
	glBegin(GL_LINES);
	glVertex3f(x + dx / 3.6, y, z + dz / 2);
	glVertex3f(x + dx / 3.6, y, z + dz / 6);
	glVertex3f(x + dx / 2, y, z + dz / 3.6);
	glVertex3f(x + dx / 6, y, z + dz / 3.6);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(x - dx / 3.6, y, z + dz / 2);
	glVertex3f(x - dx / 3.6, y, z + dz / 6);
	glVertex3f(x - dx / 2, y, z + dz / 3.6);
	glVertex3f(x - dx / 6, y, z + dz / 3.6);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(x - dx / 3.6, y, z - dz / 2);
	glVertex3f(x - dx / 3.6, y, z - dz / 6);
	glVertex3f(x - dx / 2, y, z - dz / 3.6);
	glVertex3f(x - dx / 6, y, z - dz / 3.6);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(x + dx / 3.6, y, z - dz / 2);
	glVertex3f(x + dx / 3.6, y, z - dz / 6);
	glVertex3f(x + dx / 2, y, z - dz / 3.6);
	glVertex3f(x + dx / 6, y, z - dz / 3.6);
	glEnd();

	glPushMatrix();
	glTranslatef(0, 0, dz / 4);
	drawRectangle(x, y, z, 0, PI, dx / 5);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0, -dz / 4);
	drawRectangle(x, y, z, PI, 2 * PI, dx / 5);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(dz / 4, 0, 0);
	drawRectangle(x, y, z, 3 * PI / 2, 5 * PI / 2, dx / 5);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-dz / 4, 0, 0);
	drawRectangle(x, y, z, PI / 2, 3 * PI / 2, dx / 5);
	glPopMatrix();

	glLineWidth(1.0f);

	glColor3f(0.98f, 0.98f, 0.98f);
	glBegin(GL_POLYGON);
	glVertex3f(x - dx / 2, y, z - dz / 2); // Đỉnh 0
	glVertex3f(x + dx / 2, y, z - dz / 2); // Đỉnh 1
	glVertex3f(x + dx / 2, y, z + dz / 2); // Đỉnh 2
	glVertex3f(x - dx / 2, y, z + dz / 2); // Đỉnh 3
	glEnd();

}


float	ColorArr[COLORNUM][3] = { { 1.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0 },{ 0.0,  0.0, 1.0 },
{ 1.0, 1.0,  0.0 },{ 1.0, 0.0, 1.0 },{ 0.0, 1.0, 1.0 },
{ 0.3, 0.3, 0.3 },{ 0.5, 0.5, 0.5 },{ 0.9,  0.9, 0.9 },
{ 1.0, 0.5,  0.5 },{ 0.5, 1.0, 0.5 },{ 0.5, 0.5, 1.0 },
{ 0.0, 0.0, 0.0 },{ 1.0, 1.0, 1.0 } };

void Mesh::DrawWireframe(float x, float y, float z)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(x, y, z);
	glDisable(GL_LIGHTING);
	for (int f = 0; f < numFaces; f++)
	{
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int		iv = face[f].vert[v].vertIndex;

			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
	glEnable(GL_LIGHTING);
}

void Mesh::calculateNorm() {
	float dx, dy, dz;
	int idx, next;
	for (int f = 0; f < numFaces; f++) {
		dx = 0;
		dy = 0;
		dz = 0;
		for (int v = 0; v < face[f].nVerts; v++) {
			idx = v;
			next = (idx + 1) % face[f].nVerts;

			int p1 = face[f].vert[idx].vertIndex;
			int p2 = face[f].vert[next].vertIndex;

			dx = dx + (pt[p1].y - pt[p2].y) * (pt[p1].z + pt[p2].z);
			dy = dy + (pt[p1].z - pt[p2].z) * (pt[p1].x + pt[p2].x);
			dz = dz + (pt[p1].x - pt[p2].x) * (pt[p1].y + pt[p2].y);
		}
		face[f].norm.set(dx, dy, dz);
		face[f].norm.normalize();
	}
}

void Mesh::DrawColor(int color)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (int f = 0; f < numFaces; f++)
	{
		glBegin(GL_POLYGON);
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int		iv = face[f].vert[v].vertIndex;
			int		ic = face[f].vert[v].colorIndex;

			ic = f % COLORNUM;

			glColor3f(ColorArr[ic][0], ColorArr[ic][1], ColorArr[ic][2]);
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
}

void Mesh::DrawNorm(bool flag, float ambient_default[4], float diffuse_default[4], float specular_default[4], float shine_default, float ambient[4], float diffuse[4], float specular[4], float shine) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (int f = 0; f < numFaces; f++)
	{
		glBegin(GL_POLYGON);
		glColor3f(0, 0, 0);
		if (flag == false) {
			glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_default);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_default);
			glMaterialfv(GL_FRONT, GL_SPECULAR, specular_default);
			glMaterialf(GL_FRONT, GL_SHININESS, shine_default);
		}
		else {
			if (face[f].flag == true) {
				glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
				glMaterialf(GL_FRONT, GL_SHININESS, shine);
			}
			else {
				glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_default);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_default);
				glMaterialfv(GL_FRONT, GL_SPECULAR, specular_default);
				glMaterialf(GL_FRONT, GL_SHININESS, shine_default);
			}
		}
		for (int v = 0; v < face[f].nVerts; v++)
		{
			int		iv = face[f].vert[v].vertIndex;
			glNormal3f(face[f].norm.x, face[f].norm.y, face[f].norm.z);
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
}

void Cuboid::createShape() {
	numVerts = 8;
	float x = fsizeX;
	float y = fsizeY;
	float z = fsizeZ;
	pt = new Point3[numVerts];
	pt[0] = Point3(x, 0, z);
	pt[1] = Point3(x, 0, -z);
	pt[2] = Point3(x, y, -z);
	pt[3] = Point3(x, y, z);
	pt[4] = Point3(-x, y, -z);
	pt[5] = Point3(-x, y, z);
	pt[6] = Point3(-x, 0, z);
	pt[7] = Point3(-x, 0, -z);

	numFaces = 6;
	face = new Face[numFaces];

	face[0].nVerts = 4;
	face[0].vert = new VertexID[face[0].nVerts];
	face[0].vert[0].vertIndex = 0;
	face[0].vert[1].vertIndex = 1;
	face[0].vert[2].vertIndex = 2;
	face[0].vert[3].vertIndex = 3;

	face[1].nVerts = 4;
	face[1].vert = new VertexID[face[1].nVerts];
	face[1].vert[0].vertIndex = 2;
	face[1].vert[1].vertIndex = 4;
	face[1].vert[2].vertIndex = 5;
	face[1].vert[3].vertIndex = 3;

	face[2].nVerts = 4;
	face[2].vert = new VertexID[face[2].nVerts];
	face[2].vert[0].vertIndex = 5;
	face[2].vert[1].vertIndex = 4;
	face[2].vert[2].vertIndex = 7;
	face[2].vert[3].vertIndex = 6;

	face[3].nVerts = 4;
	face[3].vert = new VertexID[face[3].nVerts];
	face[3].vert[0].vertIndex = 1;
	face[3].vert[1].vertIndex = 0;
	face[3].vert[2].vertIndex = 6;
	face[3].vert[3].vertIndex = 7;

	face[4].nVerts = 4;
	face[4].vert = new VertexID[face[4].nVerts];
	face[4].vert[0].vertIndex = 5;
	face[4].vert[1].vertIndex = 6;
	face[4].vert[2].vertIndex = 0;
	face[4].vert[3].vertIndex = 3;

	face[5].nVerts = 4;
	face[5].vert = new VertexID[face[5].nVerts];
	face[5].vert[0].vertIndex = 7;
	face[5].vert[1].vertIndex = 4;
	face[5].vert[2].vertIndex = 2;
	face[5].vert[3].vertIndex = 1;

	calculateNorm();
}

void Shape1::creatShape() {
	numVerts = 16;
	pt = new Point3[numVerts];

	pt[0].set(x - Length / 2, y, z - Width / 2);  // Góc dưới trái
	pt[1].set(x + Length / 2, y, z - Width / 2);  // Góc dưới phải
	pt[2].set(x + Length / 2, y, z + Width / 2);  // Góc trên phải
	pt[3].set(x - Length / 2, y, z + Width / 2);  // Góc trên trái

	pt[4].set(x - Length / 2, y + height1, z - Width / 2);  // Góc dưới trái
	pt[5].set(x + Length / 2, y + height1, z - Width / 2);  // Góc dưới phải
	pt[6].set(x + Length / 2, y + height1, z + Width / 2);  // Góc trên phải
	pt[7].set(x - Length / 2, y + height1, z + Width / 2);  // Góc trên trái

	pt[8].set(x - length / 2, y + height1 + height2, z - width / 2);  // Góc dưới trái
	pt[9].set(x + length / 2, y + height1 + height2, z - width / 2);  // Góc dưới phải
	pt[10].set(x + length / 2, y + height1 + height2, z + width / 2);  // Góc trên phải
	pt[11].set(x - length / 2, y + height1 + height2, z + width / 2);  // Góc trên trái

	pt[12].set(x - length / 2, y + height1 + height2 + height3, z - width / 2);  // Góc dưới trái
	pt[13].set(x + length / 2, y + height1 + height2 + height3, z - width / 2);  // Góc dưới phải
	pt[14].set(x + length / 2, y + height1 + height2 + height3, z + width / 2);  // Góc trên phải
	pt[15].set(x - length / 2, y + height1 + height2 + height3, z + width / 2);  // Góc trên trái

	numFaces = 13;
	face = new Face[numFaces];

	int idx = 0;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[4];
	face[idx].vert[0].vertIndex = 13;
	face[idx].vert[1].vertIndex = 12;
	face[idx].vert[2].vertIndex = 15;
	face[idx].vert[3].vertIndex = 14;
	face[idx].flag = true;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[4];
	face[idx].vert[0].vertIndex = 0;
	face[idx].vert[1].vertIndex = 4;
	face[idx].vert[2].vertIndex = 5;
	face[idx].vert[3].vertIndex = 1;
	face[idx].flag = true;
	idx++;
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[4];
	face[idx].vert[0].vertIndex = 1;
	face[idx].vert[1].vertIndex = 5;
	face[idx].vert[2].vertIndex = 6;
	face[idx].vert[3].vertIndex = 2;
	face[idx].flag = true;
	idx++;
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[4];
	face[idx].vert[0].vertIndex = 2;
	face[idx].vert[1].vertIndex = 6;
	face[idx].vert[2].vertIndex = 7;
	face[idx].vert[3].vertIndex = 3;
	face[idx].flag = true;
	idx++;
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[4];
	face[idx].vert[0].vertIndex = 3;
	face[idx].vert[1].vertIndex = 7;
	face[idx].vert[2].vertIndex = 4;
	face[idx].vert[3].vertIndex = 0;
	face[idx].flag = true;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[4];
	face[idx].vert[0].vertIndex = 4;
	face[idx].vert[1].vertIndex = 8;
	face[idx].vert[2].vertIndex = 9;
	face[idx].vert[3].vertIndex = 5;
	face[idx].flag = true;
	idx++;
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[4];
	face[idx].vert[0].vertIndex = 5;
	face[idx].vert[1].vertIndex = 9;
	face[idx].vert[2].vertIndex = 10;
	face[idx].vert[3].vertIndex = 6;
	face[idx].flag = true;
	idx++;
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[4];
	face[idx].vert[0].vertIndex = 6;
	face[idx].vert[1].vertIndex = 10;
	face[idx].vert[2].vertIndex = 11;
	face[idx].vert[3].vertIndex = 7;
	face[idx].flag = true;
	idx++;
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[4];
	face[idx].vert[0].vertIndex = 4;
	face[idx].vert[1].vertIndex = 7;
	face[idx].vert[2].vertIndex = 11;
	face[idx].vert[3].vertIndex = 8;
	face[idx].flag = true;
	idx++;

	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[4];
	face[idx].vert[0].vertIndex = 8;
	face[idx].vert[1].vertIndex = 12;
	face[idx].vert[2].vertIndex = 13;
	face[idx].vert[3].vertIndex = 9;
	face[idx].flag = true;
	idx++;
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[4];
	face[idx].vert[0].vertIndex = 9;
	face[idx].vert[1].vertIndex = 13;
	face[idx].vert[2].vertIndex = 14;
	face[idx].vert[3].vertIndex = 10;
	face[idx].flag = true;
	idx++;
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[4];
	face[idx].vert[0].vertIndex = 10;
	face[idx].vert[1].vertIndex = 14;
	face[idx].vert[2].vertIndex = 15;
	face[idx].vert[3].vertIndex = 11;
	face[idx].flag = true;
	idx++;
	face[idx].nVerts = 4;
	face[idx].vert = new VertexID[4];
	face[idx].vert[0].vertIndex = 8;
	face[idx].vert[1].vertIndex = 11;
	face[idx].vert[2].vertIndex = 15;
	face[idx].vert[3].vertIndex = 12;
	face[idx].flag = true;
	idx++;

	calculateNorm();
}

float angle = 20.0f;
float wheelAngle = 0.0f;
bool  autoRotate = false;
float rotationDirection = 0.01f;
bool drawToggle = 0;
bool viewToggle = 1;
float cameraHeight = 5;
float camera_dis = 12;
float cameraDx = 10;
float cameraDz = 10;
bool drawFlag = 0;

float util_ambient[4];
float util_diffuse[4];
float util_specular[4];

float ambient_default[4] = { 0.25f, 0.25f, 0.25f, 1.0f };
float diffuse_default[4] = { 0.4f, 0.4f, 0.4f, 1.0f };
float specular_default[4] = { 0.774597f, 0.774597f, 0.774597f, 1.0f };
float shine_default = 76.8f;


Cuboid GiaDo = Cuboid(1, 0.5, 4);
Shape1 shape1_1 = Shape1(0, 0.5, -3, 0.3, 0.6, 0.5, 0.8, 0.2, 0.4, 3);
Shape1 shape1_2 = Shape1(0, 0.5, 3, 0.3, 0.6, 0.5, 0.8, 0.2, 0.4, 3);
Shape3 shape3 = Shape3(0.3, 2.5, 0.3, 0.4, 5, 5.4);
Shape4 shape4 = Shape4(0.5, 0.2, 0.25, 2.5, 0);
Shape4 shape4_1 = Shape4(0.7, 0.1, 0.85, 1.5, 0);
Shape5 shape5_1 = Shape5(0.65, 2.5, 3, 1, 0.5, 0.3);
Shape5 shape5_2 = Shape5(0.65, 2.5, -3, 1, 0.5, 0.3);
Shape6 shape6_1 = Shape6(4.2, 0.15, 0.8, 2.5, 2.4);
Shape6 shape6_2 = Shape6(4.2, 0.15, 0.8, 2.5, -2.4);
Wheel  wheel = Wheel(0.35, 0, 0, 0.2, 0.45, 1.3, 1.6, 0.3);
Slider slider = Slider(0.8, 2.5, 0, 0.3, 3.4, 3.8, 0.2, 0.6);

void drawText(float xPos, float yPos, float zPos, string str) {
	glRasterPos3f(xPos, yPos, zPos);
	void* font = GLUT_BITMAP_9_BY_15;
	for (int i = 0; i < str.length(); ++i)
		glutBitmapCharacter(font, str[i]);
}

void drawAxis() {
	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(5, 0, 0);

	glVertex3f(0, 0, 0);
	glVertex3f(0, 5, 0);

	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 5);
	glEnd();
	glColor3f(0, 0, 0);
	drawText(4.9, 0, 0, "X");
	drawText(0, 4.3, 0, "Y");
	drawText(0, 0, 4.9, "Z");
}

void changeCameraPos() {
	cameraDx = cos(DEG2RAD(angle))*camera_dis;
	cameraDz = sin(DEG2RAD(angle))*camera_dis;
}
void keyNormal(unsigned char key, int x, int y) {
	if (key == '1') {
		viewToggle ? wheelAngle += 1.0f : wheelAngle -= 1.0f;
	}
	else if (key == '2') {
		viewToggle ? wheelAngle -= 1.0f : wheelAngle += 1.0f;
	}
	else if (key == 'A' || key == 'a') autoRotate = !autoRotate;
	else if (key == 'B' || key == 'b') rotationDirection = -rotationDirection;
	else if (key == 'W' || key == 'w') drawToggle = !drawToggle;
	else if (key == 'V' || key == 'v') viewToggle = !viewToggle;
	else if (key == '+' && viewToggle) {
		camera_dis += 0.1;
		changeCameraPos();
	}
	else if (key == '-' && viewToggle) {
		camera_dis -= 0.1;
		changeCameraPos();
	}
	glutPostRedisplay();
}

void setMaterial(float ambient[], float diffuse[], float specular[], float shiness)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiness);
}

void drawChainShape1() {
	float mat_ambient[] = { 1.0f, 0.0f, 0.0f, 0.8f };
	float mat_diffuse[] = { 1.0f, 0.0f, 0.0f, 0.8f };
	float mat_specular[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	setMaterial(mat_ambient, mat_diffuse, mat_specular, 100.0f);

	if (drawToggle == false) shape1_1.DrawNorm(drawFlag, mat_ambient, mat_diffuse, mat_specular, 12.8, ambient_default, diffuse_default, specular_default, shine_default);
	else shape1_1.DrawWireframe(1,0,0);
	if (drawToggle == false) shape1_2.DrawNorm(drawFlag, mat_ambient, mat_diffuse, mat_specular, 12.8, ambient_default, diffuse_default, specular_default, shine_default);
	else shape1_2.DrawWireframe(1,0,0);
	if (drawToggle == false) shape3.DrawNorm(drawFlag, mat_ambient, mat_diffuse, mat_specular, 12.8, ambient_default, diffuse_default, specular_default, shine_default);
	else shape3.DrawWireframe(0, 0, 0);

	float mat_ambient2[] = { 0.19225f, 0.19225f, 0.19225f, 1.0f };
	float mat_diffuse2[] = { 0.2f, 0.0f, 0.2f, 1.0f };
	float mat_specular2[] = { 1.0f, 0.0f, 0.5f, 1.0f };
	setMaterial(mat_ambient2, mat_diffuse2, mat_specular2, 100.0f);
	if (drawToggle == false) shape4.DrawNorm(drawFlag, mat_ambient2, mat_diffuse2, mat_specular2, 12.8, ambient_default, diffuse_default, specular_default, shine_default);
	else shape4.DrawWireframe(0.19225f, 0.50754f, 0.508273f);

	float mat_ambient3[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float mat_diffuse3[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	float mat_specular3[] = { 1.0f, 0.25f, 0.0f, 1.0f };
	setMaterial(mat_ambient3, mat_diffuse3, mat_specular3, 100.0f);
	if (drawToggle == false) shape5_1.DrawNorm(drawFlag, mat_ambient3, mat_diffuse3, mat_specular3, 12.8, ambient_default, diffuse_default, specular_default, shine_default);
	else shape5_1.DrawWireframe(0.2, 0.2, 0.2);
	if (drawToggle == false) shape5_2.DrawNorm(drawFlag, mat_ambient3, mat_diffuse3, mat_specular3, 12.8, ambient_default, diffuse_default, specular_default, shine_default);
	else shape5_2.DrawWireframe(0.2, 0.2, 0.2);

	float mat_ambient1[] = { 0.6f, 0.8f, 0.2f, 1.0f };
	float mat_diffuse1[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	float mat_specular1[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	setMaterial(mat_ambient1, mat_diffuse1, mat_specular1, 100.0f);
	if (drawToggle == false) GiaDo.DrawNorm(drawFlag, mat_ambient1, mat_diffuse1, mat_specular1, 12.8, ambient_default, diffuse_default, specular_default, shine_default);
	else GiaDo.DrawWireframe(1,0.8,0.6);

}

void drawWheelRound() {
	float mat_ambient[] = { 1.0f, 0.0f, 0.0f, 0.8f };
	float mat_diffuse[] = { 1.0f, 0.0f, 0.0f, 0.8f };
	float mat_specular[] = { 1.0f, 0.0f, 0.0f, 0.8f };
	if (drawToggle == false) shape4_1.DrawNorm(drawFlag, mat_ambient, mat_diffuse, mat_specular, 12.8, ambient_default, diffuse_default, specular_default, shine_default);
	else shape4_1.DrawWireframe(1.0f, 0.0f, 0.0f);

	float mat_ambient4[] = { 1.0f, 0.0f, 1.0f, 0.0f };
	float mat_diffuse4[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	float mat_specular4[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	//float mat_ambient4[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	//float mat_diffuse4[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	//float mat_specular4[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	if (drawToggle == false) wheel.DrawNorm(drawFlag, mat_ambient4, mat_diffuse4, mat_specular4, 12.8, ambient_default, diffuse_default, specular_default, shine_default);
	else wheel.DrawWireframe(0.0f, 0.0f, 1.0f);
}

void drawSlider() {
	float mat_ambient[] = { 0.0f, 1.0f, 0.0f, 0.8f };
	float mat_diffuse[] = { 0.0f, 1.0f, 0.0f, 0.8f };
	float mat_specular[] = { 0.0f, 1.0f, 0.0f, 0.8f };
	setMaterial(mat_ambient, mat_diffuse, mat_specular, 50.0f);
	if (drawToggle == false) shape6_1.DrawNorm(drawFlag, mat_ambient, mat_diffuse, mat_specular, 12.8, ambient_default, diffuse_default, specular_default, shine_default);
	else shape6_1.DrawWireframe(0.0f, 0.0f, 0.0f);
	if (drawToggle == false) shape6_2.DrawNorm(drawFlag, mat_ambient, mat_diffuse, mat_specular, 12.8, ambient_default, diffuse_default, specular_default, shine_default);
	else shape6_2.DrawWireframe(0.0f, 0.0f, 0.0f);
	if (drawToggle == false) slider.DrawNorm(drawFlag, mat_ambient, mat_diffuse, mat_specular, 12.8, ambient_default, diffuse_default, specular_default, shine_default);
	else slider.DrawWireframe(0.0f, 1.0f, 0.0f);
}
void drawFloor() {
	glDisable(GL_LIGHTING);
	for (int i = -20; i < 21; i+=5) {
		for (int j = -20; j < 21; j+=5) {
			FLoorTiles(i, 0, j, 5, 5);
		}
	}
	glEnable(GL_LIGHTING);
}

void myReshape(int w, int h) {
	float factor = 2;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h) gluPerspective(38.7, (double)h / w, 1, 1000);
	else gluPerspective(38.7, (double)w / h, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
}

void setLight() {
	GLfloat	lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat	lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat	lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat light_position[] = { 6.0f, 6.0f, 6.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

	GLfloat lightDiffuse1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lightSpecular1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lightAmbient1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat light_position1[] = { -6.0f, -3.0f, 6.0f, 0.0f };

	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular1);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);
}

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	changeCameraPos();
	if (viewToggle) gluLookAt(cameraDx, cameraHeight, cameraDz, 0, 2.5, 0, 0, 1, 0);
	else {
		glOrtho(-10.0f, 10.0f, -10.0f, 10.0f, 2.0f, 200.0f);
		glTranslatef(0.0f, -2.5f, 0.0f);
		glRotatef(90, 0, 1, 0);
	}

	setLight();

	glPushMatrix();
	drawChainShape1();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 2.5f, 0.0f);
	if (autoRotate) {
		wheelAngle += rotationDirection;
		if (viewToggle) wheelAngle += rotationDirection * 10;
		glutPostRedisplay();
	}
	glRotatef(wheelAngle, 1.0f, 0.0f, 0.0f);
	drawWheelRound();
	glPopMatrix();

	glPushMatrix();
	if (autoRotate) {
		wheelAngle += rotationDirection;
		if (viewToggle) wheelAngle += rotationDirection * 10;
		glutPostRedisplay();
	}
	glTranslatef(0.0f, 0.0f, 1.5 * sin(wheelAngle / 360 * 2 * PI));
	drawSlider();
	glPopMatrix();

	if(viewToggle) drawFloor();

	glFlush();
}

void init() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glFrontFace(GL_CCW);
	glColor3f(0.0, 0.0, 0.0);

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(38.7, 1, 1, 1000);
}

void mySpecialFunc(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT) {
		angle -= 1;
		changeCameraPos();
	}
	else if (key == GLUT_KEY_RIGHT) {
		angle += 1;
		changeCameraPos();
	}
	else if (key == GLUT_KEY_UP) {
		cameraHeight += 0.2;
	}
	else if (key == GLUT_KEY_DOWN) {
		cameraHeight -= 0.2;
	}
	glutPostRedisplay();
}


int main(int argc, char* argv[]) {
	cout << "1     :  Xoay banh xe nguoc chieu kim dong ho" << endl;
	cout << "2     :  Xoay banh xe cung chieu kim dong ho" << endl;
	cout << "a, A  :  Bat/tat che do xoay banh xe tu dong" << endl;
	cout << "b, B  :  Chuyen huong quay cua banh xe" << endl;
	cout << "W, w  :  Chuyen doi qua lai giua che do khung day va to mau" << endl;
	cout << "V, v  :  Chuyen doi qua lai giua view 3D va 2D" << endl;
	cout << "+     : Tang khoang cach camera" << endl;
	cout << "-     : Giam khoang cach camera" << endl;
	cout << "up arrow  : Tang chieu cao camera" << endl;
	cout << "down arrow: Giam chieu cao camera" << endl;
	cout << "<-        : Quay camera theo chieu kim dong ho" << endl;
	cout << "->        : Quay camera nguoc chieu kim dong ho" << endl;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1200, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Cao Le Hoan Thien - 2213243");
	glutDisplayFunc(myDisplay);
	glutSpecialFunc(mySpecialFunc);
	glutKeyboardFunc(keyNormal);
	glutReshapeFunc(myReshape);
	
	GiaDo.createShape();
	shape1_1.creatShape();
	shape1_2.creatShape();
	shape3.creatShape();
	shape4.creatShape();
	shape4_1.creatShape();
	wheel.creatShape();
	shape5_1.creatShape();
	shape5_2.creatShape();
	shape6_1.creatShape();
	shape6_2.creatShape();
	slider.creatShape();

	init();

	glutMainLoop();
}
