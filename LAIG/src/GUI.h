#ifndef GUI_H
#define GUI_H

#include"CGFscene.h"
#include"Graph.h"
#include<vector>

class GUI: public CGFscene{
	
public:
	Graph graph;
	vector<CGFlight *> lights;
	
	
	void init();
	void display();
	void update(unsigned long millis);
	void drawNode(Node *n);

};

#endif