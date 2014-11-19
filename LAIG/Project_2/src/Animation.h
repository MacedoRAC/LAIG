#ifndef ANIMATION_H
#define ANIMATION_H

#include<string>
#include<vector>
#include"CGFobject.h"
#include "CGFapplication.h"

using namespace std;

class Animation{

public:
	string id;
	float span;
	string type;
	bool reset, repeat, finished;
	unsigned long startTime, controlTime;
	unsigned int previousPoint;

	Animation();
	Animation(string id, float span, string type);
	
	virtual void init(unsigned long time);
	virtual void update(unsigned long time) = 0;
	virtual void apply() = 0;

	//~Animation(){};
};

class LinearAnimation: public Animation{
public:
	vector<vector<float>> controlPoints;
	float distance, xTranslation, yTranslation, zTranslation, rotationAngle;
	unsigned long deltaT;

	LinearAnimation(string id, float span, string type, vector<vector<float>> ctrlPoints);
	void update(unsigned long time);
	void apply();
	void init(unsigned long time);
	void updateRotationAngle();

	~LinearAnimation(){};
};


class CircularAnimation: public Animation{
public:
	vector<float> center;
	float radius, startAngle, rotAngle, currentAngle, angularSpeed;

	CircularAnimation(string id, float span, string type, vector<float> center, float radius, float startAngle, float rotAngle);
	void update(unsigned long time);
	void apply();
	void init(unsigned long time);

	~CircularAnimation(){};
};

#endif