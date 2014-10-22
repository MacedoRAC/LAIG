#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <string>
#include<vector>
#include "Texture.h"

using namespace std;

class Primitive{
public:
	Primitive(void){};
	string type;
	virtual void draw(){};
};

class Rectangle : public Primitive{
public:
	float x1, x2, y1, y2;

	void draw();
	void draw(Texture * text);
};

class Triangle : public Primitive{
public:
	float x1, x2, x3, y1, y2, y3, z1, z2, z3;

	void draw();
	void draw(Texture * text);
};

class Cylinder : public Primitive{
public:
	float base, top, height;
	int slices, stacks;

	void draw();
};

class Torus : public Primitive{
public:
	int slices, loops;
	bool smooth;
	float inner, outer;

	void draw();
};

class Sphere : public Primitive{
public:
	float radius;
	int slices, stacks;

	void draw();
};
#endif