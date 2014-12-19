#include"GUI.h"
#include"Scene.h"

// buffer to be used to store the hits during picking
#define BUFSIZE 256
GLuint selectBuf[BUFSIZE];

GUI:: GUI(Graph* graph){
	this->graph=graph;
	
	map<string, Camera*>::iterator it;

	for(it=graph->cameras.begin(); it!=graph->cameras.end(); it++){
		cameras.push_back((*it).second->id);
	}
}


void GUI::initGUI()
{

	graph = (((Scene*) scene)->graph);

	int* wire = &(((Scene*) scene)->wire);

	int* cam = &(((Scene*) scene)->actualCam);

	
	GLUI_Panel *varPanel= addPanel("Settings:", 1);


	GLUI_Panel *modepanel = addPanelToPanel(varPanel, "Mode", 1);
	GLUI_RadioGroup* wireFrameGroup = addRadioGroupToPanel(modepanel,wire);
	addRadioButtonToGroup(wireFrameGroup, "\tFill");
	addRadioButtonToGroup(wireFrameGroup, "\tWire");
	addRadioButtonToGroup(wireFrameGroup, "\tPoint");
	
	addColumnToPanel(varPanel);
	GLUI_Panel *cammodel = addPanelToPanel(varPanel, "Camera", 1);
	GLUI_RadioGroup* camerasGroup = addRadioGroupToPanel(cammodel,cam);
	for(unsigned int i = 0; i < cameras.size();i++)
	{

		addRadioButtonToGroup(camerasGroup, (char *)cameras[i].c_str());
	}
	addRadioButtonToGroup(camerasGroup, "Free Camera");
	

	addColumnToPanel(varPanel);
	GLUI_Panel *lightspanel = addPanelToPanel(varPanel, "Lights", 1);
	for(unsigned int i = 0; i < graph->lights.size();i++)
	{
		if (graph->lights[i].enabled)
			addCheckboxToPanel(lightspanel,
			(char*)graph->lights[i].id.c_str(), NULL, i)->set_int_val(
			1);
		else
			addCheckboxToPanel(lightspanel,
			(char*)graph->lights[i].id.c_str(), NULL, i)->set_int_val(
			0);
	}
	
	addColumnToPanel(varPanel);
	GLUI_Panel *flagPanel = addPanelToPanel(varPanel, "Flag", 1);
	windSpinner = addSpinnerToPanel(flagPanel, "Wind", GLUI_SPINNER_INT, &wind);

	windSpinner->set_int_limits(0, 10, GLUI_LIMIT_WRAP);
}


void GUI::processGUI(GLUI_Control *ctrl)
{	
	if(ctrl->user_id == windSpinner->get_id())
		graph->setWind(wind);

	if (ctrl->user_id >= 0)
		if (ctrl->get_int_val() == 1) {
			graph->lights[ctrl->user_id].enabled = true;
			((Scene *) scene)->lights[ctrl->user_id]->enable();
		} else {
			graph->lights[ctrl->user_id].enabled = false;
			((Scene *) scene)->lights[ctrl->user_id]->disable();

		}
}

void GUI::processMouse(int button, int state, int x, int y) 
{
	CGFinterface::processMouse(button,state, x, y);

	// do picking on mouse press (GLUT_DOWN)
	// this could be more elaborate, e.g. only performing picking when there is a click (DOWN followed by UP) on the same place
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		performPicking(x,y);
}

void GUI::performPicking(int x, int y) 
{
	// Sets the buffer to be used for selection and activate selection mode
	glSelectBuffer (BUFSIZE, selectBuf);
	glRenderMode(GL_SELECT);

	// Initialize the picking name stack
	glInitNames();

	// The process of picking manipulates the projection matrix
	// so we will be activating, saving and manipulating it
	glMatrixMode(GL_PROJECTION);

	//store current projmatrix to restore easily in the end with a pop
	glPushMatrix ();

	//get the actual projection matrix values on an array of our own to multiply with pick matrix later
	GLfloat projmat[16];
	glGetFloatv(GL_PROJECTION_MATRIX,projmat);

	// reset projection matrix
	glLoadIdentity();

	// get current viewport and use it as reference for 
	// setting a small picking window of 5x5 pixels around mouse coordinates for picking
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// this is multiplied in the projection matrix
	gluPickMatrix ((GLdouble) x, (GLdouble) (CGFapplication::height - y), 5.0, 5.0, viewport);

	// multiply the projection matrix stored in our array to ensure same conditions as in normal render
	glMultMatrixf(projmat);

	// force scene drawing under this mode
	// only the names of objects that fall in the 5x5 window will actually be stored in the buffer
	scene->display();

	// restore original projection matrix
	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();

	glFlush();

	// revert to render mode, get the picking results and process them
	GLint hits;
	hits = glRenderMode(GL_RENDER);
	processHits(hits, selectBuf);
}

void GUI::processHits (GLint hits, GLuint buffer[]) 
{
	GLuint *ptr = buffer;
	GLuint mindepth = 0xFFFFFFFF;
	GLuint *selected=NULL;
	GLuint nselected;

	// iterate over the list of hits, and choosing the one closer to the viewer (lower depth)
	for (int i=0;i<hits;i++) {
		int num = *ptr; ptr++;
		GLuint z1 = *ptr; ptr++;
		ptr++;
		if (z1 < mindepth && num>0) {
			mindepth = z1;
			selected = ptr;
			nselected=num;
		}
		for (int j=0; j < num; j++) 
			ptr++;
	}
	
	// if there were hits, the one selected is in "selected", and it consist of nselected "names" (integer ID's)
	if (selected!=NULL)
	{
		// this should be replaced by code handling the picked object's ID's (stored in "selected"), 
		// possibly invoking a method on the scene class and passing "selected" and "nselected"
		printf("Picked ID's: ");
		for (int i=0; i<nselected; i++)
			printf("%d ",selected[i]);
		printf("\n");
	}
	else
		printf("Nothing selected while picking \n");	
}
