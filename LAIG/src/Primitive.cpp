#include"Primitive.h"
#include"CGFobject.h"
#include<math.h>

void Torus::draw(){
	double pi = acos(-1.0);
	float vNormal[3];   
    double majorStep = 2.0f*pi / slices;   
    double minorStep = 2.0f*pi / loops;   
    int i, j;   
   
    for (i=0; i<slices; ++i)    
            {   
            double a0 = i * majorStep;   
            double a1 = a0 + majorStep;   
            GLfloat x0 = (GLfloat) cos(a0);   
            GLfloat y0 = (GLfloat) sin(a0);   
            GLfloat x1 = (GLfloat) cos(a1);   
            GLfloat y1 = (GLfloat) sin(a1);   
   
            glBegin(GL_TRIANGLE_STRIP);   
            for (j=0; j<=loops; ++j)    
                    {   
                    double b = j * minorStep;   
                    GLfloat c = (GLfloat) cos(b);   
                    GLfloat r = inner * c + outer;   
                    GLfloat z = inner * (GLfloat) sin(b);   
   
                    // First point   
                    glTexCoord2f((float)(i)/(float)(loops), (float)(j)/(float)(slices));   
                    vNormal[0] = x0*c;   
                    vNormal[1] = y0*c;   
                    vNormal[2] = z/inner;   
                    //gltNormalizeVector(vNormal);   
                    glNormal3fv(vNormal);   
                    glVertex3f(x0*r, y0*r, z);   
   
                    glTexCoord2f((float)(i+1)/(float)(loops), (float)(j)/(float)(slices));   
                    vNormal[0] = x1*c;   
                    vNormal[1] = y1*c;   
                    vNormal[2] = z/inner;   
					glNormal3f(vNormal[0],vNormal[1],vNormal[2]);   
                    glVertex3f(x1*r, y1*r, z);   
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

void Rectangle:: draw(){

	glRectd(x1, y1, x2, y2);

}

void Triangle:: draw(){

	glBegin(GL_TRIANGLES);
	//glTexCoord2f(0,0);
	glVertex3f(x1, y1, z1);
	//glTexCoord2f(t->getTexLengthS(), t->getTexLengthT());
	glVertex3f(x2, y2, z2);
	//glTexCoord2f(,0);
	glVertex3f(x3, y3, z3);
	glEnd();
}