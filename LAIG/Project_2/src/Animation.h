#ifndef ANIMATION_H
#define ANIMATION_H

#include<string>
#include<vector>

using namespace std;

class Animation{

public:
	string id;
	float span;
	string type;

	Animation();
	Animation(string id, float span, string type);
	
	void init(unsigned long time);
	virtual void update(unsigned long time);
	virtual void apply();

	~Animation(){};
};

class LinearAnimation: public Animation{
public:
	vector<vector<float>> controlPoints;

	LinearAnimation(string id, float span, string type, vector<vector<float>> ctrlPoints);
	void update(unsigned long time) {};
	void apply(){};

	~LinearAnimation(){};
};


class CircularAnimation: public Animation{
public:
	vector<float> center;
	float radius;
	float startAngle;
	float rotAngle;

	CircularAnimation(string id, float span, string type, vector<float> center, float radius, float startAngle, float rotAngle);
	void update(unsigned long time){};
	void apply(){};

	~CircularAnimation(){};
};

#endif