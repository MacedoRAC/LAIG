/* -*- Mode:Prolog; coding:iso-8859-1; -*- */
:-use_module(library(random)).
:-use_module(library(sockets)).

:-include('sockets.pl').
:-include('board.pl').
:-include('io.pl').

user:runtime_entry(start) :-
	server.

startGame(Size):-
        odd(Size),
        initialBoard(Board,Size),
        write('Select Player 1\n'),
        selectPlayer(P1),
        write('Select Player 2\n'),
        selectPlayer(P2),
        play(Board,Size,1,[P1,P2],FinalBoard),
        printGameOver(FinalBoard,Size).

/* Game Over Check */
play(Board,_,_,_,Board):-
        gameOver(Board).

/* Human Play */
play(Board,Size,PlayerNumber,Players,FinalBoard):-
        nth1(PlayerNumber,Players,1),
        printBoard(Board,Size),
        selectChoice(Choice),
        /* IF */
        (
           makePlay(Board,Size,PlayerNumber,1,ResultingBoard,Choice) -> 
        /* THEN */       
         (
            switchPlayer(PlayerNumber,NewPlayerNumber),
            play(ResultingBoard,Size,NewPlayerNumber,Players,FinalBoard)
         );
        /* ELSE */
         (
            write('Invalid play\nTry another one\n'),
            play(Board,Size,PlayerNumber,Players,FinalBoard)
         )
        ).

/* Dumb Computer Play */
play(Board,Size,PlayerNumber,Players,FinalBoard):-
        nth1(PlayerNumber,Players,2),
        !,
        random(1,3,Choice),
        (
           makePlay(Board,Size,PlayerNumber,2,ResultingBoard,Choice) ->
           (
              !,
              printBoard(ResultingBoard,Size),
              switchPlayer(PlayerNumber,NewPlayerNumber),
              play(ResultingBoard,Size,NewPlayerNumber,Players,FinalBoard)
           );
           (
              play(Board,Size,PlayerNumber,Players,FinalBoard)
           )
        ).

/* Smart Computer Play */
play(Board,Size,PlayerNumber,Players,FinalBoard):-
        nth1(PlayerNumber,Players,3),
        !,
        findall([TmpBoard,Xi,Yi,Xf,Yf,Dir],placeBarrier(Xi,Yi,Xf,Yf,Dir,Board,TmpBoard,PlayerNumber),Boards),
        evaluate_and_choose(Boards,(nil,-1000),ChosenBoard),
        
        findall([X,Y],aloneInArea(Board,X,Y,_),NClosedAreas),
        length(NClosedAreas,OriginalValue),
        
        findall([X,Y],aloneInArea(ChosenBoard,X,Y,_),NClosedAreas),
        length(NClosedAreas,NewValue),
        
        (
           OriginalValue == NewValue
        ) ->
        (
           random(1,3,Choice),
           (
                makePlay(Board,Size,PlayerNumber,2,ResultingBoard,Choice) ->
                (
                   !,
                   printBoard(ResultingBoard,Size),
                   switchPlayer(PlayerNumber,NewPlayerNumber),
                   play(ResultingBoard,Size,NewPlayerNumber,Players,FinalBoard)
                );
                (
                   play(Board,Size,PlayerNumber,Players,FinalBoard)
                )
           )
        );
        (
           !,
           printBoard(ChosenBoard,Size),
           switchPlayer(PlayerNumber,NewPlayerNumber),
           play(ChosenBoard,Size,NewPlayerNumber,Players,FinalBoard)
        ).
        

evaluate_and_choose([],[Board,_],Board).
evaluate_and_choose([List|Lists],Record,BestBoard):-
        nth0(0,List,Board),
        findall([X,Y],aloneInArea(Board,X,Y,_),NClosedAreas),
        length(NClosedAreas,Value),
        update(Board,Value,Record,NewRecord),
        evaluate_and_choose(Lists,NewRecord,BestBoard).

update(_,Value,[OldBoard,OldValue],[OldBoard,OldValue]):-
        Value =< OldValue.
update(Board,Value,[_,OldValue],[Board,Value]):-
        Value > OldValue.

/* Human Play - Move a piece and place a new barrier */
makePlay(Board,_,PlayerNumber,1,ResultingBoard,1):-
        write('Select the piece you wish to move\n'),
        receiveXY(Xi,Yi),
        write('Select where you wish to move\n'),
        receiveXY(Xf,Yf),
        write('Select the direction where to place the Barrier\n1 - Up\t2 - Right\t3 - Down\t4 - Left\n'),
        read(Dir),
        placeBarrier(Xi,Yi,Xf,Yf,Dir,Board,ResultingBoard,PlayerNumber).

/* Human Play - Place a new Piece */
makePlay(Board,Size,PlayerNumber,1,ResultingBoard,2):-
        write('Select where you wish to play the new Piece\n'),
        receiveXY(X,Y),
        placeNewPiece(X,Y,PlayerNumber,Board,ResultingBoard,Size).

