#include <String>
#include <vector>
#include <map>

using namespace std;





class Drawing
{
	string mode,shading;
	float background[4];
public:
	Drawing(string mode, string shading, float bg[4]){
		this->mode = mode;
		this->shading = shading;
		this->background[0] = bg[0];
		this->background[1] = bg[1];
		this->background[2] = bg[2];
		this->background[3] = bg[3];
	}
};

class Culling
{
	string face, order;
public:
	Culling(string face, string order){
		this->face= face;
		this->order=order;
	}
};

class Lighting
{
	bool doublesided, local, enabled;
	float ambient[4];
public:
	Lighting(bool doublesided, bool local, bool enabled, float ambient[4]){
		this->doublesided = doublesided;
		this->local = local;
		this->enabled= enabled;
		this->ambient[0] = ambient[0];
		this->ambient[1] = ambient[1];
		this->ambient[2] = ambient[2];
		this->ambient[3] = ambient[3]; 
	}
};

class Globals
{
	Drawing drawing;
	Culling culling;
	Lighting lighting;
public:
	Globals(Drawing drawing, Culling culling, Lighting lighting) {
		this->drawing= drawing;
		this->culling= culling;
		this->lighting= lighting;
	}
};

class Camera
{
	string id;
public:
	Camera(string id){
		this->id=id;
	}
};

class CPerspective : public Camera
{
	float near, far, angle, pos[3], target[3];
public:
	CPerspective(string id, float near,float far,float angle,float pos[3],float target[3]): Camera(id){
		this->near=near;
		this->far=far;
		this->angle=angle;
		this->pos[0]=pos[0];
		this->pos[1]=pos[1];
		this->pos[2]=pos[2];
		this->target[0]=target[0];
		this->target[1]=target[1];
		this->target[2]=target[2];
	}
};

class COrtho : public Camera
{
	char direction;
	float near,far,left,right, top, bottom;
public:
	COrtho(string id, char direction, float near,float far,float left,float right,float top,float bottom): Camera(id){
		this->direction=direction;
		this->near=near;
		this->far=far;
		this->left=left;
		this->right=right;
		this->top=top;
		this->bottom=bottom;
	}
};

class Light
{
	string id,type;
	bool enabled,marker;
	float pos[3], ambient[4], diffuse[4], specular[4], target[3], angle, exponent;
public:
	Light(string id,string type, bool enabled,bool marker, float pos[3], float ambient[4], float diffuse[4], 
		float specular[4],float target[3],float angle,float exponent){

			this->id=id;
			this->type=type;
			this->enabled=enabled;
			this->marker=marker;
			this->pos[0]=pos[0];
			this->pos[1]=pos[1];
			this->pos[2]=pos[2];
			this->ambient[0]=ambient[0];
			this->ambient[1]=ambient[1];
			this->ambient[2]=ambient[2];
			this->diffuse[0]=diffuse[0];
			this->diffuse[1]=diffuse[1];
			this->diffuse[2]=diffuse[2];
			this->diffuse[3]=diffuse[3];
			this->specular[0]=specular[0];
			this->specular[1]=specular[1];
			this->specular[2]=specular[2];
			this->specular[3]=specular[3];
			this->target[0]=target[0];
			this->target[1]=target[1];
			this->target[2]=target[2];
			this->angle=angle;
			this->exponent=exponent;
	}
};

class Texture
{
	string id, file;
	float texLengthS,texLengthT;
public:
	Texture(string id, string file, float txtLenS, float txtLenT){
		this->id=id;
		this->file=file;
		this->texLengthS=txtLenS;
		this->texLengthT=txtLenT;
	}
};

class Appearance
{
	string id, textureRef;
	float ambient[4], diffuse[4], specular[4];
public:
	Appearance(string id,string textureRef, float ambient[4], float diffuse[4], float specular[4]){
		this->id=id;
		this->textureRef=textureRef;
		this->ambient[0]=ambient[0];
		this->ambient[1]=ambient[1];
		this->ambient[2]=ambient[2];
		this->diffuse[0]=diffuse[0];
		this->diffuse[1]=diffuse[1];
		this->diffuse[2]=diffuse[2];
		this->diffuse[3]=diffuse[3];
		this->specular[0]=specular[0];
		this->specular[1]=specular[1];
		this->specular[2]=specular[2];
		this->specular[3]=specular[3];
	}
};

