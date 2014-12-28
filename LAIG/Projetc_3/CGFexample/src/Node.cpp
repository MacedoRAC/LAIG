#include"Node.h"
#include"CGFapplication.h"

Node::Node(){

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	glPopMatrix();

	this->app=NULL; 
	this->displayList=NULL; 
	this->flag=NULL; 
	this->animations=NULL;
}

void Node::draw(Appearance* appea){

	glPushMatrix();

	if(app)
		appea = app;

	if(appea)
		appea->app->apply();

	if(animations)
		animations->apply();

	
	// Display List

	if(displayList==NULL && hasDisplayList) {

		displayList = glGenLists(1);
		glNewList(displayList, GL_COMPILE);

		// Transformações
		glMultMatrixf(matrix);

		//Picking Names
		for(unsigned int i = 0; i < pickingNames.size(); i++) {
			glPushName(pickingNames[i]);
		}

		// Primitivas
		for (unsigned int i = 0; i < primitives.size(); i++){
			if(appea->textureref != ""){
				primitives[i]->draw(appea->texture);
			}else{
				primitives[i]->draw();
			}
		}

		//flag
		if(flag)
			flag->draw();

		// Nós filhos
		for (unsigned int i = 0; i < descendantNode.size(); i++)
			descendantNode[i]->draw(appea);

		for(unsigned int i = 0; i < pickingNames.size(); i++){
			glPopName();
		}

		glEndList();
		
	}else if(displayList!=NULL && hasDisplayList) {
			glCallList(displayList);
	
	}else{ //não tem displaylists

		// Transformações
		glMultMatrixf(matrix);

		//Picking names
		for(unsigned int i = 0; i < pickingNames.size(); i++) {
			glPushName(pickingNames[i]);
		}

		// Primitivas
		for (unsigned int i = 0; i < primitives.size(); i++){
				if(appea->textureref != ""){
					primitives[i]->draw(appea->texture);
				}else{
					primitives[i]->draw();
				}
		}

		//flag
		if(flag)
			flag->draw();

		// Nós filhos 
		for (unsigned int i = 0; i < descendantNode.size(); i++)
			descendantNode[i]->draw(appea);

		for(unsigned int i = 0; i < pickingNames.size(); i++){
			glPopName();
		}
	}

	glPopMatrix();

}

void Node::update(unsigned long milis) {
	if(animations)
		animations->update(milis);

	if(flag)
		flag->update(milis, wind);

	for (unsigned int i = 0; i < descendantNode.size(); i++){
			descendantNode[i]->update(milis);
	}
}