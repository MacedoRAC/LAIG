#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include<string>

using namespace std;

class Primitive{
public:
	string type;
};

class Rectangle{
public:
	float x1, x2, y1, y2;

	void draw();
};

class Triangle{
public:
	float x1, x2, x3, y1, y2, y3, z1, z2, z3;

	void draw();
};

class Cylinder{
public:
	float base, top, height;
	int slices, stacks;

	void draw();
};

class Torus{
public:
	int slices, loops;
	bool smooth;
	float inner, outer;

	void draw();
};

class Sphere{
public:
	float radius;
	int slices, stacks;

	void draw();
};
#endif