#include"Texture.h"

Texture:: Texture(string id, string file, float texlength_S, float texlength_T){
	this->id=id;
	this->file=file;
	this->texlength_S=texlength_S;
	this->texlength_T=texlength_T;
}

Texture:: Texture(){
	this->id="";
	this->file="";
	this->texlength_S=0.0;
	this->texlength_T=0.0;
}