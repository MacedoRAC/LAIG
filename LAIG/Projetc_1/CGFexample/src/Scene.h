#ifndef SCENE_H
#define SCENE_H

#include"CGFscene.h"
#include"Graph.h"
#include"Primitive.h"
#include<vector>

class Scene: public CGFscene{
	
public:
	Graph* graph;
	vector<CGFlight *> lights;
	int  actualCam;
	int wire;
	int light0, light1, light2, light3;
	vector<Camera*>::iterator camIt;
	vector<Camera*> cameras;
	
	void init();
	void display();
	void update(unsigned long millis);

};

#endif