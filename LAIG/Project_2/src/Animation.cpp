#include "Animation.h"

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
}

void Animation::apply(){

}

void Animation::update(unsigned long time){

}

void Animation::init(unsigned long time){

}

//LINEAR ANIMATION
LinearAnimation::LinearAnimation(string id, float span, string type, vector<vector<float>> ctrlPoints): Animation(id, span, type){
	this->controlPoints=ctrlPoints;
}

//CIRCULAR ANIMATION
CircularAnimation::CircularAnimation(string id, float span, string type, vector<float> center, float radius, float startAngle, float rotAngle):Animation(id, span, type){
	this->center=center;
	this->radius=radius;
	this->startAngle=startAngle;
	this->rotAngle=rotAngle;
}