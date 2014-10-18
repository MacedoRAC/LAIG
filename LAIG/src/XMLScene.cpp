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

		
		parser.globals->drawing.mode=mode; //crashing here


		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// - - - - - - - - - - - SHADING - - - - - - - - - -
		// - - - - - - - - - - - - - - - - - - - - - - - - -
		shading = drawingElement->Attribute("shading");
		if(shading.compare("flat") != 0 && shading.compare("gouraud") != 0){
			printf("	Invalid shading. A default shading was selected.\n");
			shading = "flat";
		}

		printf("	Shading: %s\n", shading.c_str());

		parser.globals->drawing.shading = shading;

		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// - - - - - - - - - -BACKGROUND - - - - - - - - - -
		// - - - - - - - - - - - - - - - - - - - - - - - - -

		char * valstring = (char *)drawingElement->Attribute("background");
		if(valstring && sscanf_s(valstring, "%f %f %f %f", &background[0], &background[1], &background[2], &background[3])!=4){

			printf("	Invalid background color. Black was assumed as default.\n");
			background[0]= 0;
			background[1]= 0;
			background[2]= 0;
			background[3]= 1;
		}

		printf("	Background color: %f %f %f %f\n\n", background[0], background[1], background[2], background[3]);

		parser.globals->drawing.background[0] = background[0];
		parser.globals->drawing.background[1] = background[1];
		parser.globals->drawing.background[2] = background[2];
		parser.globals->drawing.background[3] = background[3];

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

		parser.globals->culling.face=face;

		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// - - - - - - - - - - - - ORDER - - - - - - - - - -
		// - - - - - - - - - - - - - - - - - - - - - - - - -
		order = cullingElement->Attribute("order");
		if(order.compare("ccw") != 0 && face.compare("cw") != 0 ){
			printf("	Invalid culling order. A default culling order was selected.\n\n");
			order="ccw";
		}

		printf("	Culling order: %s\n\n", order.c_str());
		
		parser.globals->culling.order=order;

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

		if(doublesided.compare("true"))
			parser.globals->lighting.doublesided=true;
		else
			parser.globals->lighting.doublesided=false;

		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// - - - - - - - - - LOCAL LIGHTING - - - - - - - -
		// - - - - - - - - - - - - - - - - - - - - - - - - -
		local = lightingElement->Attribute("local");
		if(local.compare("true") != 0 && local.compare("false") != 0){
			printf("	Invalid local value. A default value was admited.\n");
			local="true";
		}

		printf("	local: %s\n", local.c_str());

		if(local.compare("true"))
			parser.globals->lighting.local=true;
		else
			parser.globals->lighting.local=false;

		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// - - - - - - - - - ENABLED LIGHTING - - - - - - -
		// - - - - - - - - - - - - - - - - - - - - - - - - -
		enabled = lightingElement->Attribute("enabled");
		if(enabled.compare("true") != 0 && enabled.compare("false")){
			printf("	Invalid enabled value. A default value was admited.\n");
			enabled="true";
		}

		printf("	enabled: %s\n", enabled.c_str());

		if(enabled.compare("true"))
			parser.globals->lighting.enabled=true;
		else
			parser.globals->lighting.enabled=false;

		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// - - - - - - - - - AMBIENT LIGHTING - - - - - - -
		// - - - - - - - - - - - - - - - - - - - - - - - - -

		valstring = (char *)lightingElement->Attribute("ambient");
		if(valstring && sscanf_s(valstring, "%f %f %f %f", &ambient[0], &ambient[1], &ambient[2], &ambient[3])!=4){

			printf("	Invalid ambient values. Default values were assumed.\n");
			ambient[0]= 0.2; 
			ambient[1]= 0.2;
			ambient[2]= 0.2;
			ambient[3]= 1;
		}

		printf("	Ambient: %f %f %f %f\n\n", ambient[0], ambient[1], ambient[2], ambient[3]);

		parser.globals->lighting.ambient[0]=ambient[0];
		parser.globals->lighting.ambient[1]=ambient[1];
		parser.globals->lighting.ambient[2]=ambient[2];
		parser.globals->lighting.ambient[3]=ambient[3];

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// - - - - - - - - - - CAMERAS - - - - - - - - - - -
		// - - - - - - - - - - - - - - - - - - - - - - - - -
		TiXmlElement* perspective = camerasElement->FirstChildElement("perspective");
		TiXmlElement* ortho = camerasElement->FirstChildElement("ortho");

		printf("Camera %s\n",camerasElement->Attribute("initial"));

		parser.initCam = camerasElement->Attribute("initial");
		//perspective Cameras
		CPerspective persCam;
		float near, far, angle;
		float target[3], pos[3]; //coordinates are saved in an array
		char *posVal=NULL, *targetVal=NULL;

		while(perspective){
			printf("perspective camera: %s", perspective->Attribute("id"));
			persCam.id=perspective->Attribute("id");

			if(perspective->QueryFloatAttribute("near", &near) == TIXML_SUCCESS){
				printf("	Near: %f\n", near);
				persCam.near=near;
			}if(perspective->QueryFloatAttribute("far", &far) == TIXML_SUCCESS){
				printf("	Far: %f\n", far);
				persCam.far=far;
			}if(perspective->QueryFloatAttribute("angle", &angle) == TIXML_SUCCESS){
				printf("	Angle: %f\n", angle);
				persCam.angle=angle;
			}

			posVal = (char *) perspective->Attribute("pos");
			if(posVal && sscanf_s(posVal, "%f %f %f", &pos[0], &pos[1], &pos[2])!=3){
				printf("Error parsing perspective camera position. Default position was admited.\n");
				pos[0]=0;
				pos[1]=0;
				pos[2]=0;
			}

			printf("Camera position: %f ex, %f ey, %f ez\n", pos[0], pos[1], pos[2]);
			persCam.pos[0]=pos[0];
			persCam.pos[1]=pos[1];
			persCam.pos[2]=pos[2];

			targetVal = (char *) perspective->Attribute("target");
			if(targetVal && sscanf_s(targetVal, "%f %f %f", &target[0], &target[1], &target[2])!=3){
				printf("Error parsing perspective camera target. Default position was admited.\n");
				target[0]=1;
				target[1]=1;
				target[2]=1;
			}

			printf("Camera target: %f ex, %f ey, %f ez\n", target[0], target[1], target[2]);
			persCam.target[0]=target[0];
			persCam.target[1]=target[1];
			persCam.target[2]=target[2];
			
			parser.cameras[persCam.id]=persCam;

			perspective=perspective->NextSiblingElement("perspective");
		}


		//Ortho Cameras

		COrtho orthoCam;
		float nearOrtho,farOrtho,left,right,top,bottom;
		char* direction;

		while(ortho){
			printf("ortho camera: %s", ortho->Attribute("id"));
			orthoCam.id=ortho->Attribute("id");

			direction = (char*)ortho->Attribute("direction");
			if(*direction == 'x' || *direction == 'y' || *direction == 'z'){
				printf("	Direction: %c\n", *direction);
				orthoCam.direction=*direction;
			}if(ortho->QueryFloatAttribute("near", &nearOrtho) == TIXML_SUCCESS){
				printf("	Near: %f\n", nearOrtho);
				orthoCam.near=nearOrtho;
			}if(ortho->QueryFloatAttribute("far", &farOrtho) == TIXML_SUCCESS){
				printf("	Far: %f\n", farOrtho);
				orthoCam.far=farOrtho;
			}if(ortho->QueryFloatAttribute("left", &left) == TIXML_SUCCESS){
				printf("	Left: %f\n", left);
				orthoCam.left=left;
			}if(ortho->QueryFloatAttribute("right", &right) == TIXML_SUCCESS){
				printf("	Right: %f\n", right);
				orthoCam.right=right;
			}if(ortho->QueryFloatAttribute("top", &top) == TIXML_SUCCESS){
				printf("	Top: %f\n", top);
				orthoCam.top=top;
			}if(ortho->QueryFloatAttribute("bottom", &bottom) == TIXML_SUCCESS){
				printf("	Bottom: %f\n", bottom);
				orthoCam.bottom=bottom;
			}

			parser.cameras[orthoCam.id]=orthoCam;
			ortho=ortho->NextSiblingElement("ortho");
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// - - - - - - - - - - LIGHTS - - - - - - - - - - -
		// - - - - - - - - - - - - - - - - - - - - - - - - -

		TiXmlElement *light = lightsElement->FirstChildElement("lights");

		string type, marker, componentType;
		float color[4];
		Light lght;

		while (light)
		{
			printf("Light - %s\n", light->Attribute("id"));
			lght.id=light->Attribute("id");

			type = lightsElement->Attribute("type");
			if(type.compare("omni") != 0 && type.compare("spot") != 0){
				printf("	Invalid type of light. A default value was admited.\n");
				type="omni";
			}
			printf("	Type: %s\n", type);
			lght.type=type;

			enabled = lightsElement->Attribute("enabled");
			if(enabled.compare("true") != 0 && enabled.compare("false") != 0){
				printf("	Invalid switch state. A default value was admited.\n");
				enabled="true";
			}
			printf("	Enabled: %s\n", type);
			if(enabled.compare("true"))
				lght.enabled=true;
			else
				lght.enabled=false;

			marker = lightsElement->Attribute("marker");
			if(marker.compare("true") != 0 && marker.compare("false") != 0){
				printf("	Invalid input on marker light. A default value was admited.\n");
				marker="false";
			}
			if(marker.compare("true"))
				lght.marker=true;
			else
				lght.marker=false;

			TiXmlElement* component=light->FirstChildElement("component");

			while(component){
				componentType = component->Attribute("type");
				if(componentType.compare("ambient") != 0 && componentType.compare("diffuse") != 0 && componentType.compare("specular") != 0){
					printf("	Invalid component type input. A default value was admited.\n");
					componentType="ambient";
				}
				lght.type=componentType;

				char * valstring = (char *)component->Attribute("value");
				if(valstring && sscanf_s(valstring, "%f %f %f %f", &color[0], &color[1], &color[2], &color[3])!=4){

					printf("	Invalid light color. Black was assumed as default.\n");
					color[0]= 0.5;
					color[1]= 0.5;
					color[2]= 0.5;
					color[3]= 1;
				}

				printf("	Light color: %f %f %f %f\n\n", color[0], color[1], color[2], color[3]);
				if(componentType.compare("ambient")){
					lght.ambient[0]=color[0];
					lght.ambient[1]=color[1];
					lght.ambient[2]=color[2];
					lght.ambient[3]=color[3];
				}else if(componentType.compare("diffuse")){
					lght.diffuse[0]=color[0];
					lght.diffuse[1]=color[1];
					lght.diffuse[2]=color[2];
					lght.diffuse[3]=color[3];
				} else{
					lght.specular[0]=color[0];
					lght.specular[1]=color[1];
					lght.specular[2]=color[2];
					lght.specular[3]=color[3];
				}


				component = component->NextSiblingElement("component");
			}

			parser.lights.push_back(&lght);
			light = light->NextSiblingElement("lights");

		}
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// - - - - - - - - - - TEXTURES- - - - - - - - - - -
		// - - - - - - - - - - - - - - - - - - - - - - - - -

	TiXmlElement* textures = textsElement->FirstChildElement("textures");
	Texture text;
	string file;
	float texlength_t, texlength_s;
	while(textures){
		printf("Texture - %s\n", textures->Attribute("id"));
		text.id=textures->Attribute("id");

		file = textures->Attribute("file");
		printf("	The texture loaded was: %s", file);
		text.file=file;
		
		if(textures->QueryFloatAttribute("texlength_s", &texlength_s) == TIXML_SUCCESS){
				printf("	Texlength_s: %f\n", texlength_s);
				text.texLengthS=texlength_s;
		}if(textures->QueryFloatAttribute("texlength_t", &texlength_t) == TIXML_SUCCESS){
				printf("	Texlength_s: %f\n", texlength_t);
				text.texLengthT=texlength_t;
		}

		*parser.textures[text.id]=text;
		textures = textures->NextSiblingElement("textures");
	}


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// - - - - - - - - -APPEARANCES- - - - - - - - - - -
		// - - - - - - - - - - - - - - - - - - - - - - - - -

	TiXmlElement* appearance = matsElement->FirstChildElement("appearance");
	float shininess;
	string	textureref, componentType;
	float color[4]={0,0,0,0};
	Appearance appea;

	while(appearance){
		printf("Appearance - %s\n", appearance->Attribute("id"));
		appea.id=appearance->Attribute("id");

		if(appearance->QueryFloatAttribute("shininess", &shininess) == TIXML_SUCCESS){
				printf("	Shininess: %f\n", shininess);
				appea.shininess=shininess;
		}
		/*printf("Textureref - %s\n", appearance->Attribute("textureref"));
		appea.textureRef=appearance->Attribute("textureref");*/

		TiXmlElement* component=appearance->FirstChildElement("component");

			while(component){
				componentType = component->Attribute("type");
				if(componentType.compare("ambient") != 0 && componentType.compare("diffuse") != 0 && componentType.compare("specular") != 0){
					printf("	Invalid component type input. A default value was admited.\n");
					componentType="ambient";
				}

				printf("	%s: %f %f %f %f\n\n", componentType, color[0], color[1], color[2], color[3]);
				if(componentType.compare("ambient")){
					appea.ambient[0]=color[0];
					appea.ambient[1]=color[1];
					appea.ambient[2]=color[2];
					appea.ambient[3]=color[3];
				}else if(componentType.compare("diffuse")){
					appea.diffuse[0]=color[0];
					appea.diffuse[1]=color[1];
					appea.diffuse[2]=color[2];
					appea.diffuse[3]=color[3];
				} else{
					appea.specular[0]=color[0];
					appea.specular[1]=color[1];
					appea.specular[2]=color[2];
					appea.specular[3]=color[3];
				}

				char * valstring = (char *)component->Attribute("value");
				if(valstring && sscanf_s(valstring, "%f %f %f %f", &color[0], &color[1], &color[2], &color[3])!=4){

					printf("	Invalid ambient values. Default values were assumed.\n");
					color[0]= 0.5;
					color[1]= 0.5;
					color[2]= 0.5;
					color[3]= 1;
				}

				component = component->NextSiblingElement("component");
			}

			*parser.appearances[appea.id]=appea;
			appearance = appearance->NextSiblingElement("appearance");
	}
	

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// - - - - - - - - -GRAPH - - -- - - - - - - - - - -
		// - - - - - - - - - - - - - - - - - - - - - - - - -
if (graphElement == NULL)
		printf("Graph block not found!\n");
	else
	{

		TiXmlElement *node=graphElement->FirstChildElement();
		parser.graph->rootID = graphElement->Attribute("rootid");

		if(	node == NULL)
			printf("Node block not found!\n");
		else
		{
			while (node)
			{
				Node* pNode = new Node();
				pNode->id = node->Attribute("id");
				TiXmlElement *transforms = node->FirstChildElement("transforms");
				glLoadIdentity();
				glGetFloatv(GL_MODELVIEW_MATRIX,pNode->matrix);
				if(transforms == NULL)
				{
					printf("Transforms block not found!\n");
				}
				else
				{
					TiXmlElement *transform = transforms->FirstChildElement("transform");
					while(transform)
					{
						if((string) transform->Attribute("type") == "translate")
						{

							char *pos=NULL;
							float posx,posy,posz;
							pos=(char *) transform->Attribute("to");

							if(pos && sscanf_s(pos,"%f %f %f",&posx, &posy, &posz)==3)
							{
								glTranslatef(posx,posy,posz);
							}
							else
								printf("\tError reading position\n");
						}
						if((string) transform->Attribute("type") == "rotate")
						{
							string axis = (char*) transform->Attribute("axis");
							float angle;
							if(transform->QueryFloatAttribute("angle",&angle)==TIXML_SUCCESS)
								glRotatef(angle, axis == "x", axis == "y", axis == "z");
						}

						if((string) transform->Attribute("type") == "scale")
						{
							char *pos=NULL;
							float posx,posy,posz;
							pos=(char *) transform->Attribute("factor");

							if(pos && sscanf_s(pos,"%f %f %f",&posx, &posy, &posz)==3)
							{
								glScalef(posx,posy,posz);
							}
							else
								printf("\tError reading factor\n");
						}


						glGetFloatv(GL_MODELVIEW_MATRIX,pNode->matrix);
						transform = transform->NextSiblingElement();
					}
				}

				TiXmlElement *appearance = node->FirstChildElement("appearanceref");

				if(appearance)
					pNode->appearanceRef = appearance->Attribute("id");
				else 
					pNode->appearanceRef = "";

				TiXmlElement *primitives = node->FirstChildElement("primitives");
				if(primitives == NULL)
				{
					printf("Primitives block not found!\n");
				}
				else
				{

					TiXmlElement *rectangle = primitives->FirstChildElement("rectangle");
					TiXmlElement *triangle = primitives->FirstChildElement("triangle");
					TiXmlElement *cylinder = primitives->FirstChildElement("cylinder");
					TiXmlElement *sphere = primitives->FirstChildElement("sphere");
					TiXmlElement *torus = primitives->FirstChildElement("torus");

					while(rectangle){
						Rectangle* rect = new Rectangle();
						rect->name = "rectangle";

						char *pos=NULL;
						float pos1,pos2;
						pos=(char *) rectangle->Attribute("xy1");

						if(pos && sscanf_s(pos,"%f %f",&pos1, &pos2)==2)
						{
							rect->xy1[0] = pos1;
							rect->xy1[1] = pos2;
						}
						else
							printf("\tError reading rectangle\n");

						pos=(char *) rectangle->Attribute("xy2");

						if(pos && sscanf_s(pos,"%f %f",&pos1, &pos2)==2)
						{
							rect->xy2[0] = pos1;
							rect->xy2[1] = pos2;
						}
						else
							printf("\tError reading rectangle\n");

						pNode->primitives.push_back(rect);
						rectangle=rectangle->NextSiblingElement("rectangle");
					}
					while(triangle){

						Triangle* tri = new Triangle();
						tri->name = "triangle";

						char *pos=NULL;
						float pos1,pos2, pos3;
						pos=(char *) triangle->Attribute("xyz1");

						if(pos && sscanf_s(pos,"%f %f %f",&pos1, &pos2, &pos3)==3)
						{
							tri->xyz1[0] = pos1;
							tri->xyz1[1] = pos2;
							tri->xyz1[2] = pos3;
						}
						else
							printf("\tError reading triangle\n");

						pos=(char *) triangle->Attribute("xyz2");

						if(pos && sscanf_s(pos,"%f %f %f",&pos1, &pos2, &pos3)==3)
						{
							tri->xyz2[0] = pos1;
							tri->xyz2[1] = pos2;
							tri->xyz2[2] = pos3;
						}
						else
							printf("\tError reading triangle\n");

						pos=(char *) triangle->Attribute("xyz3");

						if(pos && sscanf_s(pos,"%f %f %f",&pos1, &pos2, &pos3)==3)
						{
							tri->xyz3[0] = pos1;
							tri->xyz3[1] = pos2;
							tri->xyz3[2] = pos3;
						}
						else
							printf("\tError reading triangle\n");

						pNode->primitives.push_back(tri);

						triangle=triangle->NextSiblingElement("triangle");
					}
					while(cylinder){
						Cylinder* cyl = new Cylinder(); 
						cyl->name = "cylinder";

						float base,top,height;
						int slices, stacks;

						if(cylinder->QueryFloatAttribute("base",&base)==TIXML_SUCCESS)
							cyl->base = base;
						if(cylinder->QueryFloatAttribute("top",&top)==TIXML_SUCCESS)
							cyl->top = top;
						if(cylinder->QueryFloatAttribute("height",&height)==TIXML_SUCCESS)
							cyl->height = height;
						if(cylinder->QueryIntAttribute("slices",&slices)==TIXML_SUCCESS)
							cyl->slices = slices;
						if(cylinder->QueryIntAttribute("stacks",&stacks)==TIXML_SUCCESS)
							cyl->stacks = stacks;

						pNode->primitives.push_back(cyl);
						cylinder=cylinder->NextSiblingElement("cylinder");
					}
					while(sphere){
						Sphere* sph = new Sphere();
						sph->name = "sphere";

						float radius;
						int slices, stacks;

						if(sphere->QueryFloatAttribute("radius",&radius)==TIXML_SUCCESS)
							sph->radius = radius;
						if(sphere->QueryIntAttribute("slices",&slices)==TIXML_SUCCESS)
							sph->slices = slices;
						if(sphere->QueryIntAttribute("stacks",&stacks)==TIXML_SUCCESS)
							sph->stacks = stacks;

						pNode->primitives.push_back(sph);
						sphere=sphere->NextSiblingElement("sphere");
					}
					while(torus){

						Torus* tor = new Torus();
						tor->name = "torus";

						float inner, outer;
						int slices, loops;

						if(torus->QueryFloatAttribute("inner",&inner)==TIXML_SUCCESS)
							tor->inner = inner;
						if(torus->QueryFloatAttribute("outer",&outer)==TIXML_SUCCESS)
							tor->outer = outer;
						if(torus->QueryIntAttribute("slices",&slices)==TIXML_SUCCESS)
							tor->slices = slices;
						if(torus->QueryIntAttribute("loops",&loops)==TIXML_SUCCESS)
							tor->loops = loops;

						pNode->primitives.push_back(tor);
						torus=torus->NextSiblingElement("torus");
					}

				}

				TiXmlElement *descendants = node->FirstChildElement("descendants");
				if(descendants !=NULL)
				{
					TiXmlElement *nodeRef = descendants->FirstChildElement();
					while(nodeRef)
					{
						pNode->descendants.push_back(nodeRef->Attribute("id"));

						nodeRef = nodeRef->NextSiblingElement();
					}
				}
				parser.graph->nodes[pNode->id] = pNode;
				node = node->NextSiblingElement();

			}
		}
	}


}


