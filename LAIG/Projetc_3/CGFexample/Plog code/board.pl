/* -*- Mode:Prolog; coding:iso-8859-1; -*- */

:-use_module(library(lists)).

:-include('math.pl').

traduz(e,' ').
traduz(x,'X').
traduz(1,'1').
traduz(2,'2').
traduz(o,'O').
traduz(c,'C').

sumAreas([_,[]],Score1,Score2,_,Score1,Score2).

sumAreas([_|[Line|Board]],Score1,Score2,Size,Score1Out,Score2Out):-
        InSize is ((Size*2) +1),
        sumAreasLine(Line,1,Score1,Score2,InSize,Score1Tmp,Score2Tmp),
        sumAreas(Board,Score1Tmp,Score2Tmp,Size,Score1Out,Score2Out).

sumAreasLine(_,X,Score1,Score2,Size,Score1,Score2):-
        X >= Size.

sumAreasLine(Line,X,Score1,Score2,Size,Score1Out,Score2Out):-
        nth0(X,Line,'1'),
        NewX is X + 2,
        sumAreasLine(Line,NewX,Score1,Score2,Size,Score1Tmp,Score2Out),
        Score1Out is Score1Tmp + 1.

sumAreasLine(Line,X,Score1,Score2,Size,Score1Out,Score2Out):-
        nth0(X,Line,'2'),
        NewX is X + 2,
        sumAreasLine(Line,NewX,Score1,Score2,Size,Score1Out,Score2Tmp),
        Score2Out is Score2Tmp + 1.

numberPieces([],Count,_,Count).

numberPieces([Line|Board],Count,Piece,CountOut):-
        numberPiecesLine(Line,Count,Piece,TMPCount),
        numberPieces(Board,TMPCount,Piece,CountOut).

numberPiecesLine([],Count,_,Count).

numberPiecesLine([Piece|Line],Count,Piece,CountOut):-
        TMPCount is Count + 1,
        numberPiecesLine(Line,TMPCount,Piece,CountOut).

numberPiecesLine([_|Board],Count,Piece,CountOut):-
        numberPiecesLine(Board,Count,Piece,CountOut).

aloneInArea(Board,X,Y,Board):-
        selectPiece(Board,X,Y,1),!,fail.

aloneInArea(Board,X,Y,Board):-
        selectPiece(Board,X,Y,2),!,fail.

aloneInArea(Board,X,Y,Board):-
        selectPiece(Board,X,Y,x).

aloneInArea(Board,X,Y,Board):-
        selectPiece(Board,X,Y,o).

aloneInArea(Board,X,Y,ResultingBoard):-
        placePiece(X,Y,'o',Board,NewBoard),
        !,
        Up is Y - 1,
        Down is Y + 1,
        Left is X - 1,
        Right is X + 1,
        aloneInArea(NewBoard,Right,Y,ResultingBoard01),!,
        aloneInArea(ResultingBoard01,X,Up,ResultingBoard02),!,
        aloneInArea(ResultingBoard02,Left,Y,ResultingBoard03),!,
        aloneInArea(ResultingBoard03,X,Down,ResultingBoard).
        
floodFill(Board,X,Y,_,Board):-
        selectPiece(Board,X,Y,x).

floodFill(Board,X,Y,Piece,Board):-
        selectPiece(Board,X,Y,Piece).

floodFill(Board,X,Y,Piece,ResultingBoard):-
        placePiece(X,Y,Piece,Board,NewBoard),
        Up is Y - 1,
        Down is Y + 1,
        Left is X - 1,
        Right is X + 1,
        floodFill(NewBoard,X,Up,Piece,ResultingBoard01),
        floodFill(ResultingBoard01,X,Down,Piece,ResultingBoard02),
        floodFill(ResultingBoard02,Left,Y,Piece,ResultingBoard03),
        floodFill(ResultingBoard03,Right,Y,Piece,ResultingBoard).

validMove(Board,Xi,Yi,Xf,Yf,Player):-
        selectPiece(Board,Xi,Yi,Player),
        validMoveX(Board,Xi,Yi,Xf,Yf).

validMove(Board,Xi,Yi,Xf,Yf,Player):-
        selectPiece(Board,Xi,Yi,Player),
        validMoveY(Board,Xi,Yi,Xf,Yf).

validMoveX(Board,X,Y,X,Y):-
        selectPiece(Board,X,Y,e).
validMoveX(Board,X,Yi,X,Yf):-
        Yi < Yf,
        NewYi is Yi + 1,
        selectPiece(Board,X,NewYi,e),
        validMoveX(Board,X,NewYi,X,Yf).
validMoveX(Board,X,Yi,X,Yf):-
        Yi > Yf,
        NewYi is Yi - 1,
        selectPiece(Board,X,NewYi,e),
        validMoveX(Board,X,NewYi,X,Yf).
