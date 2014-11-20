#include<GL/glew.h>
#include"Primitive.h"
#include"CGFobject.h"
#include<math.h>
#include <cstdlib>

Primitive:: Primitive(string type){
	this->type=type;
}

//TORUS
Torus:: Torus(string type, int slices, int loops, bool smooth, float inner, float outer): Primitive(type){
	this->slices=slices;
	this->loops=loops;
	this->smooth=smooth;
	this->inner=inner;
	this->outer=outer;
}

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


//SPHERE
Sphere:: Sphere(string type, float radius, int slices, int stacks): Primitive(type){
	this->radius=radius;
	this->slices=slices;
	this->stacks=stacks;
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


//CYLINDER
Cylinder:: Cylinder(string type, float base, float top, float height, int slices, int stacks): Primitive(type){
	this->base=base;
	this->top=top;
	this->height=height;
	this->slices=slices;
	this->stacks=stacks;
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


//TRIANGLE
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


//RECTANGLE
Rectangle:: Rectangle(string type, float x1, float x2, float y1, float y2): Primitive(type){
	this->x1=x1;
	this->x2=x2;
	this->y1=y1;
	this->y2=y2;
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


//PLANE

//inicialização dos interpoladores
const GLfloat Plane::controlPoints[4][3] = {{ 0.5, 0.0, -0.5}, 
											{ 0.5, 0.0, 0.5},
											{-0.5, 0.0, -0.5}, 
											{-0.5, 0.0, 0.5}};

const GLfloat Plane::normalComponents[4][3] = {{ 0.0, 1.0, 0.0},
											   { 0.0, 1.0, 0.0}, 
											   { 0.0, 1.0, 0.0},
											   { 0.0, 1.0, 0.0}};

const GLfloat Plane::texturePoints[4][2] = {{ 0.0, 0.0},
											{ 0.0, 1.0}, 
											{ 1.0, 0.0},
											{ 1.0, 1.0} };


Plane:: Plane(string type, int parts): Primitive(type){
	this->parts=parts;
}

void Plane:: draw(){
	
	glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 2, 0.0, 1.0, 3*2, 2,  &controlPoints[0][0]);
	
	glMap2f(GL_MAP2_NORMAL,   0.0, 1.0, 3, 2, 0.0, 1.0, 3*2, 2,  &normalComponents[0][0]);
	
	glMap2f(GL_MAP2_TEXTURE_COORD_2,  0.0, 1.0, 2, 2, 0.0, 1.0, 2*2, 2,  &texturePoints[0][0]);

	// ativar os interpoladores
	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_MAP2_NORMAL);
	glEnable(GL_MAP2_TEXTURE_COORD_2);

	//fazer a grelha de acordo com o parametro 'parts'
	glMapGrid2f(parts, 0.0, 1.0, parts, 0.0, 1.0); 


	glEvalMesh2(GL_FILL, 0, parts, 0, parts);//modo de desenho pode ser GL_POINT, GL_LINE, GL_FILL
	
	glDisable(GL_AUTO_NORMAL);
}

void Plane:: draw(Texture* text){
	glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 2, 0.0, 1.0, 3*2, 2,  &controlPoints[0][0]);
	
	glMap2f(GL_MAP2_NORMAL,   0.0, 1.0, 3, 2, 0.0, 1.0, 3*2, 2,  &normalComponents[0][0]);
	
	glMap2f(GL_MAP2_TEXTURE_COORD_2,  0.0, 1.0, 2, 2, 0.0, 1.0, 2*2, 2,  &texturePoints[0][0]);

	// ativar os interpoladores
	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_MAP2_NORMAL);
	glEnable(GL_MAP2_TEXTURE_COORD_2);

	//fazer a grelha de acordo com o parametro 'parts'
	glMapGrid2f(parts, 0.0, 1.0, parts, 0.0, 1.0); 


	glEvalMesh2(GL_FILL, 0, parts, 0, parts);//modo de desenho pode ser GL_POINT, GL_LINE, GL_FILL
	
	glDisable(GL_AUTO_NORMAL);
}


