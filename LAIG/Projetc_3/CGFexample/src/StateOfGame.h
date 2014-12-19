#ifndef STATEOFGAME_H
#define STATEOFGAME_H

#include<vector>
#include<string>

using namespace std;

class Piece{
public:
	string type; //red or white
	int coordX, coordY; //from 1 to 7

	Piece(){};
	Piece(string type, int coordX, int coordY);
	~Piece(){};
};

class Fence{
public:
	int coordX, coordY; //from 1 to 6

	Fence(){};
	Fence(int coordX, int coordY);
	~Fence(){};
};

class StateOfGame{
public:
	vector<Piece> whitePieces, redPieces;
	vector<Fence> fences;
	string winner;


	StateOfGame();
	~StateOfGame(){};
	void prologParser(string state);
	StateOfGame & operator=(const StateOfGame & state); 
	
};

#endif