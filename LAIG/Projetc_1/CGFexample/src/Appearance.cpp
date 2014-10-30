#include"Appearance.h"

Appearance:: Appearance(string id, string textureref, CGFappearance* app, Texture* texture){
	this->id=id;
	this->textureref=textureref;
	this->app=app;
	this->texture=texture;
}

Appearance:: Appearance(){
	this->app=NULL;
	this->texture=NULL;
}