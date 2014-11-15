#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <string>
#include<vector>
#include "Texture.h"
#include"CGFobject.h"


using namespace std;

class Primitive{
public:
	Primitive(void){};
	Primitive(string type);
	string type;
	virtual void draw(){};
	virtual void draw(Texture* text){}; 
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
	void draw(Texture * text);
	Cylinder(string type, float base, float top, float height, int slices, int stacks);
};

class Torus : public Primitive{
public:
	int slices, loops;
	bool smooth;
	float inner, outer;

	void draw();
	void draw(Texture * text);
	Torus(string type, int slices, int loops, bool smooth, float inner, float outer);
};

class Sphere : public Primitive{
public:
	float radius;
	int slices, stacks;

	void draw();
	void draw(Texture * text);
	Sphere(string type, float radius, int slices, int stacks);
};

class Plane : public Primitive{
public:
	int parts;

	static const GLfloat controlPoints[4][3];
	static const GLfloat normalComponents[4][3];
	static const GLfloat texturePoints[4][2];

	void draw();
	void draw(Texture * text);
	Plane(string type, int parts);
};


class Patch : public Primitive{
public:
	int order, partsU, partsV;
	string compute;
	GLfloat * controlPoints;

	void draw(){};
	void draw(Texture * text);
	Patch(string type, int order, int partsU, int partsV, string compute, GLfloat * cPoints);
};


class Vehicle : public Primitive{
public:
	
	void draw(){};
	void draw(Texture * text){};
	Vehicle(string type);

};


class Flag: public Primitive{
public:
	string texture;

	void draw(){};
	void draw(Texture * text){};
	Flag(string type, string texture);
};


#endif