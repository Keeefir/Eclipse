/*
 * Game.h
 *
 *  Created on: 23.09.2012
 *      Author: Lag
 */

#ifndef GAME_H_
#define GAME_H_

#include <iostream>

using std::ostream;
using std::endl;

template<int Width,int Height>
class GameField{



	typedef struct{
		int x;
		int y;
	}Ceil;


	Ceil EmptyCeil;
public:
	int Field[Height][Width];

	GameField()
	{
		EmptyCeil.x=0;
		EmptyCeil.y=0;
		for(int y=0;y<Height;y++)
		{
			for(int x=0;x<Width;x++)
			{
				Field[y][x]=y*Width+x;
			}
		}
	}
	void RandomFill()
	{
		srand(time(0));

		int Loops=1000+rand()%1000;
		for(int i=0;i<Loops;i++)
		{
			int RandomMove=rand()%4;
			switch( RandomMove )
			{
			case 0:
				MoveEmptyCeilUp();
				break;
			case 1:
				MoveEmptyCeilDown();
				break;
			case 2:
				MoveEmptyCeilLeft();
				break;
			case 3:
				MoveEmptyCeilRight();
				break;
			}
		}

	}


	void MoveEmptyCeilUp(){
		if( EmptyCeil.y-1 >= 0 )
		{
			std::swap( Field[EmptyCeil.y][EmptyCeil.x] , Field[EmptyCeil.y-1][EmptyCeil.x] );
			-- EmptyCeil.y;
		}
	}
	void MoveEmptyCeilDown(){
		if( EmptyCeil.y+1 < Height )
		{
			std::swap( Field[EmptyCeil.y][EmptyCeil.x] , Field[EmptyCeil.y+1][EmptyCeil.x] );
			++ EmptyCeil.y;
		}
	}
	void MoveEmptyCeilLeft(){
		if( EmptyCeil.x-1 >= 0 )
		{
			std::swap( Field[EmptyCeil.y][EmptyCeil.x] , Field[EmptyCeil.y][EmptyCeil.x-1] );
			-- EmptyCeil.x;
		}
	}
	void MoveEmptyCeilRight(){
		if( EmptyCeil.x+1 < Width )
		{
			std::swap( Field[EmptyCeil.y][EmptyCeil.x] , Field[EmptyCeil.y][EmptyCeil.x+1] );
			++ EmptyCeil.x;
		}
	}


	int const * const GetCopyOfField()
	{
		return Field;
	}

	friend ostream & operator<<(ostream & out,GameField & C)
	{
		for(int y=0;y<Height;y++)
		{
			for(int x=0;x<Width;x++)
			{
				out<<C.Field[y][x]<<" ";
			}
			out<<endl;
		}
		return out;
	}
};



#endif /* GAME_H_ */
