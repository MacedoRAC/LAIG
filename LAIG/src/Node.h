#ifndef NODE_H
#define NODE_H

#include<string>
#include<vector>
#include"Primitive.h"
#include"Appearance.h"

using namespace std;

class Node{
public:
	string id, appRef;
	vector<string> descendants;
	vector<Node*> descendantNode;
	vector<vector<float>> matrix;
	vector<Triangle> triangles;
	vector<Rectangle> rectangles;
	vector<Cylinder> cylinders;
	vector<Torus> torusses;
	vector<Sphere> spheres;
	Appearance * app;
};

#endif