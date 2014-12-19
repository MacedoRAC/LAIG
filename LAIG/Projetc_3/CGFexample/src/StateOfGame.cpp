#include"StateOfGame.h"


Piece::Piece(string type, int coordX, int coordY){
	this->type=type;
	this->coordX=coordX;
	this->coordY=coordY;
}

Fence::Fence(int coordX, int coordY){
	this->coordX=coordX;
	this->coordY=coordY;
}

StateOfGame:: StateOfGame(){
	this->winner="";
}

void StateOfGame::prologParser(string state){

}

StateOfGame& StateOfGame:: operator=(const StateOfGame & state){
	this->whitePieces = state.whitePieces;
	this->redPieces = state.redPieces;
	this->fences = state.fences;
	this->winner = state.winner;

	return *this;
}