#include "CGFapplication.h"
#include "ANFScene.h"
//#include "GL/glu.h"


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
					if(type == "ambient")
					{
						newLight.ambient[0]=r;
						newLight.ambient[1]=g;
						newLight.ambient[2]=b;
						newLight.ambient[3]=a;
					}
					else if(type == "diffuse")
					{
						newLight.diffuse[0]=r;
						newLight.diffuse[1]=g;
						newLight.diffuse[2]=b;
						newLight.diffuse[3]=a;
					}
					else if(type == "specular")
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
				Texture texture;
				texture.id=id;
				texture.file=file;
				texture.texlength_S=texlength_S;
				texture.texlength_T=texlength_T;
				
				graph->textures.push_back(texture);	
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

		while(appearances)
		{
			float shininess = 0;
			char * id = (char*) appearances->Attribute("id");
			char * shininessString = (char*) appearances->Attribute("shininess");
			string textureref = appearances->Attribute("textureref");			

			sscanf(shininessString,"%f",&shininess);

			Appearance newAppearance;
			newAppearance.id=id;
			newAppearance.shininess=shininess;
			newAppearance.textureref=textureref;

			TiXmlElement * appearanceComponent = appearances->FirstChildElement("component");

			//initialize all light components with zero
			newAppearance.ambient_comp.push_back(0);
			newAppearance.ambient_comp.push_back(0);
			newAppearance.ambient_comp.push_back(0);
			newAppearance.ambient_comp.push_back(0);
			newAppearance.diffuse_comp=newAppearance.ambient_comp;
			newAppearance.specular_comp=newAppearance.ambient_comp;

			while(appearanceComponent)
			{
				float r,g,b,a;
				string type = appearanceComponent->Attribute("type");
				char * valString = (char*) appearanceComponent->Attribute("value");

				if(sscanf(valString,"%f %f %f %f",&r,&g,&b,&a) == 4)
				{
					if(type.compare("ambient")==0)
					{
						newAppearance.ambient_comp[0]=r;
						newAppearance.ambient_comp[1]=g;
						newAppearance.ambient_comp[2]=b;
						newAppearance.ambient_comp[3]=a;
					}
					else if(type.compare("diffuse")==0)
					{
						newAppearance.diffuse_comp[0]=r;
						newAppearance.diffuse_comp[1]=g;
						newAppearance.diffuse_comp[2]=b;
						newAppearance.diffuse_comp[3]=a;
					}
					else if(type.compare("specular")==0)
					{
						newAppearance.specular_comp[0]=r;
						newAppearance.specular_comp[1]=g;
						newAppearance.specular_comp[2]=b;
						newAppearance.specular_comp[3]=a;
					}

				}

				for(unsigned int j= 0 ; j < graph->textures.size();j++){
				
					if (textureref.compare(graph->textures[j].id)==0)
					{
						Texture * text = new Texture();
						(*text) = graph->textures[j];
						newAppearance.texture=text;
					}
				}
				appearanceComponent = appearanceComponent->NextSiblingElement();
				
			}

			graph->appearances[newAppearance.id]=newAppearance;

			appearances = appearances->NextSiblingElement();
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
		char *prefix="  -";
		
		TiXmlElement *node=graphElement->FirstChildElement("node");

		while (node)
		{
			Node node1 = Node();
			node1.id = node->Attribute("id");
			printf("Node id '%s' - Descendants:\n",node->Attribute("id"));
			
			TiXmlElement * transforms = node->FirstChildElement("transforms");
			
			if (transforms == NULL)
				printf("Transforms block not found!\n");
			else 
			{
				TiXmlElement * tranformation = transforms->FirstChildElement("transform");

				char * valString = NULL, * type = NULL;
				float m[4][4];

				glLoadIdentity();
				while(tranformation){

					type = (char *) tranformation->Attribute("type");

					if(type == "translate"){
						float x,y,z;
						valString = (char*) tranformation->Attribute("to");
						if(sscanf(valString,"%f %f %f",&x,&y,&z) == 3){
							glTranslatef(x,y,z);
						}
					}else if(type == "rotate"){
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
					}else if(type == "scale"){
						float x,y,z;
						valString = (char*) tranformation->Attribute("factor");
						if(sscanf(valString,"%f %f %f",&x,&y,&z) == 3){
							glScalef(x,y,z);
						}
					}

					tranformation = tranformation->NextSiblingElement();
				}

				glGetFloatv(GL_MODELVIEW_MATRIX, &m[0][0]);

				unsigned int a=0;
				for(unsigned int i=0; i<4; i++){
					for(unsigned int j=0; j<4; j++){
						node1.matrix[a]=m[i][j];
						a++;  // a variavel a nao precisa de ser verificada por um ciclo uma vez que ambos ciclos, em conjunto o fazem
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
				Rectangle rect = Rectangle();
				
				while(primitivesDef){
					rect.type="rectangle";

					char* valString1 = (char *) primitivesDef->Attribute("xy1");
					char* valString2 = (char *) primitivesDef->Attribute("xy2");
					float x1,x2,y1,y2;
					if(sscanf(valString1,"%f %f",&x1,&y1) ==2 && sscanf(valString2,"%f %f",&x2,&y2) ==2){
						rect.x1=x1;
						rect.x2=x2;
						rect.y1=y1;
						rect.y2=y2;

						node1.primitives.push_back(&rect);
					}


					primitivesDef = primitivesDef->NextSiblingElement("rectangle");
				}

				//Triangles
				primitivesDef = primitives->FirstChildElement("triangle");
				Triangle tria = Triangle();

				while(primitivesDef){
					tria.type="triangle";

					float x1,x2,x3,y1,y2,y3,z1,z2,z3;
					char * valString1 = (char *) primitivesDef->Attribute("xyz1");
					char * valString2 = (char *) primitivesDef->Attribute("xyz2");
					char * valString3 = (char *) primitivesDef->Attribute("xyz3");
					if(sscanf(valString1,"%f %f %f",&x1,&y1,&z1) ==3 && sscanf(valString2,"%f %f %f",&x2,&y2,&z2) ==3 && sscanf(valString3,"%f %f %f",&x3,&y3,&z3) ==3){
						tria.x1=x1;
						tria.y1=y1;
						tria.z1=z1;
						tria.x2=x2;
						tria.y2=y2;
						tria.z2=z2;
						tria.x3=x3;
						tria.y3=y3;
						tria.z3=z3;

						node1.primitives.push_back(&tria);
					}

					primitivesDef = primitivesDef->NextSiblingElement("triangle");
				}

				//Cylinder
				primitivesDef = primitives->FirstChildElement("cylinder");
				Cylinder cyl = Cylinder();

				while(primitivesDef){
					cyl.type="cylinder";

					char* valString1 = (char *) primitivesDef->Attribute("base");
					char* valString2 = (char *) primitivesDef->Attribute("top");
					char* stringHeigh = (char *) primitivesDef->Attribute("height");
					char* stringSlices = (char *) primitivesDef->Attribute("slices");
					char* stringStacks = (char *) primitivesDef->Attribute("stacks");
					float base, top, height;
					int slices, stacks;

					if(sscanf(valString1,"%f",&base) ==1 && sscanf(valString2,"%f",&top)==1 
						&& sscanf(stringHeigh,"%f",&height) == 1 && sscanf(stringSlices,"%d",&slices) == 1 && sscanf(stringStacks,"%d",&stacks) == 1 ){
						
							cyl.base=base;
							cyl.top=top;
							cyl.height=height;
							cyl.slices=slices;
							cyl.stacks=stacks;

							node1.primitives.push_back(&cyl);
					}
					primitivesDef = primitivesDef->NextSiblingElement("cylinder");
				}

				//Sphere
				primitivesDef = primitives->FirstChildElement("sphere");
				Sphere sphere = Sphere();

				while(primitivesDef){
					sphere.type="sphere";

					char* valString1 = (char *) primitivesDef->Attribute("radius");
					char* stringSlices = (char *) primitivesDef->Attribute("slices");
					char* stringStacks = (char *) primitivesDef->Attribute("stacks");
					float radius;
					int slices,stacks;
					
					if(sscanf(valString1,"%f",&radius) ==1 && sscanf(stringSlices,"%d",&slices) == 1 && sscanf(stringStacks,"%d",&stacks) == 1 ){
						sphere.radius=radius;
						sphere.slices=slices;
						sphere.stacks=stacks;
						
						node1.primitives.push_back(&sphere);
					}
					primitivesDef = primitivesDef->NextSiblingElement("sphere");
				}

				//Torus
				primitivesDef = primitives->FirstChildElement("torus");
				Torus torus = Torus();

				while(primitivesDef){
					torus.type="torus";

					char* valString1 = (char *) primitivesDef->Attribute("inner");
					char* valString2 = (char *) primitivesDef->Attribute("outer");
					char* stringSlices = (char *) primitivesDef->Attribute("slices");
					char* stringLoops = (char *) primitivesDef->Attribute("loops");
					float inner,outer;
					int slices,loops;
					
					if(sscanf(valString1,"%f",&inner) ==1 &&sscanf(valString2,"%f",&outer) ==1 && sscanf(stringSlices,"%d",&slices) == 1 && sscanf(stringLoops,"%d",&loops) == 1 ){
						torus.inner=inner;
						torus.outer=outer;
						torus.slices=slices;
						torus.loops=loops;
						
						node1.primitives.push_back(&torus);
					}

					primitivesDef = primitivesDef->NextSiblingElement("torus");
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
