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
	float matrix [16];
	vector<Primitive*> primitives;
	Appearance * app;
	
	void draw(Appearance* appea);
	Node(){this->app=NULL;};

};

#endif