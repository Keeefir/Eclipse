//============================================================================
// Name        : 15game.cpp
// Author      : Lomanov Vasiliy
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "string.h"
#include <stdlib.h>
#include "Game.h"
#include <list>
using namespace std;

typedef GameField<3,3> Game15;

///some change
///one more change

class EvristicNotOwnPlace{
	Game15 Solve;

public:
	int GetMesure(Game15 & Game)
	{
		int Mesure=0;
		for(int x=0;x<3;x++)
			for(int y=0;y<3;y++)
			{
				if( Game.Field[y][x]!= y*3+x )
				{
					div_t temp=div( Game.Field[y][x],3 );
					Mesure+=abs( x - temp.rem );
					Mesure+=abs( y - temp.quot ) ;
				}
			}
		return Mesure;
	}

};

class RandomSolver{
	EvristicNotOwnPlace Evristic;
public:
	void SolveGame(Game15 & Game)
	{
		int NumOfIterations=0;
		while(Evristic.GetMesure(Game)!=0)
		{
			int RandomMove=rand()%4;
			switch( RandomMove )
			{
			case 0:
				Game.MoveEmptyCeilUp();
				break;
			case 1:
				Game.MoveEmptyCeilDown();
				break;
			case 2:
				Game.MoveEmptyCeilLeft();
				break;
			case 3:
				Game.MoveEmptyCeilRight();
				break;
			}
			++NumOfIterations;
		}
		cout<<"Решение \n"<<Game<<"найдено за "<<NumOfIterations<<" итераций"<<endl;
	}
};

int main() {

	Game15 Game;

	RandomSolver Solve;

	Game.RandomFill();

	cout<<"Игра\n"<<Game<<endl;

	Solve.SolveGame(Game);


	return 0;
}
