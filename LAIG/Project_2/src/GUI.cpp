#include"GUI.h"
#include"Scene.h"

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