class Transform
{
	string type,axis;
	float angle,factor[3], to[3];
public:
	Transform(string type, string axis, float angle,float factor[3],float to[3]){
		this->type=type;
		this->axis=axis;
		this->angle=angle;
		this->factor[0]=factor[0];
		this->factor[1]=factor[1];
		this->factor[2]=factor[2];
		this->to[0]=to[0];
		this->to[1]=to[1];
		this->to[2]=to[2];
	}
};

class Primitives
{
	string name;
public:
	Primitives(string name){
		this->name=name;
	}	
	static vector<float> getCoord();
};

class Rectangle : public Primitives
{	
	float xy1[2], xy2[2];
public:
	Rectangle(string name, float xy1[2], xy2[2]):Primitives(name){
		this->xy1[0]=xy1[0];
		this->xy1[1]=xy1[1];
		this->xy2[0]=xy2[0];
		this->xy2[1]=xy2[1];
	}
	vector<float> getCoord(){
		vector<float> coord;
		coord.push_back(xy1[0]);
		coord.push_back(xy1[1]);
		coord.push_back(xy2[0]);
		coord.push_back(xy2[1]);

		return coord;
	}

};

class Triangle : public Primitives
{
	float xyz1[3],xyz2[3], xyz3[3];
public:
	Triangle(string name, float xyz1[3],float xyz2[3],float xyz3[3]):Primitives(name){
		this->xyz1[0]=xyz1[0];
		this->xyz1[1]=xyz1[1];
		this->xyz1[2]=xyz1[2];
		this->xyz2[0]=xyz2[0];
		this->xyz2[1]=xyz2[1];
		this->xyz2[2]=xyz2[2];
		this->xyz3[0]=xyz3[0];
		this->xyz3[1]=xyz3[1];
		this->xyz3[2]=xyz3[2];
	}
	vector<float> getCoord(){
		vector<float> coord;
		coord.push_back(xyz1[0]);
		coord.push_back(xyz1[1]);
		coord.push_back(xyz1[2]);
		coord.push_back(xyz2[0]);
		coord.push_back(xyz2[1]);
		coord.push_back(xyz2[2]);
		coord.push_back(xyz3[0]);
		coord.push_back(xyz3[1]);
		coord.push_back(xyz3[2]);

		return coord;
	}
};

class Cylinder : public Primitives
{
	float base, top, height;
	int slices, stacks;
public:
	Cylinder(string name, float base, float top,float height, int slices, int stacks):Primitives(name){
		this->base=base;
		this->top=top;
		this->height=height;
		this->slices=slices;
		this->stacks=stacks;
	}
	vector<float> getCoord(){
		vector<float> coord;
		coord.push_back(base);
		coord.push_back(top);
		coord.push_back(height);
		coord.push_back(slices);
		coord.push_back(stacks);

		return coord;
	}
};

class Sphere : public Primitives
{
	float radius;
	int slices, stacks;
public:
	Sphere(string name, float radius, int slices, int stacks): Primitives(name){

	}
	vector<float> getCoord(){
		vector<float> coord;
		coord.push_back(radius);
		coord.push_back(slices);
		coord.push_back(stacks);

		return coord;
	}
};

class Torus : public Primitives
{
	float inner, outer;
	int slices, loops;
public:
	Torus(string name, float inner, float outer, int slices,int loops): Primitives(name){
		this->inner=inner;
		this->outer=outer;
		this->slices=slices;
		this->loops=loops;
	}
	vector<float> getCoord(){
		vector<float> coord;
		coord.push_back(inner);
		coord.push_back(outer);
		coord.push_back(loops);
		coord.push_back(slices);

		return coord;
	}
};

class Node
{
public:
	string id;
	vector<Transform*> tranforms;
	Appearance* apperance;
	vector<Primitives*> primitives;
	map<string, Node*> descendants;
};

class Graph
{
public:
	map<string, Node*> nodes;
	string rootID;
};

class Parser
{
public:
	Parser();
	~Parser();
	Globals* globals;
	map <string, Camera> cameras;
	string initCam;
	string activeCam;
	map<string, Light> lights;
	map<string, Texture> textures;
	map<string, Appearance> appearances;
	Graph* graph;
};