validMoveX(Board,Xi,Yi,Xf,Yf):-
        Xi < Xf,
        NewXi is Xi + 1,
        selectPiece(Board,NewXi,Yi,e),
        validMoveX(Board,NewXi,Yi,Xf,Yf).
validMoveX(Board,Xi,Yi,Xf,Yf):-
        Xi > Xf,
        NewXi is Xi - 1,
        selectPiece(Board,NewXi,Yi,e),
        validMoveX(Board,NewXi,Yi,Xf,Yf).

validMoveY(Board,X,Y,X,Y):-
        selectPiece(Board,X,Y,e).
validMoveY(Board,Xi,Y,Xf,Y):-
        Xi < Xf,
        NewXi is Xi + 1,
        selectPiece(Board,NewXi,Y,e),
        validMoveY(Board,NewXi,Y,Xf,Y).
validMoveY(Board,Xi,Y,Xf,Y):-
        Xi > Xf,
        NewXi is Xi - 1,
        selectPiece(Board,NewXi,Y,e),
        validMoveY(Board,NewXi,Y,Xf,Y).
validMoveY(Board,Xi,Yi,Xf,Yf):-
        Yi < Yf,
        NewYi is Yi + 1,
        selectPiece(Board,Xi,NewYi,e),
        validMoveY(Board,Xi,NewYi,Xf,Yf).
validMoveY(Board,Xi,Yi,Xf,Yf):-
        Yi > Yf,
        NewYi is Yi - 1,
        selectPiece(Board,Xi,NewYi,e),
        validMoveY(Board,Xi,NewYi,Xf,Yf).


selectPiece([],_,_,[]).
selectPiece(Board,X,Y,Piece):-
        nth0(Y,Board,Line),
        nth0(X,Line,Piece).

placePiece(X,Y,NewPiece,OldBoard,NewBoard):-
        nth0(Y,OldBoard,Line),
        placeElementNth(0,X,NewPiece,Line,NewLine),
        placeElementNth(0,Y,NewLine,OldBoard,NewBoard).

placeElementNth(_,_,_,[],[]).
placeElementNth(X,X,NewPiece,[_|T1],[NewPiece|T2]):-
        !,
        NewX is X + 1,
        placeElementNth(NewX,X,NewPiece,T1,T2).
placeElementNth(CurrentX,X,NewPiece,[H|T1],[H|T2]):-
        NewX is CurrentX + 1,
        placeElementNth(NewX,X,NewPiece,T1,T2).

printBoard(C,Size):- 
        InSize is (Size * 2) + 1,
        print('\t  '),
        printFirstLine(InSize),
        print('\n'),
        printBoard(C, 0, InSize).
printBoard([],_,_).
printBoard(_,Size,Size).
printBoard([C|R], N, Size) :-
        N =< Size,
        odd(N) -> inToOut(N, Np),
                  print(Np),
                  print('\t'),
                  Nn is N+1,
                  printBoardLine(C),
                  print('\n'),
                  printBoard(R,Nn,Size);                   
        even(N) ->print('\t'),
                  Nn is N+1,
                  printBoardLine(C),nl,
                  printBoard(R,Nn,Size).

printBoardLine([]).
printBoardLine([C|R]) :-
        traduz(C,CImp),
        write(CImp),
        write(' '),
        printBoardLine(R).

printFirstLine(S):- 
        inToOut(S,R),
        printFirstLine(R,1).
printFirstLine(S,N):-
                  N is S -1,
                  print(N).
printFirstLine(S, N):-
        N < 10,
        print(N),
        Nn is N+1,
        print('   '),
        printFirstLine(S,Nn).
printFirstLine(S, N):-
        print(N),
        Nn is N+1,
        print('  '),
        printFirstLine(S,Nn).


initialBoard(Board,Size):-
        InSize is (Size * 2) + 1,
        make_board(0,Board,InSize).

make_board(Size,Board,Size):-
        make_line(Size,Size,Size,Line),
        Board = [Line].

make_board(Y,[Line | Board],Size):-
        make_line(0,Y,Size,Line),
        Yn is Y + 1,
        make_board(Yn,Board,Size).

make_line(Size,_,Size,[]).

make_line(X,Y,Size,[Piece|Line]):-
        Xn is X + 1,
        getInitialPiece(X,Y,Size,Piece),
        make_line(Xn,Y,Size,Line).

getInitialPiece(0,_,_,x).
getInitialPiece(_,0,_,x).
getInitialPiece(X,_,Size,x):-
        X is Size -1.
getInitialPiece(_,Y,Size,x):-
        Y is Size -1.
getInitialPiece(X,Y,Size,1):-
        X is 1,
        Y is floor(Size/2).
getInitialPiece(X,Y,Size,2):-
        X is Size - 2,
        Y is floor(Size/2).
getInitialPiece(X,Y,_,x):-
        even(X),
        even(Y).
getInitialPiece(_,_,_,e).