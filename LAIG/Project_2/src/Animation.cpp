#include "Animation.h"
#include<math.h>
#define PI 3.141592653589793

//ANIMATION
Animation::Animation(){
	this->id="";
	this->span=0.0;
	this->type="";
}

Animation::Animation(string id, float span, string type){
	this->id=id;
	this->span=span;
	this->type=type;
	this->reset=true;
}

void Animation::init(unsigned long time){

}

//LINEAR ANIMATION
LinearAnimation::LinearAnimation(string id, float span, string type, vector<vector<float>> ctrlPoints): Animation(id, span, type){
	this->controlPoints=ctrlPoints;
	this->distance = 0;

	//calcular a distantia do movimento
	for(unsigned int i = 1; i < ctrlPoints[0].size(); i++) {
		this->distance += sqrtf( pow(ctrlPoints[i][0]-ctrlPoints[i-1][0], 2) + 
								 pow(ctrlPoints[i][1]-ctrlPoints[i-1][1], 2) + 
								 pow(ctrlPoints[i][2]-ctrlPoints[i-1][2], 2));
	}

	reset = true;
}

void LinearAnimation::init(unsigned long time){
	
	if(controlPoints.size() > 0) {
		xTranslation = controlPoints[0][0];
		yTranslation = controlPoints[0][1];
		zTranslation = controlPoints[0][2];
	} else {
		xTranslation = yTranslation = zTranslation = 0;
	}

	startTime = time;
	controlTime = time;
	previousPoint = 0;
	reset = false;
	finished = false;
	this->repeat=true;
}

void LinearAnimation::apply(){
	
	if(!finished){
		glTranslatef(this->xTranslation, this->yTranslation, this->zTranslation);
		glRotated(rotationAngle, 0, 1, 0);
	}
}

void LinearAnimation::update(unsigned long time){

	if(reset) {
		init(time);
		return;
	}
	
	if(previousPoint < controlPoints.size()-1 && xTranslation == controlPoints[previousPoint+1][0]
												&& yTranslation == controlPoints[previousPoint+1][1]
												&& zTranslation == controlPoints[previousPoint+1][2]) {
		previousPoint++;
	}
		
	if(previousPoint == controlPoints.size()-1) {
		this->finished = true;
		printf("\n\n\n FINISHED \n");
		if(repeat)
			init(time);
		return;
	}

printf("\nPreviousPoint: %d\n TamanhoControlPoints: %d\n\n", previousPoint, controlPoints.size());
	
if(!finished){
		float deltaTime = (time - startTime) * 0.001;
		startTime = time;
		

		// Calcular vector do movimento
		float v[3];
		v[0] = controlPoints[previousPoint+1][0] - controlPoints[previousPoint][0]; //xx
		v[1] = controlPoints[previousPoint+1][1] - controlPoints[previousPoint][1]; //yy
		v[2] = controlPoints[previousPoint+1][2] - controlPoints[previousPoint][2]; //zz

		// Normalizar o vetor v
		float v_norm = sqrtf( pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2));

		if(v_norm != 0) {
			v[0] /= v_norm;
			v[1] /= v_norm;
			v[2] /= v_norm;
		}

		// Calcular velocidade 
		float velocity = (distance/span)*(deltaTime);

		// Calcular movimento
		v[0] *= velocity;
		v[1] *= velocity;
		v[2] *= velocity;

		// Atualizar os valores de xTranslation, yTranslation, zTranslation
		xTranslation += v[0];
		yTranslation += v[1];
		zTranslation += v[2];

		// Verificar se não passou o ponto de controlo seguinte
		if((v[0] > 0 && xTranslation > controlPoints[previousPoint+1][0]) || (v[0] < 0 && xTranslation < controlPoints[previousPoint+1][0]))
			xTranslation = controlPoints[previousPoint+1][0];
		if((v[1] > 0 && yTranslation > controlPoints[previousPoint+1][1]) || (v[1] < 0 && yTranslation < controlPoints[previousPoint+1][1]) )
			yTranslation = controlPoints[previousPoint+1][1];
		if((v[2] > 0 && zTranslation > controlPoints[previousPoint+1][2]) || (v[2] < 0 && zTranslation < controlPoints[previousPoint+1][2]))
			zTranslation = controlPoints[previousPoint+1][2];

		updateRotationAngle();
	}

		
}

void LinearAnimation::updateRotationAngle(){

	float vec1[2]; 
	float vec2[2]; 

	vec1[0] = controlPoints[previousPoint + 1][0] - controlPoints[previousPoint][0];
	vec1[1] = controlPoints[previousPoint + 1][2] - controlPoints[previousPoint][2];
	vec2[0] = 0;
	vec2[1] = 1;

	float scalProduct = vec1[0] * vec2[0] + vec1[1] * vec2[1];
	float normVec1 = sqrt(pow((float)vec1[0],(float)2.0) + pow((float)vec1[1],(float)2.0));
	float normVec2 = sqrt(pow((float)vec2[0],(float)2.0) + pow((float)vec2[1],(float)2.0));


	rotationAngle = acos(scalProduct / (normVec1 * normVec2)) * (180/PI);

}

//CIRCULAR ANIMATION
CircularAnimation::CircularAnimation(string id, float span, string type, vector<float> center, float radius, float startAngle, float rotAngle):Animation(id, span, type){
	this->center=center;
	this->radius=radius;
	this->startAngle=startAngle;
	this->rotAngle=rotAngle;

	this->angularSpeed = rotAngle/span;
	this->reset = true;
	this->repeat = true;
}

void CircularAnimation::update(unsigned long time){
	if(reset)
		init(time);
	else{
		if(!finished){
			float deltaTime = (time - startTime) * 0.001;
			this->startTime = time;

			currentAngle += angularSpeed * deltaTime;

			if(abs(currentAngle) >= abs(startAngle + rotAngle)){
				if(repeat)
					init(time);
				else
					finished = true;
			}
		}
	}
		
}

void CircularAnimation::apply(){
	if(!finished){
		glTranslatef(center[0], center[1], center[2]);
		glRotatef(currentAngle, 0, 1, 0);
		glTranslatef(radius, 0, 0);
	}
}

void CircularAnimation::init(unsigned long time){
	this->currentAngle = startAngle;

	this->reset = false;
	this->startTime = time;
	this->finished = false;
}