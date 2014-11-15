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
	vector<vector<float>> controlPoints;
	vector<float> center;
	float radius;
	float startAngle;
	float rotAngle;

	Animation(){};
	Animation(string id, float span, string type, vector<vector<float>> ctrlPoints);
	Animation(string id, float span, string type, vector<float> center, float radius, float startAngle, float rotAngle);
};

#endif