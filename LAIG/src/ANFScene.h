#ifndef ANFSCENE_H
#define ANFSCENE_H

#include"Graph.h"
#include"tinyxml.h"

class ANFScene{
public:
	ANFScene(char *filename, Graph* graph);
	~ANFScene();

	static TiXmlElement *findChildByAttribute(TiXmlElement *parent,const char * attr, const char *val);
	
protected:

	TiXmlDocument* doc;

	TiXmlElement* globElement; 
	TiXmlElement* lightsElement;
	TiXmlElement* textsElement;
	TiXmlElement* appearancesElement;
	TiXmlElement* camerasElement;
	TiXmlElement* graphElement;
};

#endif