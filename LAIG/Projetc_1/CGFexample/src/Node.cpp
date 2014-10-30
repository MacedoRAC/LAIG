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
		(*pIt)->draw();
	}

	vector<Node*> nV = descendantNode;
	for(vector<Node*>::iterator nIt = nV.begin(); nIt != nV.end();nIt++)
		(*nIt)->draw(appea);
	
	
	glPopMatrix();
}