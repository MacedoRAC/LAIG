#ifndef TEXTURE_H
#define TEXTURE_H

#include<string>

using namespace std;

class Texture{
public:
	string id, file;
	float texlength_S, texlength_T;

	Texture(){};
	Texture(string id, string file, float texlength_S, float texlength_T);
};

#endif