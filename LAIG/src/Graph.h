#ifndef GRAPH_H
#define GRAPH_H

#include<string>
#include<vector>
#include<map>
#include"Node.h"
#include"Light.h"
#include"Camera.h"

using namespace std;

class Graph{
public:
	string rootId;
	Node * rootNode;
	string drawingMode;
	string shading;
	vector<float> background;
	string cullingFace;
	string cullingOrder;
	bool lDoubleSided;
	bool lLocal;
	bool lEnabled;
	vector<float> lAmbient;
	map<string,Node> nodes;
	string rootCamera;
	map<string,Camera *> cameras;
	vector<Light> lights;
	vector<Texture> textures;
	map<string ,Appearance> appearances;

	void updateRootNode();
	void updateDescendantNode();
};

#endif