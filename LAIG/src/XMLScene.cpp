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

		parser.globals->drawing.mode=mode;


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
		if(valstring && sscanf(valstring, "%f %f %f %f", &background[0], &background[1], &background[2], &background[3])!=4){

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
		if(valstring && sscanf(valstring, "%f %f %f %f", &ambient[0], &ambient[1], &ambient[2], &ambient[3])!=4){

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
			if(posVal && sscanf(posVal, "%f %f %f", &pos[0], &pos[1], &pos[2])!=3){
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
			if(targetVal && sscanf(targetVal, "%f %f %f", &target[0], &target[1], &target[2])!=3){
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
				if(valstring && sscanf(valstring, "%f %f %f %f", &color[0], &color[1], &color[2], &color[3])!=4){

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

		parser.textures[text.id]=text;
		textures = textures->NextSiblingElement("textures");
	}


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

		// - - - - - - - - - - - - - - - - - - - - - - - - -
		// - - - - - - - - -APPEARANCES- - - - - - - - - - -
		// - - - - - - - - - - - - - - - - - - - - - - - - -

	TiXmlElement* appearance = matsElement->FirstChildElement("appearance");
	float shininess;
	string	textureref, componentType;
	float color[4];
	Appearance appea;

	while(appearance){
		printf("Appearance - %s\n", appearance->Attribute("id"));
		appea.id=appearance->Attribute("id");

		if(appearance->QueryFloatAttribute("shininess", &shininess) == TIXML_SUCCESS){
				printf("	Shininess: %f\n", shininess);
				appea.shininess=shininess;
		}
		printf("Textureref - %s\n", appearance->Attribute("textureref"));
		appea.textureRef=appearance->Attribute("textureref");
		/*textureref = *appearance->Attribute("textureref");
		printf("	Textureref: %s\n", &textureref);*/

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
				if(valstring && sscanf(valstring, "%f %f %f %f", &color[0], &color[1], &color[2], &color[3])!=4){

					printf("	Invalid ambient values. Default values were assumed.\n");
					color[0]= 0.5;
					color[1]= 0.5;
					color[2]= 0.5;
					color[3]= 1;
				}

				component = component->NextSiblingElement("component");
			}

			parser.appearances[appea.id]=appea;
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
				if(transforms == NULL)
				{
					printf("Transforms block not found!\n");
					break;
				}
				else
				{
					TiXmlElement *transform = transforms->FirstChildElement("transform");

					while(transform)
					{
						Transform *tr = new Transform();
						if((string) transform->Attribute("type") == "translate")
						{

							tr->type = "translate";
							char *pos=NULL;
							float posx,posy,posz;
							pos=(char *) transform->Attribute("to");

							if(pos && sscanf(pos,"%f %f %f",&posx, &posy, &posz)==3)
							{
								tr->to[0] = posx;
								tr->to[1] = posy;
								tr->to[2] = posz;
							}
							else
								printf("\tError reading position\n");
						}
						if((string) transform->Attribute("type") == "rotate")
						{
							tr->type = "rotate";

							tr->axis = transform->Attribute("axis");
							float angle;
							if(transform->QueryFloatAttribute("angle",&angle)==TIXML_SUCCESS)
								tr->angle = angle;
						}

						if((string) transform->Attribute("type") == "scale")
						{
							tr->type = "scale";
							char *pos=NULL;
							float posx,posy,posz;
							pos=(char *) transform->Attribute("factor");

							if(pos && sscanf(pos,"%f %f %f",&posx, &posy, &posz)==3)
							{
								tr->factor[0] = posx;
								tr->factor[1] = posy;
								tr->factor[2] = posz;
							}
							else
								printf("\tError reading factor\n");
						}


						pNode->transforms.push_back(tr);
						transform = transform->NextSiblingElement();
					}
				}

				TiXmlElement *appearance = node->FirstChildElement("appearanceref");
				if(appearance->Attribute("id") != "inherit")
				{
					pNode->appearance = &(parser.appearances[appearance->Attribute("id")]);
				}

				TiXmlElement *primitives = node->FirstChildElement("primitives");
				if(primitives == NULL)
				{
					printf("Primitives block not found!\n");
					break;
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

						if(pos && sscanf(pos,"%f %f",&pos1, &pos2)==2)
						{
							rect->xy1[0] = pos1;
							rect->xy1[1] = pos2;
						}
						else
							printf("\tError reading rectangle\n");

						pos=(char *) rectangle->Attribute("xy2");

						if(pos && sscanf(pos,"%f %f",&pos1, &pos2)==2)
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

						if(pos && sscanf(pos,"%f %f %f",&pos1, &pos2, &pos3)==3)
						{
							tri->xyz1[0] = pos1;
							tri->xyz1[1] = pos2;
							tri->xyz1[2] = pos3;
						}
						else
							printf("\tError reading triangle\n");

						pos=(char *) triangle->Attribute("xyz2");

						if(pos && sscanf(pos,"%f %f %f",&pos1, &pos2, &pos3)==3)
						{
							tri->xyz2[0] = pos1;
							tri->xyz2[1] = pos2;
							tri->xyz2[2] = pos3;
						}
						else
							printf("\tError reading triangle\n");

						pos=(char *) triangle->Attribute("xyz3");

						if(pos && sscanf(pos,"%f %f %f",&pos1, &pos2, &pos3)==3)
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
						pNode->descendants[nodeRef->Attribute("id")] = parser.graph->nodes[nodeRef->Attribute("id")];

						nodeRef = nodeRef->NextSiblingElement();
					}
				}
				parser.graph->nodes[pNode->id] = pNode;
				node = node->NextSiblingElement();

			}
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