/* Dumb Computer Play - Move a piece and place a new barrier */
makePlay(Board,Size,PlayerNumber,2,ResultingBoard,1):-
        Size1 is Size + 1,
        random(1,Size1,Xi),
        random(1,Size1,Yi),
        random(1,Size1,Xf),
        random(1,Size1,Yf),
        random(1,5,Dir),
        placeBarrier(Xi,Yi,Xf,Yf,Dir,Board,ResultingBoard,PlayerNumber).
        

/* Dumb Computer Play - Place a new Piece */
makePlay(Board,Size,PlayerNumber,2,ResultingBoard,2):-
        Size1 is Size + 1,
        random(1,Size1,X),
        random(1,Size1,Y),
        placeNewPiece(X,Y,PlayerNumber,Board,ResultingBoard,Size).
        
        
selectChoice(Choice):-
        write('You can:\n1 - Move a piece and place a new barrier;\n2 - Place a new piece\n'),
        read(TMPChoice),
        (
           (TMPChoice == 1 ; TMPChoice == 2) ->
           (
              Choice is TMPChoice,
              true
           );
           (
              selectChoice(Choice)
           )
        ).

selectPlayer(Player):-
        write('1 - Human\n2 - Dumb Computer\n'),
        read(TMPPlayer),
        (
           (TMPPlayer == 1; TMPPlayer == 2) ->
           (
              Player is TMPPlayer,
              true
           );
           (
              selectPlayer(Player)
           )
        ).

calculatePontuations(Board, Player1Score, Player2Score):-
	findall([X|Y],selectPiece(Board,X,Y,1),ListOfPlayer1Pieces),
    floodList(Board,ListOfPlayer1Pieces,'1',Player1Flooded),
    findall([X|Y],selectPiece(Board,X,Y,2),ListOfPlayer2Pieces),
    floodList(Player1Flooded,ListOfPlayer2Pieces,'2',Player2Flooded),
    !,
    sumAreas(Player2Flooded,0,0,Size,Player1Score,Player2Score).

printGameOver(Board,Size):-
        
        findall([X|Y],selectPiece(Board,X,Y,1),ListOfPlayer1Pieces),
        floodList(Board,ListOfPlayer1Pieces,'1',Player1Flooded),
        findall([X|Y],selectPiece(Board,X,Y,2),ListOfPlayer2Pieces),
        floodList(Player1Flooded,ListOfPlayer2Pieces,'2',Player2Flooded),
        !,
        sumAreas(Player2Flooded,0,0,Size,Player1Score,Player2Score),
        
        nl,
        write('Player 1 score is '),
        write(Player1Score),
        nl,
        write('Player 2 score is '),
        write(Player2Score),
        nl,nl.

instructions :-
        nl,
        write('Enter commands using standard Prolog syntax.'), nl,
        write('Available commands are:'), nl,
        write('startGame(Size)                          -- to start the game.'), nl,
        write('placePiece(X,Y).                         -- to place a new piece.'), nl,
        write('placeBarrier(Xi,Yi,Xf,Yf,Direction).     -- to place a new barrier.'), nl,
        write('instructions.                            -- to see this message again.'), nl,
        write('halt.                                    -- to end the game and quit.'), nl,
        nl.

placeNewPiece(X,Y,Player,Board,ResultingBoard,Size):-
        write('Place New Piece '), write(X), write(' '), write(Y), nl,
        numberPieces(Board,0,Player,NPieces),
        !,
        (
           NPieces < Size
        ) ->
        (
        
        /* Convert the Input */
        outToIn(X,Xf),
        outToIn(Y,Yf),
        
        !,
        selectPiece(Board,Xi,Yi,Player),
        
        /* Verify if the destination is empty */
        selectPiece(Board,Xf,Yf,e),
        validMove(Board,Xi,Yi,Xf,Yf,Player),
        
        /* Place the Player piece in the Board */
        placePiece(Xf,Yf,Player,Board,ResultingBoard)
        );
        (
           fail
        ).
        
        

placeBarrier(Xi,Yi,Xf,Yf,1, Board, ResultingBoard, Player):-
        write('Place Barrier at '), write(Xf), write(' '), write(Yf), write(' '), write(' from  '), write(Xi), write(' '), write(Yi), nl,
        /* Convert the Input */
        outToIn(Xi,Xii),
        outToIn(Xf,Xff),
        outToIn(Yi,Yii),
        outToIn(Yf,Yff),

        BarrierX is Xff,
        BarrierY is Yff - 1,
                
        /* Verify if the destination is empty */
        selectPiece(Board,Xff,Yff,e),
        selectPiece(Board,Xii,Yii,Player),
        selectPiece(Board,BarrierX,BarrierY,e),
        validMove(Board,Xii,Yii,Xff,Yff,Player),
        
        /* Move the Player piece in the Board and place a Barrier*/
        placePiece(Xff,Yff,Player,Board,NewBoard01),
        placePiece(Xii,Yii,'e',NewBoard01,NewBoard02),
        placePiece(BarrierX,BarrierY,'x',NewBoard02,ResultingBoard),
        validBoard(ResultingBoard).

