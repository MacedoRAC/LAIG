#include"GUI.h"
#include"CGFaxis.h"
#include"Camera.h"
#include <math.h>
#include"CGFapplication.h"

float pi = acos(-1.0);
float deg2rad=pi/180.0;


float ambientNull[4]={0,0,0,1};


unsigned int lightsArray[8] = {GL_LIGHT0,GL_LIGHT1,GL_LIGHT2,GL_LIGHT3,GL_LIGHT4,GL_LIGHT5,GL_LIGHT6,GL_LIGHT7};

void GUI::init() 
{
	//Setting up globals

	//background
	glClearColor(graph.background[0], graph.background[1], graph.background[2], graph.background[3]);

	glEnable(GL_NORMALIZE);

	//Culling
	if(graph.cullingFace == "none")
		glCullFace(GL_NONE);
	else if(graph.cullingFace == "back")
		glCullFace(GL_BACK);
	else  if(graph.cullingFace == "front")
		glCullFace(GL_FRONT);
	else  if(graph.cullingFace == "both")
		glCullFace(GL_FRONT_AND_BACK);

	if(graph.cullingOrder == "ccw")
		glFrontFace(GL_CCW);
	else if(graph.cullingOrder == "cw")
		glFrontFace(GL_CW);

	//Light
	if(graph.lEnabled)
		glEnable(GL_LIGHTING);

	if(graph.lDoubleSided)
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);  
	else
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE); 

	if(graph.lLocal)
		glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
	else
		glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_FALSE);
	
	//Ambient
	float ambientLight[4] = {0,0,0,0};
	for(int i = 0; i < 4;i++)
		ambientLight[i] = graph.lAmbient[i];

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);  

	//Drawing
	if(graph.shading == "gouraud")
		glShadeModel(GL_SMOOTH);
	else if(graph.shading == "flat")
		glShadeModel(GL_FLAT);


	if(graph.drawingMode == "fill")
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else if(graph.drawingMode == "line")
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if(graph.drawingMode == "point")
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);


	for(unsigned int i = 0; i < graph.lights.size();i++){
		Light temp = graph.lights[i];

		float pos[4] = {temp.pos[0],temp.pos[1],temp.pos[2],1.0};
		float ambient[4] = {temp.ambient[0],temp.ambient[i],temp.ambient[2],temp.ambient[3]};
		float diffuse[4] =  {temp.diffuse[0],temp.diffuse[i],temp.diffuse[2],temp.diffuse[3]};
		float specular[4] =  {temp.specular[0],temp.specular[i],temp.specular[2],temp.specular[3]};
		
		
		float spotDirection[3]={0,0,0};

		if( temp.type == "spot"){

			float angle = temp.angle, exponent = temp.exponent;
			float target[3]={temp.target[0] - pos[0],temp.target[1]- pos[1],temp.target[2]- pos[2]};
			float unit = sqrt(target[0] * target[0] + target[1] * target[1] + target[2] * target[2]);
			for (int i = 0; i < 3; i++) {
				target[i] = target[i] / unit;
				spotDirection[i] = target[i];
			}
			glLightf(lightsArray[i],GL_SPOT_EXPONENT,exponent);
			glLightf(lightsArray[i],GL_SPOT_CUTOFF,angle);
			glLightfv(lightsArray[i],GL_SPOT_DIRECTION,target);

		}

		CGFlight* light =new CGFlight(lightsArray[i],pos,spotDirection);
		light->setAmbient(ambient);
		light->setDiffuse(diffuse);
		light->setSpecular(specular);

		if(temp.enabled)
			light->enable();
		else
			light->disable();

		light->update();

		lights.push_back(light);
	}
}

void GUI::display() 
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	map<string,Camera *> copyCameras = graph.cameras;
	map<string,Camera *>::iterator it = copyCameras.find(graph.rootCamera);
	
	it->second->apply();
	it->second->applyView();
	
	CGFapplication::activeApp->forceRefresh();
	

	CGFscene::activeCamera->applyView();

	for(unsigned int i = 0; i < graph.lights.size();i++)
	{
		if(graph.lights[i].marker)
			lights[i]->draw();

		lights[i]->update();

	}

	axis.draw();

	drawNode(graph.rootNode);


	glutSwapBuffers();
}


void GUI::update(unsigned long millis){}

GUI::~GUI(){}


void GUI::drawNode(Node *node){

	float m[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


	for(int i = 0; i < 4; i++){
		for(int j = 0; j<4; j++){
			m[i*4+j] = node->matrix[i][j];
		}
	}

	glPushMatrix();
	glMultMatrixf(m);
	if(node->appRef != "inherit"){
		node->app->app->setTexture( node->app->texture->file.c_str());
		node->app->app->apply();
	}


	for(unsigned int i = 0; i < node->rectangles.size();i++)
		node->rectangles[i].draw();

	for(unsigned int i = 0; i < node->triangles.size();i++)
		node->triangles[i].draw();

	for(unsigned int i = 0; i < node->cylinders.size();i++)
		node->cylinders[i].draw();

	for(unsigned int i = 0; i < node->spheres.size();i++)
		node->spheres[i].draw();

	for(unsigned int i = 0; i < node->torusses.size();i++)
		node->torusses[i].draw();

	for(unsigned int i = 0; i < node->descendants.size();i++)
		drawNode(node->descendantNode[i]);

	glPopMatrix();


}
