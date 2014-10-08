#include "XMLScene.h"

XMLScene::XMLScene(char *filename)
{

	// Read XML from file

	doc=new TiXmlDocument( filename );
	bool loadOkay = doc->LoadFile();

	if ( !loadOkay )
	{
		printf( "Could not load file '%s'. Error='%s'. Exiting.\n", filename, doc->ErrorDesc() );
		exit( 1 );
	}

	TiXmlElement* anfElement= doc->FirstChildElement( "anf" );

	if (anfElement == NULL)
	{
		printf("Main anf block element not found! Exiting!\n");
		exit(1);
	}

	globalsElement = anfElement->FirstChildElement( "globals" );
	camerasElement = anfElement->FirstChildElement( "cameras" );
	lightsElement = anfElement->FirstChildElement( "lights" );
	matsElement = anfElement->FirstChildElement( "appearances" );
	textsElement =  anfElement->FirstChildElement( "textures" );
	graphElement =  anfElement->FirstChildElement( "graph" );


	// - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - GLOBALS - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - -
	if (globalsElement == NULL)
		printf("Global block not found!\n");
	else
	{
		printf("Processing globals:\n\n");


		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// - - - - - - - - - - - DRAWING - - - - - - - - - -
		// - - - - - - - - - - - - - - - - - - - - - - - - -
		TiXmlElement* drawingElement=globalsElement->FirstChildElement("drawing");
		string mode, shading;
		float background[4];

		mode = drawingElement->Attribute("mode");
		if(mode.compare("fill") != 0 && mode.compare("line") != 0 && mode.compare("point") != 0 ){
			printf("	Invalid drawing mode. A default mode was selected.\n");
			mode = "fill";
		}

		printf("	Drawing mode: %s\n", mode.c_str());


		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// - - - - - - - - - - - SHADING - - - - - - - - - -
		// - - - - - - - - - - - - - - - - - - - - - - - - -
		shading = drawingElement->Attribute("shading");
		if(shading.compare("flat") != 0 && shading.compare("gouraud") != 0){
			printf("	Invalid shading. A default shading was selected.\n");
			shading = "flat";
		}

		printf("	Shading: %s\n", shading.c_str());

		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// - - - - - - - - - -BACKGROUND - - - - - - - - - -
		// - - - - - - - - - - - - - - - - - - - - - - - - -

		char * valstring = (char *)drawingElement->Attribute("background");
		if(valstring && sscanf(valstring, "%f %f %f %f", &background[0], &background[1], &background[2], &background[3])!=4){

			printf("	Invalid background color. Black was assumed as default.\n");
			background[0]= 0;
			background[1]= 0;
			background[2]= 0;
			background[3]= 1;
		}

		printf("	Background color: %f %f %f %f\n\n", background[0], background[1], background[2], background[3]);

		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// - - - - - - - - - - - CULLING - - - - - - - - - -
		// - - - - - - - - - - - - - - - - - - - - - - - - -
		TiXmlElement* cullingElement=globalsElement->FirstChildElement("culling");
		string face, order;

		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// - - - - - - - - - FACE CULLING - - - - - - - - -
		// - - - - - - - - - - - - - - - - - - - - - - - - -
		face = cullingElement->Attribute("face");
		if(face.compare("none") != 0 && face.compare("back") != 0 && face.compare("front") != 0 && face.compare("both") != 0){
			printf("	Invalid face culling. A default face culling was selected.\n");
			face = "back";
		}

		printf("	Culling face: %s\n", face.c_str());


		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// - - - - - - - - - - - - ORDER - - - - - - - - - -
		// - - - - - - - - - - - - - - - - - - - - - - - - -
		order = cullingElement->Attribute("order");
		if(order.compare("ccw") != 0 && face.compare("cw") != 0 ){
			printf("	Invalid culling order. A default culling order was selected.\n\n");
			order="ccw";
		}

		printf("	Culling order: %s\n\n", order.c_str());


		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// - - - - - - - - - - LIGHTING - - - - - - - - - -
		// - - - - - - - - - - - - - - - - - - - - - - - - -
		TiXmlElement* lightingElement=globalsElement->FirstChildElement("lighting");
		string doublesided, local, enabled;
		float ambient[4];

		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// - - - - - - - - DOUBLESIDED LIGHTING- - - - - - -
		// - - - - - - - - - - - - - - - - - - - - - - - - -
		doublesided = lightingElement->Attribute("doublesided");
		if(doublesided.compare("true") != 0 && doublesided.compare("false") != 0){
			printf("	Invalid doublesided value. A default value was admited.\n");
			doublesided="false";
		}

		printf("	Doublesided: %s\n", doublesided.c_str());

		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// - - - - - - - - - LOCAL LIGHTING - - - - - - - -
		// - - - - - - - - - - - - - - - - - - - - - - - - -
		local = lightingElement->Attribute("local");
		if(local.compare("true") != 0 && local.compare("false") != 0){
			printf("	Invalid local value. A default value was admited.\n");
			local="true";
		}

		printf("	local: %s\n", local.c_str());

		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// - - - - - - - - - ENABLED LIGHTING - - - - - - -
		// - - - - - - - - - - - - - - - - - - - - - - - - -
		enabled = lightingElement->Attribute("enabled");
		if(enabled.compare("true") != 0 && enabled.compare("false")){
			printf("	Invalid enabled value. A default value was admited.\n");
			enabled="true";
		}

		printf("	enabled: %s\n", enabled.c_str());


		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// - - - - - - - - - AMBIENT LIGHTING - - - - - - -
		// - - - - - - - - - - - - - - - - - - - - - - - - -

		valstring = (char *)lightingElement->Attribute("ambient");
		if(valstring && sscanf(valstring, "%f %f %f %f", &ambient[0], &ambient[1], &ambient[2], &ambient[3])!=4){

			printf("	Invalid ambient values. Default values were assumed.\n");
			ambient[0]= 0.2; 
			ambient[1]= 0.2;
			ambient[2]= 0.2;
			ambient[3]= 1;
		}

		printf("	Ambient: %f %f %f %f\n\n", ambient[0], ambient[1], ambient[2], ambient[3]);



		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// - - - - - - - - - - CAMERAS - - - - - - - - - - -
		// - - - - - - - - - - - - - - - - - - - - - - - - -
		TiXmlElement* perspective = camerasElement->FirstChildElement("perspective");
		TiXmlElement* ortho = camerasElement->FirstChildElement("ortho");

		printf("Camera %s\n",camerasElement->Attribute("initial"));


		//perspective Cameras

		float near, far, angle;
		float target[3], pos[3]; //coordinates are saved in an array
		char *posVal=NULL, *targetVal=NULL;

		while(perspective){
			printf("perspective camera: %s", perspective->Attribute("id"));

			if(perspective->QueryFloatAttribute("near", &near) == TIXML_SUCCESS)
				printf("	Near: %f\n", near);
			if(perspective->QueryFloatAttribute("far", &far) == TIXML_SUCCESS)
				printf("	Far: %f\n", far);
			if(perspective->QueryFloatAttribute("angle", &angle) == TIXML_SUCCESS)
				printf("	Angle: %f\n", angle);

			posVal = (char *) perspective->Attribute("pos");
			if(posVal && sscanf(posVal, "%f %f %f", &pos[0], &pos[1], &pos[2])!=3){
				printf("Error parsing perspective camera position. Default position was admited.\n");
				pos[0]=0;
				pos[1]=0;
				pos[2]=0;
			}

			printf("Camera position: %f ex, %f ey, %f ez\n", pos[0], pos[1], pos[2]);

			targetVal = (char *) perspective->Attribute("target");
			if(targetVal && sscanf(targetVal, "%f %f %f", &target[0], &target[1], &target[2])!=3){
				printf("Error parsing perspective camera target. Default position was admited.\n");
				target[0]=1;
				target[1]=1;
				target[2]=1;
			}

			printf("Camera target: %f ex, %f ey, %f ez\n", target[0], target[1], target[2]);


			perspective=perspective->NextSiblingElement("perspective");
		}


		//Ortho Cameras

		float nearOrtho,farOrtho,left,right,top,bottom;
		char* direction;

		while(ortho){
			printf("ortho camera: %s", perspective->Attribute("id"));

			direction = (char*)ortho->Attribute("direction");
			if(direction == (char*)'x' || direction == (char*)'y' || direction == (char*)'z')
				printf("	Direction: %c\n", direction);
			if(ortho->QueryFloatAttribute("near", &nearOrtho) == TIXML_SUCCESS)
				printf("	Near: %f\n", nearOrtho);
			if(ortho->QueryFloatAttribute("far", &farOrtho) == TIXML_SUCCESS)
				printf("	Far: %f\n", farOrtho);
			if(ortho->QueryFloatAttribute("left", &left) == TIXML_SUCCESS)
				printf("	Left: %f\n", left);
			if(ortho->QueryFloatAttribute("right", &right) == TIXML_SUCCESS)
				printf("	Right: %f\n", right);
			if(ortho->QueryFloatAttribute("top", &top) == TIXML_SUCCESS)
				printf("	Top: %f\n", top);
			if(ortho->QueryFloatAttribute("bottom", &bottom) == TIXML_SUCCESS)
				printf("	Bottom: %f\n", bottom);

			ortho=ortho->NextSiblingElement("ortho");
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// - - - - - - - - - - LIGHTS - - - - - - - - - - -
		// - - - - - - - - - - - - - - - - - - - - - - - - -

		TiXmlElement *light = lightsElement->FirstChildElement("lights");

		string type, enabled, marker, componentType;
		float color[4];

		while (light)
		{
			printf("Light - %s\n", light->Attribute("id"));

			type = lightsElement->Attribute("type");
			if(type.compare("omni") != 0 && type.compare("spot") != 0){
				printf("	Invalid type of light. A default value was admited.\n");
				type="omni";
			}
			enabled = lightsElement->Attribute("enabled");
			if(enabled.compare("true") != 0 && enabled.compare("false") != 0){
				printf("	Invalid switch state. A default value was admited.\n");
				type="true";
			}
			marker = lightsElement->Attribute("marker");
			if(marker.compare("true") != 0 && marker.compare("false") != 0){
				printf("	Invalid input on marker light. A default value was admited.\n");
				type="false";
			}

			TiXmlElement* component=light->FirstChildElement("component");

			while(component){
				componentType = lightsElement->Attribute("type");
				if(componentType.compare("ambient") != 0 && componentType.compare("diffuse") != 0 && componentType.compare("specular") != 0){
					printf("	Invalid component type input. A default value was admited.\n");
					type="ambient";
				}

				char * valstring = (char *)lightsElement->Attribute("value");
				if(valstring && sscanf(valstring, "%f %f %f %f", &color[0], &color[1], &color[2], &color[3])!=4){

					printf("	Invalid light color. Black was assumed as default.\n");
					color[0]= 0.5;
					color[1]= 0.5;
					color[2]= 0.5;
					color[3]= 1;
				}

				printf("	Light color: %f %f %f %f\n\n", color[0], color[1], color[2], color[3]);

				light = light->NextSiblingElement("component");
			}


			light = light->NextSiblingElement("lights");

		}
	}

	// Other blocks could be validated/processed here


	// graph section
	if (graphElement == NULL)
		printf("Graph block not found!\n");
	else
	{
		char *prefix="  -";
		TiXmlElement *node=graphElement->FirstChildElement();

		while (node)
		{
			printf("Node id '%s' - Descendants:\n",node->Attribute("id"));
			TiXmlElement *child=node->FirstChildElement();
			while (child)
			{
				if (strcmp(child->Value(),"Node")==0)
				{
					// access node data by searching for its id in the nodes section

					TiXmlElement *noderef=findChildByAttribute(nodesElement,"id",child->Attribute("id"));

					if (noderef)
					{
						// print id
						printf("  - Node id: '%s'\n", child->Attribute("id"));

						// prints some of the data
						printf("    - Material id: '%s' \n", noderef->FirstChildElement("material")->Attribute("id"));
						printf("    - Texture id: '%s' \n", noderef->FirstChildElement("texture")->Attribute("id"));

						// repeat for other leaf details
					}
					else
						printf("  - Node id: '%s': NOT FOUND IN THE NODES SECTION\n", child->Attribute("id"));

				}
				if (strcmp(child->Value(),"Leaf")==0)
				{
					// access leaf data by searching for its id in the leaves section
					TiXmlElement *leaf=findChildByAttribute(leavesElement,"id",child->Attribute("id"));

					if (leaf)
					{
						// it is a leaf and it is present in the leaves section
						printf("  - Leaf id: '%s' ; type: '%s'\n", child->Attribute("id"), leaf->Attribute("type"));

						// repeat for other leaf details
					}
					else
						printf("  - Leaf id: '%s' - NOT FOUND IN THE LEAVES SECTION\n",child->Attribute("id"));
				}

				child=child->NextSiblingElement();
			}
			node=node->NextSiblingElement();
		}
	}

}

XMLScene::~XMLScene()
{
	delete(doc);
}

//-------------------------------------------------------

TiXmlElement *XMLScene::findChildByAttribute(TiXmlElement *parent,const char * attr, const char *val)
	// Searches within descendants of a parent for a node that has an attribute _attr_ (e.g. an id) with the value _val_
	// A more elaborate version of this would rely on XPath expressions
{
	TiXmlElement *child=parent->FirstChildElement();
	int found=0;

	while (child && !found)
		if (child->Attribute(attr) && strcmp(child->Attribute(attr),val)==0)
			found=1;
		else
			child=child->NextSiblingElement();

	return child;
}