//PATCH
Patch::Patch(string type, int order, int partsU, int partsV, string compute, GLfloat* cPoints): Primitive(type){
	this->order=order;
	this->partsU=partsU;
	this->partsV=partsV;
	this->compute=compute;

	int numberOfPoints = (order+1)*(order+1); //curvas de bezier
    this->controlPoints = new GLfloat[numberOfPoints*3];

    for(int i = 0; i < numberOfPoints; i++) {
        this->controlPoints[i*3+0] = cPoints[i*3+0];
        this->controlPoints[i*3+1] = cPoints[i*3+1];
        this->controlPoints[i*3+2] = cPoints[i*3+2];
    }
}

void Patch::draw(Texture * text){
	
	float s = 1;
	float t = 1;

	GLint fc[1];
	glGetIntegerv(GL_FRONT_FACE, fc);
    glFrontFace(GL_CW);
    glEnable(GL_AUTO_NORMAL);
    
    glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, order+1, 0.0, 1.0, 3*(order+1), order+1,  &controlPoints[0]);

	GLfloat* texture;

   if(order==1){
        GLfloat temp[4][2] = {{0.0, t}, {s,t}, 
							  {0.0, 0.0}, {s, 0.0}};
		texture = *temp;
    }else if(order==2){
        GLfloat temp[9][2] = {	{0.0,t},	{s/2, t},	{s,t},
								{0.0,t/2},	{s/2, t/2},	{s, t/2},
								{0.0,0.0},	{s/2,0.0},	{s,0.0}};
		texture = *temp;
    }else if(order==3){
        GLfloat temp[16][2] = {	{0.0,t},	{s/3, t},	{2*s/3,t},		{s,t},
								{0.0,2*t/3},{s/3,2*t/3},{2*s/3,2*t/3},	{s,2*t/3},
								{0.0,t/3},	{s/3,t/3},	{2*s/3,t/3},	{s,t/3},
								{0.0,0.0},	{s/3,0.0},	{2*s/3,0.0},	{s,0.0}};
		texture = *temp;
    }

	glMap2f(GL_MAP2_TEXTURE_COORD_2, 0.0, 1.0, 2, (order+1), 0.0, 1.0, (order+1)*2, (order+1), &texture[0]);

    // activar os interpoladores:
    glEnable(GL_MAP2_VERTEX_3);
    glEnable(GL_MAP2_NORMAL);
    glEnable(GL_MAP2_TEXTURE_COORD_2);
	
	//desenha a grelha de acordo com 'partsU' e 'partsV' respectivamente
    glMapGrid2f(partsU, 0.0, 1.0, partsV, 0.0, 1.0);
    
    glShadeModel(GL_SMOOTH); // tipo de shades pode ser: GL_FLAT, GL_SMOOTH

    if(compute == "fill")
        glEvalMesh2(GL_FILL, 0, partsU, 0, partsV);
    else if(compute == "point")
        glEvalMesh2(GL_POINT, 0, partsU, 0, partsV);
    else if(compute == "line")
        glEvalMesh2(GL_LINE, 0, partsU, 0, partsV);


    glDisable(GL_AUTO_NORMAL);
    glFrontFace(fc[0]);
}

void Patch::draw(){
	float s = 1;
	float t = 1;

	GLint fc[1];
	glGetIntegerv(GL_FRONT_FACE, fc);
    glFrontFace(GL_CW);
    glEnable(GL_AUTO_NORMAL);
    
    glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, order+1, 0.0, 1.0, 3*(order+1), order+1,  &controlPoints[0]);

	GLfloat* texture;

   if(order==1){
        GLfloat temp[4][2] = {{0.0, t}, {s,t}, 
							  {0.0, 0.0}, {s, 0.0}};
		texture = *temp;
    }else if(order==2){
        GLfloat temp[9][2] = {	{0.0,t},	{s/2, t},	{s,t},
								{0.0,t/2},	{s/2, t/2},	{s, t/2},
								{0.0,0.0},	{s/2,0.0},	{s,0.0}};
		texture = *temp;
    }else if(order==3){
        GLfloat temp[16][2] = {	{0.0,t},	{s/3, t},	{2*s/3,t},		{s,t},
								{0.0,2*t/3},{s/3,2*t/3},{2*s/3,2*t/3},	{s,2*t/3},
								{0.0,t/3},	{s/3,t/3},	{2*s/3,t/3},	{s,t/3},
								{0.0,0.0},	{s/3,0.0},	{2*s/3,0.0},	{s,0.0}};
		texture = *temp;
    }

	glMap2f(GL_MAP2_TEXTURE_COORD_2, 0.0, 1.0, 2, (order+1), 0.0, 1.0, (order+1)*2, (order+1), &texture[0]);

    // activar os interpoladores:
    glEnable(GL_MAP2_VERTEX_3);
    glEnable(GL_MAP2_NORMAL);
    glEnable(GL_MAP2_TEXTURE_COORD_2);
	
	//desenha a grelha de acordo com 'partsU' e 'partsV' respectivamente
    glMapGrid2f(partsU, 0.0, 1.0, partsV, 0.0, 1.0);
    
    glShadeModel(GL_SMOOTH); // tipo de shades pode ser: GL_FLAT, GL_SMOOTH

    if(compute == "fill")
        glEvalMesh2(GL_FILL, 0, partsU, 0, partsV);
    else if(compute == "point")
        glEvalMesh2(GL_POINT, 0, partsU, 0, partsV);
    else if(compute == "line")
        glEvalMesh2(GL_LINE, 0, partsU, 0, partsV);


    glDisable(GL_AUTO_NORMAL);
    glFrontFace(fc[0]);
}

