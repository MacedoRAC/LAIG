#include"Camera.h"
#include"CGFapplication.h"

void Orthogonal::apply(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho( left,right,bottom, top, near, far);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	switch(direction)
	{
	case 'z':
		//default position
		break;
	case 'y':
		glRotated(90,1,0,0);
		break;
	case 'x':
		glRotated(-90,0,1,0);
		break;
	}
}

void Perspective::apply()
{
	float ratio = ((float) CGFapplication::width)/((float) CGFapplication::height);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angle, ratio,near, far);
	glPopMatrix();
	gluLookAt(pos[0],pos[1],pos[2],target[0],target[1],target[2],0,1,0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}