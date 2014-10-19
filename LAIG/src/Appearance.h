#ifndef APPEARANCE_H
#define APPEARANCE_H

#include<string>
#include<vector>
#include"CGFappearance.h"
#include"Texture.h"

using namespace std;

class Appearance{
public:
	string id;
	float shininess;
	string textureref;
	vector<float> ambient_comp;
	vector<float> diffuse_comp;
	vector<float> specular_comp;
	CGFappearance* app;
	Texture* texture;
};

#endif