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
	vector<int>pickingNames;
	
	void draw(Appearance* appea);
	Node();
	void update(unsigned long milis);

};

#endif