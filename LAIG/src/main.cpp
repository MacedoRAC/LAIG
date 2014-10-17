#include <iostream>
#include <exception>
#include <ctime>

#include "XMLScene.h"
#include "CGFapplication.h"

int main(int argc, char* argv[]){
	srand(time(NULL));

	CGFapplication app = CGFapplication();

	try {
		app.init(&argc, argv);

		app.setScene(new XMLScene(argv[1]));
		app.setInterface(new CGFinterface());

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