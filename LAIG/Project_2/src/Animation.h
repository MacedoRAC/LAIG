#ifndef ANIMATION_H
#define ANIMATION_H

#include<string>
#include<vector>
#include"CGFobject.h"

using namespace std;

class Animation{

public:
	string id;
	float span;
	string type;
	bool reset;
	unsigned long startTime;
	unsigned int previousPoint;

	Animation();
	Animation(string id, float span, string type);
	
	virtual void init(unsigned long time);
	virtual void update(unsigned long time);
	virtual void apply();

	~Animation(){};
};

class LinearAnimation: public Animation{
public:
	vector<vector<float>> controlPoints;
	float distance, xTranslation, yTranslation, zTranslation, rotation;

	LinearAnimation(string id, float span, string type, vector<vector<float>> ctrlPoints);
	void update(unsigned long time);
	void apply();
	void init(unsigned long time);

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