void XMLScene::switchLight(int id,bool enable1){
	parser.lights[id]->enabled = enable1;
}

XMLScene::~XMLScene()
{
	delete(doc);
}

void XMLScene::init() 
{

	float globalAmbientLight[4];
	for(int i = 0; i < 4 ;i++)
		globalAmbientLight[i] = parser.globals->lighting.ambient[i];

	if(parser.globals->lighting.enabled)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	if(parser.globals->lighting.doublesided)
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	else
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	if(parser.globals->lighting.local)
		glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	else
		glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);



	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientLight);  


	if(parser.globals->drawing.shading == "flat")
		glShadeModel(GL_FLAT);
	else
		glShadeModel(GL_SMOOTH);



	if(parser.globals->drawing.mode == "fill")
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else if(parser.globals->drawing.mode == "point")
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	glClearColor(parser.globals->drawing.background[0], parser.globals->drawing.background[1],
		parser.globals->drawing.background[2],parser.globals->drawing.background[3]);

	//Globals culling
	glEnable(GL_CULL_FACE);

	if(parser.globals->culling.face == "none")
		glDisable(GL_CULL_FACE);
	else if (parser.globals->culling.face == "front")
		glCullFace(GL_FRONT);
	else if (parser.globals->culling.face == "back")
		glCullFace(GL_BACK);
	else
		glCullFace(GL_FRONT_AND_BACK);

	if(parser.globals->culling.order == "cw")
		glFrontFace(GL_CW);
	else
		glFrontFace(GL_CCW);

	int id[8];
	id[0] = GL_LIGHT0;
	id[1] = GL_LIGHT1;
	id[2] = GL_LIGHT2;
	id[3] = GL_LIGHT3;
	id[4] = GL_LIGHT4;
	id[5] = GL_LIGHT5;
	id[6] = GL_LIGHT6;
	id[7] = GL_LIGHT7;


	for(unsigned int i=0;i<parser.lights.size() && i<8;i++)
	{


		CGFlight* light;
		Light* recentlight = parser.lights[i];
		float pos[4];
		pos[0] = recentlight->pos[0];
		pos[1] = recentlight->pos[1];
		pos[2] = recentlight->pos[2];
		pos[3] = 1;
		if(recentlight->type==("spot")){
			//glLightf(id,GL_SPOT_CUTOFF,recentlight->angle);
			glLightf(id[i],GL_SPOT_EXPONENT,recentlight->exponent);
			glLightfv(id[i],GL_SPOT_DIRECTION,recentlight->target);
			pos[3] = 0;
			light->setAngle(recentlight->angle);
		}

		light = new CGFlight(id[i], pos);
		light->setAmbient(recentlight->ambient);
		light->setSpecular(recentlight->specular);
		light->setDiffuse(recentlight->diffuse);

		lightsV.push_back(light);
	}



	// Uncomment below to enable normalization of lighting normal vectors
	glEnable (GL_NORMALIZE);
	glEnable (GL_TEXTURE_2D);

	//Declares scene elements

}