placeBarrier(Xi,Yi,Xf,Yf,3, Board, ResultingBoard, Player):-
        write('Place Barrier at '), write(Xf), write(' '), write(Yf), write(' '), write(' from  '), write(Xi), write(' '), write(Yi), nl,
        
        /* Convert the Input */
        outToIn(Xi,Xii),
        outToIn(Xf,Xff),
        outToIn(Yi,Yii),
        outToIn(Yf,Yff),

        BarrierX is Xff,
        BarrierY is Yff + 1,
                
        /* Verify if the destination is empty */
        selectPiece(Board,Xff,Yff,e),
        selectPiece(Board,Xii,Yii,Player),
        selectPiece(Board,BarrierX,BarrierY,e),
        validMove(Board,Xii,Yii,Xff,Yff,Player),
        
        /* Move the Player piece in the Board and place a Barrier*/
        placePiece(Xff,Yff,Player,Board,NewBoard01),
        placePiece(Xii,Yii,'e',NewBoard01,NewBoard02),
        placePiece(BarrierX,BarrierY,'x',NewBoard02,ResultingBoard),
        validBoard(ResultingBoard).

placeBarrier(Xi,Yi,Xf,Yf,4, Board, ResultingBoard, Player):-
        write('Place Barrier at '), write(Xf), write(' '), write(Yf), write(' '), write(' from  '), write(Xi), write(' '), write(Yi), nl,
        
        /* Convert the Input */
        outToIn(Xi,Xii),
        outToIn(Xf,Xff),
        outToIn(Yi,Yii),
        outToIn(Yf,Yff),

        BarrierX is Xff - 1,
        BarrierY is Yff,
                
        /* Verify if the destination is empty */
        selectPiece(Board,Xff,Yff,e),
        selectPiece(Board,Xii,Yii,Player),
        selectPiece(Board,BarrierX,BarrierY,e),
        validMove(Board,Xii,Yii,Xff,Yff,Player),
        
        /* Move the Player piece in the Board and place a Barrier*/
        placePiece(Xff,Yff,Player,Board,NewBoard01),
        placePiece(Xii,Yii,'e',NewBoard01,NewBoard02),
        placePiece(BarrierX,BarrierY,'x',NewBoard02,ResultingBoard),
        validBoard(ResultingBoard).

placeBarrier(Xi,Yi,Xf,Yf,2, Board, ResultingBoard, Player):-
        write('Place Barrier at '), write(Xf), write(' '), write(Yf), write(' '), write(' from  '), write(Xi), write(' '), write(Yi), nl,
        
        /* Convert the Input */
        outToIn(Xi,Xii),
        outToIn(Xf,Xff),
        outToIn(Yi,Yii),
        outToIn(Yf,Yff),

        BarrierX is Xff + 1,
        BarrierY is Yff,
                
        /* Verify if the destination is empty */
        selectPiece(Board,Xff,Yff,e),
        selectPiece(Board,Xii,Yii,Player),
        selectPiece(Board,BarrierX,BarrierY,e),
        validMove(Board,Xii,Yii,Xff,Yff,Player),
        
        /* Move the Player piece in the Board and place a Barrier*/
        placePiece(Xff,Yff,Player,Board,NewBoard01),
        placePiece(Xii,Yii,'e',NewBoard01,NewBoard02),
        placePiece(BarrierX,BarrierY,'x',NewBoard02,ResultingBoard),
        validBoard(ResultingBoard).

floodList(Board,[],_,Board).

floodList(Board,[[X|Y]|RestOfList],Piece,FloodedBoard):-
        floodFill(Board,X,Y,Piece,ResultingBoard),
        floodList(ResultingBoard,RestOfList,Piece,FloodedBoard).
        
checkListArea(_,[]).

checkListArea(Board,[[X|Y]|RestOfList]):-
        checkArea(Board,X,Y),
        checkListArea(Board,RestOfList).

checkArea(Board,X,Y):-
        placePiece(X,Y,'s',Board,NewBoard),!,
        aloneInArea(NewBoard,X,Y,_).

validBoard(Board):-
        (
           gameOver(Board)
        ->
           (true)
        ;
           (
              /* Find the open area */
              (selectPiece(Board,X,Y,1);selectPiece(Board,X,Y,2)),
              \+checkArea(Board,X,Y),
              floodFill(Board,X,Y,'o',FilledBoard),
              !,
              
              /* Flood the closed Areas */
              findall([X|Y],(selectPiece(FilledBoard,X,Y,1);selectPiece(FilledBoard,X,Y,2)),ListOfPieces),
              floodList(FilledBoard,ListOfPieces,'c',FloodedBoard),
              \+ selectPiece(FloodedBoard,_,_,e)
                 )
        ).

gameOver(Board):-
        findall([X|Y],(selectPiece(Board,X,Y,1);selectPiece(Board,X,Y,2)),ListOfPieces),
        !,
        checkListArea(Board,ListOfPieces),
        floodList(Board,ListOfPieces,'c',FloodedBoard),
        \+ selectPiece(FloodedBoard,_,_,e).

switchPlayer(1,2).
switchPlayer(2,1).