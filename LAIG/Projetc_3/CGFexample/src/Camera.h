#ifndef CAMERA_H
#define CAMERA_H

#include"CGFcamera.h"
#include"Animation.h"
#include<string>
#include<vector>

using namespace std;

class Camera: public CGFcamera{
public:
	string id, type;
	float near, far;

	virtual void apply()=0;
};

class Orthogonal: public Camera{
public:
	char direction;
	float left,right,top,bottom;

	void apply();
};

class Perspective: public Camera{
public:
	float angle;
	vector<float> pos, target;

	void apply();
};
/*
class animatedCamera: public Camera, Animation{
public:
	vector<float> startPos, endPos, target;

	void apply();
	void update(unsigned long time);
	void init(unsigned long time);
}
*/
#endif