#ifndef GUI_H
#define GUI_H

#include "CGFinterface.h"

class GUI: public CGFinterface {
	public:
		GUI();
		
		virtual void initGUI();
		virtual void processGUI(GLUI_Control *control);


};


#endif