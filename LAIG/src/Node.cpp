#include"Node.h"
#include"CGFapplication.h"

void Node::draw(){

	glPushMatrix();
	glMultMatrixf(matrix);

	if(app)
		app->app->apply();

	vector<Primitive*> p=primitives;
	

	for(vector<Primitive*>::iterator pIt = p.begin(); pIt < p.end();pIt++){
		if(app)
			if(app->texture)
				if(dynamic_cast<Triangle*>((*pIt))){
					dynamic_cast<Triangle*>((*pIt))->draw(app->texture);
				}
				else if(dynamic_cast<Rectangle*>((*pIt))){
					dynamic_cast<Rectangle*>((*pIt))->draw(app->texture);
				}
				else
					(*pIt)->draw();
			else
				(*pIt)->draw();
		else
			(*pIt)->draw();
	}

	vector<Node*> nV = descendantNode;
	for(vector<Node*>::iterator nIt = nV.begin(); nIt != nV.end();nIt++)
		(*nIt)->draw();
	glPopMatrix();
}