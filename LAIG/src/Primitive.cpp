#include"Primitive.h"
#include"CGFobject.h"
#include<math.h>

void Torus::draw()
{
	double pi = acos(-1.0);
	float vectorNormal[3];
	double majorStep = 2.0f*pi / slices;
	double minorStep = 2.0f*pi / loops;
	vector<float> x, y, z, r, c;

	for (int i=0; i<slices; i++) //calcula e guarda todas as coordenadas e angulos para depois desenhar
	{
		double a0 = i * majorStep;
		x.push_back(cos(a0));
		y.push_back(sin(a0));
		for (int j=0; j<=loops; ++j){
			double b = j * minorStep;
			c.push_back(cos(b));
			r.push_back(inner * c[j] + outer);
			z.push_back(inner *sin(b));	
		}
	} 
	
	//desenha o torus
	for (int i=0; i<slices; ++i){
		glBegin(GL_TRIANGLE_STRIP);
		for (int j=0; j<=loops; ++j){
			glTexCoord2f((float)(i)/(loops), (float)(j)/(slices));
			vectorNormal[0] = x[i]*c[j];
			vectorNormal[1] = y[i]*c[j];
			vectorNormal[2] = z[j]/inner;
			glNormal3fv(vectorNormal);
			glVertex3f(x[i]*r[j], y[i]*r[j], z[j]);
			
			glTexCoord2f((float)(i+1)/(loops), (float)(j)/(slices));
			if(i+1 < slices){
				vectorNormal[0] = x[i+1]*c[j];
				vectorNormal[1] = y[i+1]*c[j];
				vectorNormal[2] = z[j]/inner;
				glNormal3f(vectorNormal[0],vectorNormal[1],vectorNormal[2]);
				glVertex3f(x[i+1]*r[j], y[i+1]*r[j], z[j]);
			}else{
				vectorNormal[0] = x[0]*c[j];
				vectorNormal[1] = y[0]*c[j];
				vectorNormal[2] = z[j]/inner;
				glNormal3f(vectorNormal[0],vectorNormal[1],vectorNormal[2]);
				glVertex3f(x[0]*r[j], y[0]*r[j], z[j]);
			}
		}
		glEnd();
	}
}

void Torus::draw(Texture* text){
	double pi = acos(-1.0);
	float vectorNormal[3];
	double majorStep = 2.0f*pi / slices;
	double minorStep = 2.0f*pi / loops;
	vector<float> x, y, z, r, c;

	for (int i=0; i<slices; i++) //calcula e guarda todas as coordenadas e angulos para depois desenhar
	{
		double a0 = i * majorStep;
		x.push_back(cos(a0));
		y.push_back(sin(a0));
		for (int j=0; j<=loops; ++j){
			double b = j * minorStep;
			c.push_back(cos(b));
			r.push_back(inner * c[j] + outer);
			z.push_back(inner *sin(b));	
		}
	} 
	
	//desenha o torus
	for (int i=0; i<slices; ++i){
		glBegin(GL_TRIANGLE_STRIP);
		for (int j=0; j<=loops; ++j){
			glTexCoord2f((float)(i)/(loops), (float)(j)/(slices));
			vectorNormal[0] = x[i]*c[j];
			vectorNormal[1] = y[i]*c[j];
			vectorNormal[2] = z[j]/inner;
			glNormal3fv(vectorNormal);
			glVertex3f(x[i]*r[j], y[i]*r[j], z[j]);
			
			glTexCoord2f((float)(i+1)/(loops), (float)(j)/(slices));
			if(i+1 < slices){
				vectorNormal[0] = x[i+1]*c[j];
				vectorNormal[1] = y[i+1]*c[j];
				vectorNormal[2] = z[j]/inner;
				glNormal3f(vectorNormal[0],vectorNormal[1],vectorNormal[2]);
				glVertex3f(x[i+1]*r[j], y[i+1]*r[j], z[j]);
			}else{
				vectorNormal[0] = x[0]*c[j];
				vectorNormal[1] = y[0]*c[j];
				vectorNormal[2] = z[j]/inner;
				glNormal3f(vectorNormal[0],vectorNormal[1],vectorNormal[2]);
				glVertex3f(x[0]*r[j], y[0]*r[j], z[j]);
			}
		}
		glEnd();
	}
}

void Sphere:: draw(){
	GLUquadric * quad;

	quad = gluNewQuadric();
	gluQuadricTexture(quad,GL_TRUE);
	gluQuadricDrawStyle(quad,GLU_FILL);
	gluQuadricNormals(quad,GLU_SMOOTH);
	gluQuadricOrientation(quad,GLU_OUTSIDE);

	gluSphere(quad,radius,slices,stacks);


	gluDeleteQuadric(quad);
}

void Sphere:: draw(Texture* text){
	GLUquadric * quad;

	quad = gluNewQuadric();
	gluQuadricTexture(quad,GL_TRUE);
	gluQuadricDrawStyle(quad,GLU_FILL);
	gluQuadricNormals(quad,GLU_SMOOTH);
	gluQuadricOrientation(quad,GLU_OUTSIDE);

	gluSphere(quad,radius,slices,stacks);


	gluDeleteQuadric(quad);
}

