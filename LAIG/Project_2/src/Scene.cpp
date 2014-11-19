#include"Scene.h"
#include"CGFaxis.h"
#include"Camera.h"
#include <math.h>
#include"CGFapplication.h"
#include "graph.h"

float pi = acos(-1.0);
float deg2rad=pi/180.0;


float ambientNull[4]={0,0,0,1};


unsigned int lightsArray[8] = {GL_LIGHT0,GL_LIGHT1,GL_LIGHT2,GL_LIGHT3,GL_LIGHT4,GL_LIGHT5,GL_LIGHT6,GL_LIGHT7};

void Scene::init(){

	glClearColor(graph->background[0], graph->background[1], graph->background[2], graph->background[3]);

	//inicializar o vector de cameras da Scene
	map<string, Camera*>::iterator it;
	for(it=graph->cameras.begin(); it != graph->cameras.end(); it++){
		cameras.push_back((*it).second);
	}


	camIt = cameras.begin();
	for(int i = 0; camIt != cameras.end(); i++,camIt++) 
	{
		if((*camIt)->id == graph->rootCamera)
		{
			actualCam = i;
			break;
		}
	}

	glEnable(GL_NORMALIZE);

	if(graph->cullingFace == "none")
		glCullFace(GL_NONE);
	else if(graph->cullingFace == "back")
		glCullFace(GL_BACK);
	else  if(graph->cullingFace == "front")
		glCullFace(GL_FRONT);
	else  if(graph->cullingFace == "both")
		glCullFace(GL_FRONT_AND_BACK);

	if(graph->cullingOrder == "ccw")
		glFrontFace(GL_CCW);
	else if(graph->cullingOrder == "cw")
		glFrontFace(GL_CW);

	//lighting
	if(graph->lEnabled)
		glEnable(GL_LIGHTING);

	if(graph->lDoubleSided){
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);  
	}else{
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE); 
	}

	if(graph->lLocal)
		glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
	else{
		glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_FALSE);
	}

	//ambient light


	float ambientLight[4] = {0,0,0,0};
	for(int i = 0; i < 4;i++)
		ambientLight[i] = graph->lAmbient[i];

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);  


	if(graph->shading == "gouraud")
		glShadeModel(GL_SMOOTH);
	else if(graph->shading == "flat")
		glShadeModel(GL_FLAT);

	if(graph->drawingMode == "fill")
		wire = 0;
	else if(graph->drawingMode == "line")
		wire = 1;
	else if(graph->drawingMode == "point")
		wire = 2;

	vector<Light> lghts = graph->lights;
	int i = 0;
	vector<Light>::iterator lIt;
	for(lIt = lghts.begin(); lIt != lghts.end(); lIt++,i++){

		float pos[4] = {lIt->pos[0], lIt->pos[1], lIt->pos[2], 1.0};
		float ambient[4] = {lIt->ambient[0], lIt->ambient[1], lIt->ambient[2], lIt->ambient[3]};
		float diffuse[4] =  {lIt->diffuse[0], lIt->diffuse[1], lIt->diffuse[2], lIt->diffuse   [3]};
		float specular[4] =  {lIt->specular[0], lIt->specular[1], lIt->specular[2], lIt->specular[3]};
		CGFlight* newLight;

		if( lIt->type == "spot")
		{

			float angle = lIt->angle, exp = lIt->exponent;
			float target[3]={lIt->target[0]-pos[0], lIt->target[1]-pos[1], lIt->target[2]-pos[2]};
			float unit = sqrt(target[0] * target[0] + target[1] * target[1] + target[2] * target[2]);
			for (int i = 0; i < 3; i++) 
				target[i] = target[i] / unit;
			
			glLightf(lightsArray[i],GL_SPOT_EXPONENT,exp);
			glLightf(lightsArray[i],GL_SPOT_CUTOFF,angle);
			glLightfv(lightsArray[i],GL_SPOT_DIRECTION,target);	
			newLight = new CGFlight(lightsArray[i],pos,target);
		}
		else
			newLight =new CGFlight(lightsArray[i],pos);
		
		newLight->setAmbient(ambient);
		newLight->setDiffuse(diffuse);
		newLight->setSpecular(specular);

		if(lIt->enabled)
			newLight->enable();
		else
			newLight->disable();

		newLight->update();

		lights.push_back(newLight);
	}

	setUpdatePeriod(1);

}

void Scene::display(){

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	switch(wire)
	{
	case 0:	
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 1:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 2:		
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	}


	camIt = cameras.begin() + actualCam;
	if(camIt != cameras.end()){
		(*camIt)->applyView();
		(*camIt)->apply();
	}else{
		CGFscene::activeCamera->applyView();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}


	CGFapplication::activeApp->forceRefresh();

	// ---- BEGIN Background, camera and axis setup

	// Clear image and depth buffer everytime we update the scene


	// Initialize Model-View matrix as identity (no transformation

	// Apply transformations corresponding to the camera position relative to the origin

	vector<Light> lghts = graph->lights;
	vector<Light>::iterator lghtsIt = lghts.begin();

	for(int i =0; lghtsIt != lghts.end(); lghtsIt++,i++)
	{
		if(lghtsIt->marker)
			lights[i]->draw();
		if(lghtsIt->enabled)
			glEnable(lightsArray[i]);
		else
			glDisable(lightsArray[i]);
		
		lights[i]->update();
	}
	
	// Draw axis
	axis.draw();
	
	Appearance * app = NULL;
	graph->rootNode->draw(app);
	
	glutSwapBuffers();
}


void Scene::update(unsigned long milis){
	map<string, Node>::iterator it;
	
	for(it = graph->nodes.begin(); it != graph->nodes.end(); it++)
		it->second.update(milis);
}