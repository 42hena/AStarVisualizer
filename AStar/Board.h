#pragma once

#include "Weight.h"
#include <memory>

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

class Node;

// TODO
struct BoardInfo
{
	int		type;
	Weight	weight;
	// Node*	parent;
};

class Board
{
public:
	enum BOARD
	{
		MAX_WIDTH = 100,
		MAX_HEIGHT = 100,
	};
public:
	Board()
	{
		for (int i = 0; i < 50; ++i)
		{
			for (int j = 0; j < 50; ++j)
			{
				memset(&_boardInfo[i][j], 0, sizeof(BoardInfo));
			}
		}
	}

	/*int		_board[100][100];
	Weight	_weight[100][100];
	Node* p[100][100] = { nullptr };*/

	BoardInfo _boardInfo[100][100];
};
