#include"Node.h"
#include"CGFapplication.h"

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

		// Transforma��es
		glMultMatrixf(matrix);

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

		// N�s filhos
		for (unsigned int i = 0; i < descendantNode.size(); i++)
			descendantNode[i]->draw(appea);

		glEndList();
		
	}else if(displayList!=NULL && hasDisplayList) {
			glCallList(displayList);
	
	}else{ //n�o tem displaylists

		// Transforma��es
		glMultMatrixf(matrix);

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

		// N�s filhos 
		for (unsigned int i = 0; i < descendantNode.size(); i++)
			descendantNode[i]->draw(appea);
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