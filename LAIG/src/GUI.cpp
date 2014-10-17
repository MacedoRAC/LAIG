#include "GUI.h"
#include "XMLScene.h"

GUI::GUI()
{
}
void GUI::initGUI()
{
	GLUI_Panel *geral =addPanel("Options", 1);
	addColumnToPanel(geral);
	GLUI_Panel *luzesPanel = addPanelToPanel(geral,"Enable Lights", 1);

	for(unsigned int i=0;i<((XMLScene *) scene)->parser.lights.size();i++){

		string str=((XMLScene *) scene)->parser.lights[i]->id;
		char * writable = new char[str.size() + 1];
		copy(str.begin(), str.end(), writable);
		writable[str.size()] = '\0';


		if(((XMLScene *) scene)->parser.lights[i]->enabled==true){
			addCheckboxToPanel(luzesPanel,writable,NULL,i)->set_int_val(1);
		}
		else{
			addCheckboxToPanel(luzesPanel,writable,NULL,i)->set_int_val(0);
		}
		delete[] writable;

	}
}

void GUI::processGUI(GLUI_Control *control)
{
	if( control->get_int_val() == 1)
		((XMLScene *) scene)->switchLight(control->user_id,true);
	else 
		((XMLScene *) scene)->switchLight(control->user_id,false);


}