//VEHICLE
//incialização dos pontos de controlo
GLfloat Vehicle::controlPoints[16][3] = {{ -0.5,  0,  -0.5},
										 { -0.5,  0,  -0.25},
										 { -0.5,  0,  0.25},
										 { -0.5,  0,  0.5},
										 { -0.25, 0,  -0.5}, 
										 { -0.25, 1,  -0.25},
										 { -0.25, 1,  0.25},
										 { -0.25, 0,  0.5},
										 { 0.25,  0,  -0.5},
										 { 0.25,  1,  -0.25},
										 { 0.25,  1,  0.25},
										 { 0.25,  0,  0.5},
										 { 0.5,   0,  -0.5},
										 { 0.5,   0,  -0.25},
										 { 0.5,   0,  0.25},
										 { 0.5,   0,  0.5}};


Vehicle::Vehicle(string type):Primitive(type){

	patch = new Patch("patch", 3, 10, 10, "fill", controlPoints[0]);
}

void Vehicle::draw(Texture * text){
	
	// Top
	glPushMatrix();
		glTranslated(0,0.5,0);
		patch->draw(text);
	glPopMatrix();

	// Bottom
	glPushMatrix();
		glTranslated(0,-0.5,0);
		glRotated(180, 1, 0, 0);
		patch->draw(text);
	glPopMatrix();

	// Left
	glPushMatrix();
		glTranslated(-0.5,0,0);
		glRotated(90, 0, 0, 1);
		patch->draw(text);
	glPopMatrix();

	// Right
	glPushMatrix();
		glTranslated(0.5,0,0);
		glRotated(-90, 0, 0, 1);
		patch->draw(text);
	glPopMatrix();

	// Back
	glPushMatrix();
		glTranslated(0,0,-0.5);
		glRotated(-90, 1, 0, 0);
		patch->draw(text);
	glPopMatrix();

	// Front
	glPushMatrix();
		glTranslated(0,0,0.5);
		glRotated(90, 1, 0, 0);
		patch->draw(text);
	glPopMatrix();

}


//FLAG
Flag::Flag(string type, string texture): Plane(type, 30){
	this->elapsed=0;
	
	this->texture=texture;

	init("flag.vert", "flag.frag");

	CGFshader::bind();

	text = new CGFtexture(texture);
	timer = glGetUniformLocation(id(), "timer");
	wind_Loc = glGetUniformLocation(id(), "wind");
	
	texture_Loc = glGetUniformLocation(id(), "text");
	glUniform1i(texture_Loc, 0);
}

void Flag::draw(){
	bind();
	Plane::draw();
	CGFshader::unbind();
}

void Flag::draw(Texture * text){
	bind();
	Plane::draw(text);
	CGFshader::unbind();
}

void Flag::bind(){
	CGFshader::bind();

	glUniform1f(timer, ((float)elapsed)/1000.0);
	glUniform1f(wind_Loc, wind);

	glActiveTexture(GL_TEXTURE0);

	text->apply();
}

void Flag::update(unsigned long time, int wind){
	if(elapsed==0){
		startTime = time;
		elapsed = startTime;
	}else
		elapsed = (time - startTime);

	this->wind = wind;
}