void XMLScene::display() 
{

	// ---- BEGIN Background, camera and axis setup

	// Clear image and depth buffer everytime we update the scene
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// Initialize Model-View matrix as identity (no transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	// Apply transformations corresponding to the camera position relative to the origin
	CGFscene::activeCamera->applyView();


	for(unsigned int i=0;i<parser.lights.size() && i<8;i++)
	{
		if(parser.lights[i]->marker)
			lightsV[i]->draw();

		lightsV[i]->disable();
		if(parser.lights[i]->enabled)
			lightsV[i]->enable();
		lightsV[i]->update();
	}
	// Draw axis
	axis.draw();

	drawGraph(parser.graph->rootID);

	glutSwapBuffers();
}

void XMLScene::drawGraph(string nodeID)
{

	Node Cnode;
	Cnode = *parser.graph->nodes[nodeID];
	if(Cnode.appearanceRef != "")
		if(Cnode.appearanceRef != "inherit")
			parser.appearances[Cnode.appearanceRef]->appCGF->apply();

	glMultMatrixf(Cnode.matrix);

	for(unsigned int i = 0; i < Cnode.primitives.size(); i++)
	{
		if(Cnode.appearanceRef != "inherit")
		{
			if(parser.appearances[Cnode.appearanceRef]->isTexApp)
				(*Cnode.primitives[i]).draw(parser.textures[parser.appearances[Cnode.appearanceRef]->textureRef]);
			else
				(*Cnode.primitives[i]).draw();
		}
		else
			(*Cnode.primitives[i]).draw();
	}

	for(unsigned int i = 0; i < Cnode.descendants.size(); i++)
	{
		glPushMatrix();
		drawGraph(Cnode.descendants[i]);
		glPopMatrix();
	}


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