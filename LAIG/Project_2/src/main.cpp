#include <iostream>
#include <exception>

#include "CGFapplication.h"
#include "GUI.h"
#include "Scene.h"
#include "ANFScene.h"
#include "Graph.h"

using namespace std;


int main(int argc, char* argv[]) {

	Graph * pgraph = new Graph();

	char* filename = "linearAnimTest.xml";
	if (argc >1)
		filename = argv[1];
	CGFapplication app = CGFapplication();
	Scene * scene = new Scene();
	try {
		app.init(&argc, argv);
		ANFScene n = ANFScene(filename,pgraph);
		printf("\n\n\n ja leu o ANF \n\n\n");
		scene->graph=pgraph;
		app.setScene(scene);
		app.setInterface(new GUI(scene->graph));
		
		app.run();
	}
	catch(GLexception& ex) {
		cout << "Erro: " << ex.what();
		return -1;
	}
	catch(exception& ex) {
		cout << "Erro inesperado: " << ex.what();
		return -1;
	}

	return 0;
}