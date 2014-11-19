#include "CGFapplication.h"
#include "ANFScene.h"
#include <math.h>

ANFScene::ANFScene(char *filename, Graph* graph)
{

	// Read ANF from file

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



	globElement = anfElement->FirstChildElement( "globals" );
	camerasElement = anfElement->FirstChildElement( "cameras" );
	lightsElement = anfElement->FirstChildElement( "lights" );
	appearancesElement = anfElement->FirstChildElement( "appearances" );
	textsElement = anfElement->FirstChildElement( "textures" );
	graphElement =  anfElement->FirstChildElement( "graph" );
	animationsElement = anfElement->FirstChildElement( "animations" );


	//- - - - - - - - - - - - - - - - - - - - - - - - - - 
	//- - - - - - - - - -GLOBALS- - - - - - - - - - - - - 
	//- - - - - - - - - - - - - - - - - - - - - - - - - - 

	if (globElement == NULL)
		printf("Init block not found!\n");
	else
	{
		printf("Processing init:\n");
		
		//Drawing
		TiXmlElement* drawElement= globElement->FirstChildElement("drawing");
		if (drawElement)
		{
			float r,g,b,a;
			char* mode = (char *) drawElement->Attribute("mode");
			char* shading = (char *) drawElement->Attribute("shading");
			char* valstring = (char *) drawElement->Attribute("background");

			if (mode && shading && valstring && sscanf(valstring,"%f %f %f %f",&r,&g,&b,&a) ==4)
			{
				graph->drawingMode=mode;
				graph->shading=shading;
				graph->background.push_back(r);
				graph->background.push_back(g);
				graph->background.push_back(b);
				graph->background.push_back(a);
				printf("  Drawing attributes: %s %s\n , background %f %f %f %f \n", mode, shading, r ,g ,b ,a);
			}
			else
				printf("Error drawing frustum\n");
		}
		else
			printf("drawing not found\n");


		//Culling
		TiXmlElement* culElement=globElement->FirstChildElement("culling");
		if (culElement)
		{
			char* face=(char *) culElement->Attribute("face");
			char* order=(char *) culElement->Attribute("order");

			if(face && order)
			{
				graph->cullingFace=face;
				graph->cullingOrder=order;
				printf("  Culling values: face: %s  order: %s\n", face,order);
			}
			else
				printf("Error parsing culling");
		}
		else
			printf("culling not found\n");  

		//Lighting
		TiXmlElement* lightElement = globElement->FirstChildElement("lighting");
		if (lightElement)
		{
			int doublesided, local, enabled;
			float r,g,b,a;

			char* valStringD=(char *) lightElement->Attribute("doublesided");
			char* valStringL=(char *) lightElement->Attribute("local");
			char* valStringE=(char *) lightElement->Attribute("enabled");
			char* valStringA=(char *) lightElement->Attribute("ambient");


			if(valStringD && valStringL && valStringE && valStringA && sscanf(valStringA,"%f %f %f %f",&r,&g,&b,&a) ==4)
			{
				if(strcmp(valStringD,"true")==0)
					doublesided = 1;
				else
					doublesided = 0;

				if(strcmp(valStringL,"true")==0)
					local = 1;
				else
					local = 0;

				if(strcmp(valStringE,"true")==0)
					enabled = 1;
				else
					enabled = 0;

				graph->lDoubleSided=doublesided;
				graph->lLocal=local;
				graph->lEnabled=enabled;
				graph->lAmbient.push_back(r);
				graph->lAmbient.push_back(g);
				graph->lAmbient.push_back(b);
				graph->lAmbient.push_back(a);

				printf("	Lightning values: doublesided: %d  , local: %d , enabled:  %d \n ambient: %f %f %f %f \n",doublesided,local,enabled,r,g,b,a);
			}
			else
				printf("Error parsing lightning");
		}
		else
			printf("lightning not found\n");
	}

	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - 
	//- - - - - - - - - -CAMERAS- - - - - - - - - - - - - 
	//- - - - - - - - - - - - - - - - - - - - - - - - - - 
	
	if (camerasElement == NULL)
		printf("Cameras block not found!\n");
	else
	{
		char * rootCamera = (char*) camerasElement->Attribute("initial");
		graph->rootCamera= (string)rootCamera;

		//Orthogonal Cameras
		TiXmlElement *cam=camerasElement->FirstChildElement("ortho");

		while(cam){
			Orthogonal* orthoCam = new Orthogonal();
			char * camID = (char*)cam->Attribute("id");     
			char * direction = (char *) cam->Attribute("direction");
			char * valstringNear = (char *) cam->Attribute("near");
			char * valstringFar = (char *) cam->Attribute("far");
			char * valstringLeft = (char *) cam->Attribute("left");
			char * valstringRight = (char *) cam->Attribute("right");
			char * valstringTop = (char *) cam->Attribute("top");
			char * valstringBottom = (char *) cam->Attribute("bottom");
			float near,far,left,right,top,bottom;

			if(sscanf(valstringNear,"%f",&near) == 1 && sscanf(valstringFar,"%f",&far) == 1 && sscanf(valstringLeft,"%f",&left) == 1
				&& sscanf(valstringRight,"%f",&right) == 1 && sscanf(valstringTop,"%f",&top) == 1 && sscanf(valstringBottom,"%f",&bottom) == 1){
				
				orthoCam->id=camID;
				orthoCam->direction=direction[0];
				orthoCam->near=near;
				orthoCam->far=far;
				orthoCam->left=left;
				orthoCam->right=right;
				orthoCam->top=top;
				orthoCam->bottom=bottom;
			}

			graph->cameras[orthoCam->id]=orthoCam; //add camera to graph

			cam = cam->NextSiblingElement("ortho");
		}

		//Perspective Cameras
		cam = camerasElement->FirstChildElement("perspective");
		while(cam){
			Perspective* perspCam = new Perspective();
			char * camID = (char*)cam->Attribute("id");     
			char * valstringNear = (char *) cam->Attribute("near");
			char * valstringFar = (char *) cam->Attribute("far");
			char * valstringAngle = (char *) cam->Attribute("angle");
			char * valstringPos= (char *) cam->Attribute("pos");
			char * valstringTarget = (char *) cam->Attribute("target");
			float near,far,angle,posX,posY,posZ,targetX,targetY,targetZ;

			if( sscanf(valstringNear,"%f",&near) == 1 && sscanf(valstringFar,"%f",&far) == 1  && sscanf(valstringAngle,"%f",&angle) == 1 && sscanf(valstringPos,"%f %f %f",&posX,&posY,&posZ) == 3
				&& sscanf(valstringTarget,"%f %f %f",&targetX,&targetY,&targetZ) == 3){

				perspCam->id=camID;
				perspCam->near=near;
				perspCam->far=far;
				perspCam->angle=angle;
				perspCam->pos.push_back(posX);
				perspCam->pos.push_back(posY);
				perspCam->pos.push_back(posZ);
				perspCam->target.push_back(targetX);
				perspCam->target.push_back(targetY);
				perspCam->target.push_back(targetZ);
			}
			
			graph->cameras[perspCam->id]=perspCam;

			cam = cam->NextSiblingElement("perspective");
		}

	}

	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - 
	//- - - - - - - - - - LIGHTS- - - - - - - - - - - - - 
	//- - - - - - - - - - - - - - - - - - - - - - - - - - 

	if (lightsElement == NULL)
		printf("Lights block not found!\n");
	else
	{


		TiXmlElement *light=lightsElement->FirstChildElement("light");

		while(light)
		{
			int enabled,marker;
			float posX,posY,posZ;
			char * id = (char*) light->Attribute("id");
			string type = light->Attribute("type");
			char * valstringEnabled = (char*) light->Attribute("enabled");			
			char * valstringMarker = (char*) light->Attribute("marker");
			char * pos = (char*) light->Attribute("pos");

			if(sscanf(pos,"%f %f %f",&posX,&posY,&posZ) == 3)
			{
				if(valstringEnabled == "true")
					enabled = true;
				else 
					enabled = false;
				if(valstringMarker == "true")
					marker = true;
				else
					marker = false;

			}

			Light newLight;
			newLight.id=id;
			newLight.type=type;
			newLight.enabled=enabled;
			newLight.marker=marker;
			newLight.pos.push_back(posX);
			newLight.pos.push_back(posY);
			newLight.pos.push_back(posZ);


			if(type.compare("spot") == 0)
			{
				float angle, exponent, targetX,targetY,targetZ;
				char* valstringAngle = (char*) light->Attribute("angle");
				char* valstringExponent = (char*) light->Attribute("exponent");
				char* valstringTarget = (char*) light->Attribute("target");

				if(sscanf(valstringAngle,"%f",&angle) ==1 && sscanf(valstringExponent,"%f",&exponent)  == 1 && sscanf(valstringTarget,"%f %f %f",&targetX,&targetY,&targetZ) == 3)
				{
					newLight.angle=angle;
					newLight.exponent=exponent;
					newLight.target.push_back(targetX);
					newLight.target.push_back(targetY);
					newLight.target.push_back(targetZ);
				}
			}

			TiXmlElement *lightComponent = light->FirstChildElement("component");

			//initialize all light components with zero
			newLight.ambient.push_back(0);
			newLight.ambient.push_back(0);
			newLight.ambient.push_back(0);
			newLight.ambient.push_back(0);
			newLight.diffuse=newLight.ambient;
			newLight.specular=newLight.ambient;

			while(lightComponent)
			{
				float r,g,b,a;
				char * type = (char*) lightComponent->Attribute("type");
				char * valString = (char*) lightComponent->Attribute("value");

				if(sscanf(valString,"%f %f %f %f",&r,&g,&b,&a) == 4)
				{
					if(strcmp(type, "ambient") == 0)
					{
						newLight.ambient[0]=r;
						newLight.ambient[1]=g;
						newLight.ambient[2]=b;
						newLight.ambient[3]=a;
					}
					else if(strcmp(type, "diffuse") == 0)
					{
						newLight.diffuse[0]=r;
						newLight.diffuse[1]=g;
						newLight.diffuse[2]=b;
						newLight.diffuse[3]=a;
					}
					else if(strcmp(type, "specular") == 0)
					{
						newLight.specular[0]=r;
						newLight.specular[1]=g;
						newLight.specular[2]=b;
						newLight.specular[3]=a;
					}

				}


				lightComponent = lightComponent->NextSiblingElement();
			}

			graph->lights.push_back(newLight);

			light = light->NextSiblingElement();
		}


	}



	//- - - - - - - - - - - - - - - - - - - - - - - - - - 
	//- - - - - - - - - -TEXTURE- - - - - - - - - - - - - 
	//- - - - - - - - - - - - - - - - - - - - - - - - - - 

	if (textsElement == NULL)
		printf("Text block not found!\n");
	else
	{
		TiXmlElement *textures=textsElement->FirstChildElement("texture");

		while(textures){

			float texlength_S=0, texlength_T=0;
			char* id = (char*) textures->Attribute("id");
			char* file =(char*) textures->Attribute("file");
			char* texlength_s= (char *)textures->Attribute("texlength_s");
			char* texlength_t= (char *)textures->Attribute("texlength_t");

			if(sscanf(texlength_s,"%f",&texlength_S) == 1 && sscanf(texlength_t,"%f",&texlength_T) == 1){
								
				graph->textures.push_back(Texture(id, file, texlength_S, texlength_T));	
			}

			textures= textures->NextSiblingElement();
		}

	}

	
	//- - - - - - - - - - - - - - - - - - - - - - - - - - 
	//- - - - - - - - - -APPEARANCE - - - - - - - - - - - 
	//- - - - - - - - - - - - - - - - - - - - - - - - - - 

	if (appearancesElement == NULL)
		printf("Appearence block not found!\n");
	else
	{
		TiXmlElement *appearances=appearancesElement->FirstChildElement("appearance");
		Texture * text = NULL; 
		
		while(appearances)
		{
			text = NULL;
			float shininess = 0;
			char * id = (char*) appearances->Attribute("id");
			char * shininessString = (char*) appearances->Attribute("shininess");
			char * texturerefp = NULL;
			texturerefp = (char *) appearances->Attribute("textureref");			
			string textureref = "";
			sscanf(shininessString,"%f",&shininess);

			if(texturerefp != NULL){
				textureref = (string) texturerefp;
			}

			TiXmlElement * appearanceComponent = appearances->FirstChildElement("component");

			float ambient_comp[4], diffuse_comp[4], specular_comp[4];

			while(appearanceComponent)
			{
				float r,g,b,a;
				string type = appearanceComponent->Attribute("type");
				char * valString = (char*) appearanceComponent->Attribute("value");

				if(sscanf(valString,"%f %f %f %f",&r,&g,&b,&a) == 4)
				{
					if(type.compare("ambient")==0){
						ambient_comp[0]=r;
						ambient_comp[1]=g;
						ambient_comp[2]=b;
						ambient_comp[3]=a;
					}
					else if(type.compare("diffuse")==0){
						diffuse_comp[0]=r;
						diffuse_comp[1]=g;
						diffuse_comp[2]=b;
						diffuse_comp[3]=a;
					}
					else if(type.compare("specular")==0){
						specular_comp[0]=r;
						specular_comp[1]=g;
						specular_comp[2]=b;
						specular_comp[3]=a;
					}

				}

				appearanceComponent = appearanceComponent->NextSiblingElement();
				
			}

			CGFappearance* CGFapp= NULL;
			CGFapp = new CGFappearance(ambient_comp, diffuse_comp, specular_comp, shininess);
			CGFapp->setShininess(shininess);

			if(textureref != ""){
				for(unsigned int j= 0 ; j < graph->textures.size();j++){
					if (textureref.compare(graph->textures[j].id)==0)
					{
						text = &graph->textures[j];
					}
				}
			}


			graph->appearances[id]=Appearance(id, textureref, CGFapp, text);

			if(graph->appearances[id].textureref != "")
				graph->appearances[id].app->setTexture(graph->appearances[id].texture->file);

			appearances = appearances->NextSiblingElement();
		}

	}


	//- - - - - - - - - - - - - - - - - - - - - - - - - - 
	//- - - - - - - - - - ANIMATION - - - - - - - - - - - 
	//- - - - - - - - - - - - - - - - - - - - - - - - - - 
	if (animationsElement == NULL)
		printf("Animations block not found!\n");
	else
	{
		TiXmlElement *animation = animationsElement->FirstChildElement("animation");

		while(animation){

			char* id = (char*) animation->Attribute("id");
			char* type =(char*) animation->Attribute("type");
			char* spanString= (char *)animation->Attribute("span");
			float span=0;
			sscanf(spanString,"%f", &span);

			if(strcmp(type, "linear")==0){
				TiXmlElement *ctrlPoints = animation->FirstChildElement("controlpoint");
				vector<vector<float>> cPoints;

				while(ctrlPoints){
					vector<float> point;

					char* xxString = (char*) ctrlPoints->Attribute("xx");
					float xx=0;
					if(sscanf(xxString,"%f", &xx))
						point.push_back(xx);

					char* yyString = (char*) ctrlPoints->Attribute("yy");
					float yy=0;
					if(sscanf(yyString,"%f", &yy))
						point.push_back(yy);

					char* zzString = (char*) ctrlPoints->Attribute("zz");
					float zz=0;
					if(sscanf(zzString,"%f", &zz))
						point.push_back(zz);

					cPoints.push_back(point);

					ctrlPoints = ctrlPoints->NextSiblingElement();
				}

				graph->animations[id] = LinearAnimation(id, span, type, cPoints);

			}else{
				char* centerString = (char*) animation->Attribute("center");
				vector<float> center;
				float centerX, centerY, centerZ;
				sscanf(centerString, "%f %f %f", &centerX, &centerY, &centerZ);

				center.push_back(centerX);
				center.push_back(centerY);
				center.push_back(centerZ);

				char* radiusString = (char*) animation->Attribute("radius");
				float radius = 0;
				sscanf(radiusString,"%f",&radius);

				char* starAngString = (char*) animation->Attribute("startang");
				float startAng = 0;
				sscanf(starAngString,"%f",&startAng);

				char* rotAngString = (char*) animation->Attribute("rotang");
				float rotAng = 0;
				sscanf(rotAngString,"%f",&rotAng);

				graph->animations[id] = CircularAnimation(id, span, type, center, radius, startAng, rotAng);
			}

			animation = animation->NextSiblingElement();
		}

	}

	//- - - - - - - - - - - - - - - - - - - - - - - - - - 
	//- - - - - - - - - - - GRAPH - - - - - - - - - - - - 
	//- - - - - - - - - - - - - - - - - - - - - - - - - - 

	if (graphElement == NULL)
		printf("Graph block not found!\n");
	else
	{
		char * rootID = (char*) graphElement->Attribute("rootid");
		graph->rootId=rootID;
		
		TiXmlElement *node=graphElement->FirstChildElement("node");

		while (node)
		{
			Node node1 = Node();
			
			char * displayListString = NULL;
			displayListString = (char*) node->Attribute("displaylist");

			if(displayListString != NULL){
				if(strcmp(displayListString, "true")==0)
					node1.hasDisplayList = true;
				else
					node1.hasDisplayList = false;
			}else
				node1.hasDisplayList = false;


			node1.id = node->Attribute("id");
			printf("Node id '%s' - Descendants:\n",node->Attribute("id"));
			
			//Animation ref
			TiXmlElement * animationref = node->FirstChildElement("animationref");

			if (animationref == NULL){
				printf("AnimationRef not found!\n");
			}else{
				string animId = "";
				animId = (string) animationref->Attribute("id");
				
				node1.animationRef=animId;

				if(animId != ""){
					node1.animation.push_back(&graph->animations.find(animId)->second);
				}
			}

			TiXmlElement * transforms = node->FirstChildElement("transforms");
			
			if (transforms == NULL)
				printf("Transforms block not found!\n");
			else 
			{
				TiXmlElement * tranformation = transforms->FirstChildElement("transform");

				char * valString = NULL;
				char * type = NULL;
				float m[4][4];

				glLoadIdentity();
				while(tranformation){

					type = (char *) tranformation->Attribute("type");

					if(strcmp(type, "translate") == 0){
						float x,y,z;
						valString = (char*) tranformation->Attribute("to");
						if(sscanf(valString,"%f %f %f",&x,&y,&z) == 3){
							glTranslatef(x,y,z);
						}
					}else if(strcmp(type, "rotate") == 0){
						float angle;
						char * axis = (char *) tranformation->Attribute("axis");
						valString = (char*) tranformation->Attribute("angle");
						if(sscanf(valString,"%f",&angle) == 1){
							if(axis[0] == 'x')
								glRotatef(angle,1,0,0);
							else if(axis[0] == 'y')
								glRotatef(angle,0,1,0);
							else if(axis[0] == 'z')
								glRotatef(angle,0,0,1);
							else
								printf("erro na rotação \n");
						}
					}else if(strcmp(type, "scale") == 0){
						float x,y,z;
						valString = (char*) tranformation->Attribute("factor");
						if(sscanf(valString,"%f %f %f",&x,&y,&z) == 3){
							glScalef(x,y,z);
						}
					}

					tranformation = tranformation->NextSiblingElement();
				}

				glGetFloatv(GL_MODELVIEW_MATRIX, &m[0][0]);


				for(unsigned int i=0; i<4; i++){
					for(unsigned int j=0; j<4; j++){
						node1.matrix[i*4+j]=m[i][j];
					}
				}
			}

			TiXmlElement * appearanceref = node->FirstChildElement("appearanceref");

			if (appearanceref == NULL)
				printf("Appearance block not found!\n");
			else{
				string id = (string) appearanceref->Attribute("id");
				node1.appRef=id;
				if(id.compare("inherit") != 0){
					Appearance *app = new Appearance();
					(*app) = graph->appearances.find(id)->second;
					node1.app=app;
				}
			}
			//Primitives
			TiXmlElement *primitives = node->FirstChildElement("primitives");
			if (primitives == NULL)
				printf("Primitives block not found!\n");
			else 
			{
				//Rectangles
				TiXmlElement *primitivesDef = primitives->FirstChildElement("rectangle");	
				
				while(primitivesDef){
					char* valString1 = (char *) primitivesDef->Attribute("xy1");
					char* valString2 = (char *) primitivesDef->Attribute("xy2");
					float x1,x2,y1,y2;
					if(sscanf(valString1,"%f %f",&x1,&y1) ==2 && sscanf(valString2,"%f %f",&x2,&y2) ==2){
						node1.primitives.push_back(new Rectangle("rectangle", x1, x2, y1, y2));
					}

					primitivesDef = primitivesDef->NextSiblingElement("rectangle");
				}

				//Triangles
				primitivesDef = primitives->FirstChildElement("triangle");

				while(primitivesDef){
					float x1,x2,x3,y1,y2,y3,z1,z2,z3;
					char * valString1 = (char *) primitivesDef->Attribute("xyz1");
					char * valString2 = (char *) primitivesDef->Attribute("xyz2");
					char * valString3 = (char *) primitivesDef->Attribute("xyz3");
					if(sscanf(valString1,"%f %f %f",&x1,&y1,&z1) ==3 && sscanf(valString2,"%f %f %f",&x2,&y2,&z2) ==3 && sscanf(valString3,"%f %f %f",&x3,&y3,&z3) ==3){
						node1.primitives.push_back(new Triangle("triangle", x1, x2, x3, y1, y2, y3, z1, z2, z3));
					}

					primitivesDef = primitivesDef->NextSiblingElement("triangle");
				}

				//Cylinder
				primitivesDef = primitives->FirstChildElement("cylinder");

				while(primitivesDef){
					char* valString1 = (char *) primitivesDef->Attribute("base");
					char* valString2 = (char *) primitivesDef->Attribute("top");
					char* stringHeigh = (char *) primitivesDef->Attribute("height");
					char* stringSlices = (char *) primitivesDef->Attribute("slices");
					char* stringStacks = (char *) primitivesDef->Attribute("stacks");
					float base, top, height;
					int slices, stacks;

					if(sscanf(valString1,"%f",&base) ==1 && sscanf(valString2,"%f",&top)==1 
						&& sscanf(stringHeigh,"%f",&height) == 1 && sscanf(stringSlices,"%d",&slices) == 1 && sscanf(stringStacks,"%d",&stacks) == 1 ){
							
							node1.primitives.push_back(new Cylinder("cylinder", base, top, height, slices, stacks));
					}
					primitivesDef = primitivesDef->NextSiblingElement("cylinder");
				}

				//Sphere
				primitivesDef = primitives->FirstChildElement("sphere");

				while(primitivesDef){
					char* valString1 = (char *) primitivesDef->Attribute("radius");
					char* stringSlices = (char *) primitivesDef->Attribute("slices");
					char* stringStacks = (char *) primitivesDef->Attribute("stacks");
					float radius;
					int slices,stacks;
					
					if(sscanf(valString1,"%f",&radius) ==1 && sscanf(stringSlices,"%d",&slices) == 1 && sscanf(stringStacks,"%d",&stacks) == 1 ){
						
						node1.primitives.push_back(new Sphere("sphere", radius, slices, stacks));
					}
					primitivesDef = primitivesDef->NextSiblingElement("sphere");
				}

				//Torus
				primitivesDef = primitives->FirstChildElement("torus");

				while(primitivesDef){
					char* valString1 = (char *) primitivesDef->Attribute("inner");
					char* valString2 = (char *) primitivesDef->Attribute("outer");
					char* stringSlices = (char *) primitivesDef->Attribute("slices");
					char* stringLoops = (char *) primitivesDef->Attribute("loops");
					float inner,outer;
					int slices,loops;
					
					if(sscanf(valString1,"%f",&inner) ==1 &&sscanf(valString2,"%f",&outer) ==1 && sscanf(stringSlices,"%d",&slices) == 1 && sscanf(stringLoops,"%d",&loops) == 1 ){
					
						node1.primitives.push_back(new Torus("torus", slices, loops, true, inner, outer));
					}

					primitivesDef = primitivesDef->NextSiblingElement("torus");
				}

				//Plane
				primitivesDef = primitives->FirstChildElement("plane");
				
				while(primitivesDef){
					char* partsString = (char *) primitivesDef->Attribute("parts");
					int parts = 1;
					
					if(sscanf(partsString,"%d",&parts) ==1){
					
						node1.primitives.push_back(new Plane("plane", parts));
					}

					primitivesDef = primitivesDef->NextSiblingElement("plane");
				}

				//Patch
				primitivesDef = primitives->FirstChildElement("patch");
				
				while(primitivesDef){
					char* orderString = (char *) primitivesDef->Attribute("order");
					char* partsUString = (char *) primitivesDef->Attribute("partsU");
					char* partsVString = (char *) primitivesDef->Attribute("partsV");
					string compute = (string) primitivesDef->Attribute("compute");
					int order , partsU, partsV;
					sscanf(orderString,"%d",&order);
					sscanf(partsUString,"%d",&partsU);
					sscanf(partsVString,"%d",&partsV);
					
					int numbOfPoints = (order+1)*(order+1);
					GLfloat* tempPoints = new GLfloat[numbOfPoints * 3];

					TiXmlElement *ctrlPoints = primitivesDef->FirstChildElement("controlpoint");

					for(int i = 0; i < numbOfPoints; i++) { //o ficheiro tem que ter todos os pontos de controlo senao para a aplicaçao
						char* xString = (char*) ctrlPoints->Attribute("x");
						sscanf(xString, "%f", &tempPoints[i*3 + 0]);
						char* yString = (char*) ctrlPoints->Attribute("y");
						sscanf(yString, "%f", &tempPoints[i*3 + 1]);
						char* zString = (char*) ctrlPoints->Attribute("z");
						sscanf(zString, "%f", &tempPoints[i*3 + 2]);

						ctrlPoints = ctrlPoints->NextSiblingElement("controlpoint");
					}


					GLfloat* cPoints = new GLfloat[numbOfPoints * 3];

					for(int i = 0; i < numbOfPoints; i++) {
						int temp = i%3;
						temp *= ((order+1)*(order+1));								
						temp += i/3 *3;

						cPoints[i*3 + 0] = tempPoints[temp + 0]; 
						cPoints[i*3 + 1] = tempPoints[temp + 1]; 
						cPoints[i*3 + 2] = tempPoints[temp + 2]; 
					}

					for(int i = 0; i < numbOfPoints; i++) {
						printf("%f %f %f\n", cPoints[i*3 + 0], cPoints[i*3 + 1], cPoints[i*3 + 2]);
					}

					if(graph->cullingOrder == "ccw")
						node1.primitives.push_back(new Patch("patch", order, partsU, partsV, compute, cPoints));
					else
						node1.primitives.push_back(new Patch("patch", order, partsU, partsV, compute, tempPoints));
					
					primitivesDef = primitivesDef->NextSiblingElement("patch");
				}

				//Vehicle
				primitivesDef = primitives->FirstChildElement("vehicle");
				
				while(primitivesDef){

					node1.primitives.push_back(new Vehicle("vehicle"));

					primitivesDef = primitivesDef->NextSiblingElement("vehicle");
				}

				//Flag
				primitivesDef = primitives->FirstChildElement("flag");
				
				while(primitivesDef){
					string texture = "";
					texture = (string) primitivesDef->Attribute("texture");
					Texture* text=NULL;
					
					for(unsigned int i=0; i<graph->textures.size(); i++){
						if(graph->textures[i].id == texture){
							text = &graph->textures[i];
							break;
						}
					}

					node1.primitives.push_back(new Flag("flag", texture, text));

					primitivesDef = primitivesDef->NextSiblingElement("flag");
				}
			}

			//Descendants
			TiXmlElement *descendants=node->FirstChildElement("descendants");
			if(descendants == NULL)
				printf("Descendants block not found!\n");
			else
			{
				TiXmlElement *child =descendants->FirstChildElement("noderef");
				if(child != NULL)
					while (child){
						char* nodeChild = (char *) child->Attribute("id");
						printf("Child: %s\n", nodeChild);
						node1.descendants.push_back(nodeChild);
						child = child->NextSiblingElement();
					}
			}
			node=node->NextSiblingElement();
			graph->nodes[node1.id]=node1;
		}
	}

	graph->updateDescendantNode();
	graph->updateRootNode();

}

ANFScene::~ANFScene()
{
	delete(doc);
}

//-------------------------------------------------------

TiXmlElement *ANFScene::findChildByAttribute(TiXmlElement *parent,const char * attr, const char *val)
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
