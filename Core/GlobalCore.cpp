#include "../AStar/AStar.h"
#include "../AStar/Board.h"

bool global_mouseMiddleButton = false;

AStarWithList global_astar;
Board global_board;

int global_bufferLength = 48;	// TODO: 이름 변경
bool global_processFlag = false;

int global_state = 0;

// 0: None
// 1: 가중치 Value 값.
// 2: Path
