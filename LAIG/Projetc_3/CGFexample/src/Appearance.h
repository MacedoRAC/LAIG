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
	string textureref;
	CGFappearance* app;
	Texture* texture;

	Appearance();
	Appearance(string id, string textureref, CGFappearance* app, Texture* texture);
};

#endif