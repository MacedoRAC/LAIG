/* -*- Mode:Prolog; coding:iso-8859-1; -*- */

odd(N):- 1 is (N mod 2).
even(N):- 0 is (N mod 2).

outToIn(N, R):- R is ((N*2) - 1).
inToOut(N, R):- R is ceiling(N/2).