#pragma once

struct Weight
{
	float f = 0.0;
	float g = 0.0;
	float h = 0.0;
};

enum ObjectState
{
	None, 
	Start,
	Wall,
	End,
	Open,
	Close,
	Path
};

class Board
{
public:
	Board()
	{
		for (int i = 0; i < 50; ++i)
		{
			for (int j = 0; j < 50; ++j)
			{
				_board[i][j] = 0;
			}
		}
	}

	int		_board[100][100];
	Weight	_weight[100][100];
	int boardWidth = 10;
	int boardHeight = 10;
};