void Cylinder:: draw(){

	GLUquadric * quad, *disk1, *disk2;

	quad = gluNewQuadric();
	disk1 = gluNewQuadric();
	disk2 = gluNewQuadric();

	gluQuadricTexture(quad,GL_TRUE);
	gluQuadricTexture(disk1,GL_TRUE);
	gluQuadricTexture(disk2,GL_TRUE);

	gluQuadricDrawStyle(quad,GLU_FILL);
	gluQuadricNormals(quad,GLU_SMOOTH);
	gluQuadricOrientation(quad,GLU_OUTSIDE);

	gluCylinder(quad,base,top,height,slices,stacks);

	glPushMatrix();
	glTranslated(0,0,height);
	gluDisk(disk1,0,top,slices,stacks);
	glPopMatrix();

	glPushMatrix();
	glRotated(180,0,1,0);
	gluDisk(disk2,0,base,slices,stacks);
	glPopMatrix();

	gluDeleteQuadric(quad);
	gluDeleteQuadric(disk1);
}

void Cylinder:: draw(Texture* text){

	GLUquadric * quad, *disk1, *disk2;

	quad = gluNewQuadric();
	disk1 = gluNewQuadric();
	disk2 = gluNewQuadric();

	gluQuadricTexture(quad,GL_TRUE);
	gluQuadricTexture(disk1,GL_TRUE);
	gluQuadricTexture(disk2,GL_TRUE);

	gluQuadricDrawStyle(quad,GLU_FILL);
	gluQuadricNormals(quad,GLU_SMOOTH);
	gluQuadricOrientation(quad,GLU_OUTSIDE);

	gluCylinder(quad,base,top,height,slices,stacks);

	glPushMatrix();
	glTranslated(0,0,height);
	gluDisk(disk1,0,top,slices,stacks);
	glPopMatrix();

	glPushMatrix();
	glRotated(180,0,1,0);
	gluDisk(disk2,0,base,slices,stacks);
	glPopMatrix();

	gluDeleteQuadric(quad);
	gluDeleteQuadric(disk1);
}

void Triangle::draw(){

	glBegin(GL_TRIANGLES);
		glVertex3f(x1,y1,z1);
		glVertex3f(x2,y2,z2);
		glVertex3f(x3,y3,z3);
	glEnd();
}

void Triangle::draw(Texture * text)
{
	float width = text->texlength_S;
	float height = text->texlength_T;
	float lengthText = sqrt( (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));
	float angle = acos( (x3-x1)*(y2-y1)*(z3-z1)+(x2-x1)*(y3-y1)*(z2-z1));
	float heighText = sin( angle)*sqrt( (x1-x3)*(x1-x3)+(y1-y3)*(y1-y3)+(z1-z3)*(z1-z3));

	float deltaX = lengthText/width;
	float deltaY = heighText/height;

	glBegin(GL_TRIANGLES);
		glNormal3f(x1, y1, 1);
		glTexCoord2f(0,0);
		glVertex3f(x1,y1,z1);

		glNormal3f(x2, y2,1);
		glTexCoord2f(deltaX,0);
		glVertex3f(x2,y2,z2);

		glNormal3f(z3,y3,1);
		glTexCoord2f(deltaX,deltaY);
		glVertex3f(x3,y3,z3);
	glEnd();
}

void Rectangle::draw(){

	glBegin(GL_QUADS);
		glNormal3f(0,0,1);

		glTexCoord2f(0,0);
		glVertex2f(x1,y1);

		glTexCoord2f(1,0);
		glVertex2f(x2,y1);

		glTexCoord2f(1,1);
		glVertex2f(x2,y2);

		glTexCoord2f(0,1);
		glVertex2f(x1,y2);
	glEnd();
}

void Rectangle::draw(Texture * text){

	float width = text->texlength_S;
	float height = text->texlength_T;
	float lengthText = x2-x1;
	float heightText = y2-y1;
	float deltaX = lengthText / width;
	float deltaY = heightText / height;

	glBegin(GL_QUADS);
	glNormal3f(0,0,1);

	glTexCoord2f(0,0);
	glVertex2f(x1,y1);

	glTexCoord2f(0,deltaY);
	glVertex2f(x2,y1);

	glTexCoord2f(deltaX,deltaY);
	glVertex2f(x2,y2);

	glTexCoord2f(deltaX,0);
	glVertex2f(x1,y2);
	glEnd();
}

Primitive:: Primitive(string type){
	this->type=type;
}

Rectangle:: Rectangle(string type, float x1, float x2, float y1, float y2): Primitive(type){
	this->x1=x1;
	this->x2=x2;
	this->y1=y1;
	this->y2=y2;
}

Triangle:: Triangle(string type, float x1, float x2, float x3, float y1, float y2, float y3, float z1, float z2, float z3): Primitive(type){
	this->x1=x1;
	this->y1=y1;
	this->z1=z1;
	this->x2=x2;
	this->y2=y2;
	this->z2=z2;
	this->x3=x3;
	this->y3=y3;
	this->z3=z3;
}

Cylinder:: Cylinder(string type, float base, float top, float height, int slices, int stacks): Primitive(type){
	this->base=base;
	this->top=top;
	this->height=height;
	this->slices=slices;
	this->stacks=stacks;
}

Torus:: Torus(string type, int slices, int loops, bool smooth, float inner, float outer): Primitive(type){
	this->slices=slices;
	this->loops=loops;
	this->smooth=smooth;
	this->inner=inner;
	this->outer=outer;
}

Sphere:: Sphere(string type, float radius, int slices, int stacks): Primitive(type){
	this->radius=radius;
	this->slices=slices;
	this->stacks=stacks;
}