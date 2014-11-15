#include "Animation.h"

Animation::Animation(string id, float span, string type, vector<vector<float>> ctrlPoints){
	this->id = id;
	this->span=span;
	this->type=type;
	this->controlPoints=ctrlPoints;
}


Animation::Animation(string id, float span, string type, vector<float> center, float radius, float startAngle, float rotAngle){
	this->id=id;
	this->span=span;
	this->type=type;
	this->center=center;
	this->radius=radius;
	this->startAngle=startAngle;
	this->rotAngle=rotAngle;
}