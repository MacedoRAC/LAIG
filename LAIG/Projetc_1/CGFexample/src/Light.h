#ifndef LIGHT_H
#define LIGHT_H

#include<vector>

using namespace std;

class Light{
public:
	string id, type;
	vector<float> pos, ambient, diffuse, specular;
	vector<float> target;
	float angle, exponent;
	bool enabled, marker;
};

#endif