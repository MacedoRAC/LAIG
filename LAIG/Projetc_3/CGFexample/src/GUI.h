#ifndef GUI_H
#define GUI_H

#include "CGFinterface.h"
#include "Camera.h"
#include "Graph.h"

class GUI: public CGFinterface{
public:
	Graph* graph;
	vector<string>cameras;
	int wind;
	GLUI_Spinner * windSpinner;

	GUI(Graph* graph);
	virtual void initGUI();
	virtual void processGUI(GLUI_Control *ctrl);

	virtual void processMouse(int button, int state, int x, int y);	
	void performPicking(int x, int y);
	void processHits(GLint hits, GLuint buffer[]); 
};

#endif