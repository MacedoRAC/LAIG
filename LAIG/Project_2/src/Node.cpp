#include"Node.h"
#include"CGFapplication.h"

void Node::draw(Appearance* appea){

	glPushMatrix();

	if(app)
		appea = app;

	if(appea)
		appea->app->apply();

	
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

		// N�s filhos
		for (unsigned int i = 0; i < descendantNode.size(); i++)
			descendantNode[i]->draw(appea);

		glEndList();
		
	}else if(displayList!=NULL && hasDisplayList) {
			glCallList(displayList);
	
	}else{ //n�o tem displaylists

		// Transforma��es
		glMultMatrixf(matrix);

		if(animation)
			animation->apply();

		// Primitivas
		for (unsigned int i = 0; i < primitives.size(); i++){
				if(appea->textureref != ""){
					primitives[i]->draw(appea->texture);
				}else{
					primitives[i]->draw();
				}
			}

		// N�s filhos 
		for (unsigned int i = 0; i < descendantNode.size(); i++)
			descendantNode[i]->draw(appea);
	}

	glPopMatrix();

}

void Node::update(unsigned long milis) {
	if(animation)
		animation->update(milis);

	for (int i = 0; i < primitives.size(); i++){
			primitives[i]->update(milis);
	}
}