#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <string>
#include<vector>
#include "Texture.h"

using namespace std;

class Primitive{
public:
	Primitive(void){};
	Primitive(string type);
	string type;
	virtual void draw(){};
};

class Rectangle : public Primitive{
public:
	float x1, x2, y1, y2;

	void draw();
	void draw(Texture * text);
	Rectangle(string type, float x1, float x2, float y1, float y2);
};

class Triangle : public Primitive{
public:
	float x1, x2, x3, y1, y2, y3, z1, z2, z3;

	void draw();
	void draw(Texture * text);
	Triangle(string type, float x1, float x2, float x3, float y1, float y2, float y3, float z1, float z2, float z3);
};

class Cylinder : public Primitive{
public:
	float base, top, height;
	int slices, stacks;

	void draw();
	Cylinder(string type, float base, float top, float height, int slices, int stacks);
};

class Torus : public Primitive{
public:
	int slices, loops;
	bool smooth;
	float inner, outer;

	void draw();
	Torus(string type, int slices, int loops, bool smooth, float inner, float outer);
};

class Sphere : public Primitive{
public:
	float radius;
	int slices, stacks;

	void draw();
	Sphere(string type, float radius, int slices, int stacks);
};
#endif