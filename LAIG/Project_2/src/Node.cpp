#include"Node.h"
#include"CGFapplication.h"

void Node::draw(Appearance* appea){

	glPushMatrix();
	glMultMatrixf(matrix);

	if(app)
		appea = app;
	
	if(appea)
		appea->app->apply();

	vector<Primitive*> p=primitives;
	

	for(vector<Primitive*>::iterator pIt = p.begin(); pIt < p.end(); pIt++){
			if(appea->textureref != ""){
				(*pIt)->draw(appea->texture);
			}
			else{
				(*pIt)->draw();
			}
	}

	vector<Node*> nV = descendantNode;
	for(vector<Node*>::iterator nIt = nV.begin(); nIt != nV.end();nIt++)
		(*nIt)->draw(appea);
	
	
	glPopMatrix();
}

void Node::createDisplayList(Appearance* appea){

	if(app)
		appea = app;
	
	if(appea)
		appea->app->apply();

	vector<Node*> descNodes = descendantNode;

	for(vector<Node*>::iterator nIt = descNodes.begin(); nIt != descNodes.end();nIt++)
			(*nIt)->createDisplayList(appea);


	if(hasDisplayList)
	{
		displayList = glGenLists(1);
		hasDisplayList = false;
		glNewList(displayList,GL_COMPILE);
		draw(appea);
		glEndList();
		hasDisplayList = true;
	}
}