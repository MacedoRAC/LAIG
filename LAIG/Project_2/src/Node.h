#ifndef NODE_H
#define NODE_H

#include<string>
#include<vector>
#include"Primitive.h"
#include"Appearance.h"
#include"Animation.h"

using namespace std;

class Node{
public:
	string id, appRef;
	vector<string> descendants;
	vector<Node*> descendantNode;
	float matrix [16];
	vector<Primitive*> primitives;
	Appearance * app;
	bool hasDisplayList;
	GLuint displayList;
	string animationRef;
	Flag * flag;
	ComposedAnimation* animations;
	int wind;
	
	void draw(Appearance* appea);
	Node(){this->app=NULL; this->displayList=NULL; this->flag=NULL; this->animations=NULL;};
	void update(unsigned long milis);

};

#endif