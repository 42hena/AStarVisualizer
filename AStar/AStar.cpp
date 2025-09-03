#include "AStar.h"
#include "Vector2.h"
#include "../AStar/Board.h"
#include "../Core/GlobalCore.h"
#include <math.h>
#include <Windows.h>

/*
*		AStar의 특수 맴버 함수
*/

struct DirInfo
{
	Vector2 pos;
	float value;
};

AStar::AStar()
	: _startPosition(-1, -1),
	_destinationPosition(-1, -1),
	_state(NoneLevel)
{
}
AStar::~AStar()
{
	ClearOpenList();
	ClearCloseList();
}
/*
*		공개 함수
*/

Vector2 AStar::GetStartPosition() const
{
	return _startPosition;
}

Vector2 AStar::GetDestPosition() const
{
	return _destinationPosition;
}

void AStar::SetStartPosition(const Vector2& pos)
{
	if (_hasStartPosFlag) {
		// TODO: board 필요
	}

	_startPosition = pos;
}

void AStar::SetDestPosition(const Vector2& pos)
{
	if (_hasStartPosFlag) {
		// TODO: board 필요
	}

	_destinationPosition = pos;
}

void AStar::Clear()
{
	// TODO: 정리하는 함수가 될 듯
}

void AStar::ClearOpenList()
{
	// openList에 들어있는 노드 삭제
	for (auto node : _openList) {
		delete node;
	}

	// 접근하지 못하도록 clear
	_openList.clear();
}

void AStar::ClearCloseList()
{
	// closeList에 들어있는 노드 삭제
	for (auto node : _closeList) {
		delete node;
	}

	// 접근하지 못하도록 clear
	_closeList.clear();
}

void AStar::NoneWithList()
{
	// case 시작 위치와 목적 위치가 정해지지 않음
	if (HasStartPos() == false || HasDestPos() == false) {
		return;
	}

	// 상태 변경
	_state = InitLevel;
}

void AStar::InitAStarWithList()
{
	// 시작 노드 초기화
	Node* newNode = new Node();
	newNode->parentNode = nullptr;
	newNode->gValue = 0;
	newNode->hValue = CalHValue(_startPosition);
	newNode->fValue = newNode->hValue;
	newNode->_nodePos = _startPosition;

	// 시작 노드를 openList에 삽입
	_openList.push_back(newNode);

 	global_board._board[newNode->_nodePos._y][newNode->_nodePos._x] = Open;
	global_board._weight[newNode->_nodePos._y][newNode->_nodePos._x].f = newNode->fValue;
	global_board._weight[newNode->_nodePos._y][newNode->_nodePos._x].g = newNode->gValue;
	global_board._weight[newNode->_nodePos._y][newNode->_nodePos._x].h = newNode->hValue;
	global_board.p[newNode->_nodePos._y][newNode->_nodePos._x] = newNode;
	// 상태 변경
	_state = SearchLevel;
}

void AStar::SearchDestWithList()
{
	wchar_t messageBuffer[100];

	// case 더 이상 탐색할 노드가 없음
	if (_openList.empty()) {
		_state = ClearLevel;
		return;
	}

	// 1. OpenList에서 가장 작은 F값을 검색
	std::list<Node*>::iterator minIter = _openList.begin();
	for (auto openListIter = _openList.begin(); openListIter != _openList.end(); ++openListIter)
	{
		Node* pCompareNode = *openListIter;
		if ((*minIter)->fValue > pCompareNode->fValue) {
			minIter = openListIter;
		}
	}

	Node* pMinNode = *minIter;
	
	// dest인지
	if (pMinNode->_nodePos == _destinationPosition)
	{
		lastNode = pMinNode;
		_state = PathLevel;
		return;
	}

	// 2. OpenList에서 삭제.
	_openList.erase(minIter);

	swprintf_s(messageBuffer, 100, L"Pop (%d %d) f:%.3f g:%.3f h:%.3f\n", pMinNode->_nodePos._y, pMinNode->_nodePos._x, pMinNode->fValue, pMinNode->gValue, pMinNode->hValue);
	OutputDebugString(messageBuffer);

	// 3. CloseList에 존재하는 지 확인.
	bool alreadySearchFlag = false;
	for (auto closeListIter = _closeList.begin(); closeListIter != _closeList.end(); ++closeListIter)
	{
		Node* pCompareNode = *closeListIter;
		if (pMinNode->_nodePos == pCompareNode->_nodePos)
		{
			alreadySearchFlag = true;
			break;
		}
	}

	// 이미 방문했음(closeList에 있음).
	if (alreadySearchFlag == true) {
		return;
	}

	// 4. CloseList에 삽입
	_closeList.push_back(pMinNode);
	global_board._board[pMinNode->_nodePos._y][pMinNode->_nodePos._x] = Close;
	global_board.p[pMinNode->_nodePos._y][pMinNode->_nodePos._x] = pMinNode;
	/*global_board._weight[pMinNode->_nodePos._y][pMinNode->_nodePos._x].f = pMinNode->fValue;
	global_board._weight[pMinNode->_nodePos._y][pMinNode->_nodePos._x].g = pMinNode->gValue;
	global_board._weight[pMinNode->_nodePos._y][pMinNode->_nodePos._x].h = pMinNode->hValue;*/

	// 5. 방문 가능한 방향 전부 확인.
	std::vector<DirInfo> dirInfo{
		{Vector2(1, 0), 1.0f},{Vector2(-1, 0), 1.0f},{Vector2(0, 1), 1.0f},{Vector2(0, -1), 1.0f},
		{Vector2(1, 1), 1.414f},{Vector2(1, -1), 1.414f},{Vector2(-1, 1), 1.414f},{Vector2(-1, -1), 1.414f},
	};
	
	for (int dir = 0; dir < 8; ++dir)
	{
		Vector2 nextPos = pMinNode->_nodePos + dirInfo[dir].pos;
		/*swprintf_s(messageBuffer, 100, L"(%d %d)\n", nextPos._x, nextPos._y);
		OutputDebugString(messageBuffer);*/
		// 원하는 칸인지 확인
		if (nextPos._x < 0 || nextPos._x >= 20 || nextPos._y < 0 || nextPos._y >= 20) {
			continue;
		}

		// 벽인지 확인
		if (global_board._board[nextPos._y][nextPos._x] == Wall) {
			continue;
		}
		
		// closeList에 있는지
		bool closeFlag = false;
		for (auto closeListIter = _closeList.begin(); closeListIter != _closeList.end(); ++closeListIter)
		{
			Node* cmpNode = *closeListIter;
			if (cmpNode->_nodePos == nextPos) {
				closeFlag = true;
				break;
			}
		}
		if (closeFlag) {
			continue;
		}

		// 다음 가중치 계산
		float nextGValue = pMinNode->gValue + dirInfo[dir].value;
		float nextHValue = CalHValue(nextPos);
		float nextFValue = nextGValue + nextHValue;

		swprintf_s(messageBuffer, 100, L"Check (%d %d) f:%.3f g:%.3f h:%.3f\n", nextPos._y, nextPos._x, nextFValue, nextGValue, nextHValue);
		OutputDebugString(messageBuffer);

		// case 이미 OpenList에 들어있으면서 값이 작은 경우
		bool findNodeFlag = false;
		for (auto openListIter = _openList.begin(); openListIter != _openList.end(); ++openListIter)
		{
			Node* cmpNode = *openListIter;
			if (cmpNode->_nodePos == nextPos)
			{
 				int a = 0;
				findNodeFlag = true;
				if (cmpNode->fValue > nextFValue)
				{
 					cmpNode->fValue = nextFValue;
					cmpNode->gValue = nextGValue;
					cmpNode->hValue = nextHValue;
					cmpNode->parentNode = pMinNode;
					break;
				}
			}
		}
		if (findNodeFlag == false)
		{
			Node* pOpenNode = new Node();
			pOpenNode->parentNode = pMinNode;
			pOpenNode->_nodePos = nextPos;
			pOpenNode->gValue = nextGValue;
			pOpenNode->hValue = nextHValue;
			pOpenNode->fValue = nextFValue;
			if (pOpenNode->_nodePos == _destinationPosition)
				global_board._board[pOpenNode->_nodePos._y][pOpenNode->_nodePos._x] = End;
			else
				global_board._board[pOpenNode->_nodePos._y][pOpenNode->_nodePos._x] = Open;
			global_board._weight[pOpenNode->_nodePos._y][pOpenNode->_nodePos._x].f = pOpenNode->fValue;
			global_board._weight[pOpenNode->_nodePos._y][pOpenNode->_nodePos._x].g = pOpenNode->gValue;
			global_board._weight[pOpenNode->_nodePos._y][pOpenNode->_nodePos._x].h = pOpenNode->hValue;
			
			swprintf_s(messageBuffer, 100, L"Add Open(%d %d) f:%.3f g:%.3f h:%.3f\n", nextPos._y, nextPos._x, pOpenNode->fValue, pOpenNode->gValue, pOpenNode->hValue);
			OutputDebugString(messageBuffer);

			_openList.push_back(pOpenNode);
		}
	}
}

void AStar::RecordPathWithList()
{
	Node* nextNode;
	while (lastNode != nullptr)
	{
		lastNode;
		nextNode = lastNode->parentNode;

		global_board._board[lastNode->_nodePos._y][lastNode->_nodePos._x] = Path;
		if (nextNode != nullptr)
		{
			// Todo: 그림 그려야 할 듯.
		}

		lastNode = nextNode;
	}

	_state = ClearLevel;
}

void AStar::ClearBoardWithList()
{
	// OpenList 및 CloseList 삭제.
	ClearOpenList();
	ClearCloseList();

	// Board 판 정리
	for (int i = 0; i < 50; ++i)
	{
		for (int j = 0; j < 50; ++j)
		{
			global_board._board[i][j] = 0;
			global_board._weight[i][j].f = 0;
			global_board._weight[i][j].g = 0;
			global_board._weight[i][j].h = 0;
			global_board.p[i][j] = nullptr;
		}
	}

	SetStartFalseFlag();
	SetDestFalseFlag();

	_state = NoneLevel;
}

void AStar::NoneWithPQ()
{
	// case 시작 위치와 목적 위치가 정해지지 않음
	if (HasStartPos() == false || HasDestPos() == false) {
		return;
	}

	// 상태 변경
	_state = InitLevel;
}

void AStar::InitAStarWithPQ()
{
	// 시작 노드 초기화
	Node* newNode = new Node();
	newNode->parentNode = nullptr;
	newNode->gValue = 0;
	newNode->hValue = CalHValue(_startPosition);
	newNode->fValue = newNode->hValue;
	newNode->_nodePos = _startPosition;

	// 시작 노드를 openPQ에 삽입
	_openPQ.push(newNode);

	global_board._board[newNode->_nodePos._y][newNode->_nodePos._x] = Open;
	global_board._weight[newNode->_nodePos._y][newNode->_nodePos._x].f = newNode->fValue;
	global_board._weight[newNode->_nodePos._y][newNode->_nodePos._x].g = newNode->gValue;
	global_board._weight[newNode->_nodePos._y][newNode->_nodePos._x].h = newNode->hValue;

	// 상태 변경
	_state = SearchLevel;
}

void AStar::SearchDestWithPQ()
{
	if (_openPQ.empty()) {
		_state = ClearLevel;
		return;
	}

	Node* openNode = _openPQ.top(); _openPQ.pop();

	Vector2 minNodePos = openNode->_nodePos;
	// 0. 목적지인지
	if (openNode->_nodePos == _destinationPosition)
	{
		lastNode = openNode;
		_state = PathLevel;
		return;
	}


	// 1. closeList에 있는지 확인
	auto closeIt = _closeSet.find(openNode->_nodePos);
	if (closeIt != _closeSet.end()) {
		return;
	}

	// 2 closeList에 삽입
	_closeSet.insert(openNode->_nodePos);
	global_board._board[minNodePos._y][minNodePos._x] = Close;

	std::vector<DirInfo> dirInfo{
		{Vector2(1, 0), 1.0f},{Vector2(-1, 0), 1.0f},{Vector2(0, 1), 1.0f},{Vector2(0, -1), 1.0f},
		{Vector2(1, 1), 1.414f},{Vector2(1, -1), 1.414f},{Vector2(-1, 1), 1.414f},{Vector2(-1, -1), 1.414f},
	};

	// 3. 8방향 확인
	for (int dir = 0; dir < 8; ++dir)
	{
		Vector2 nextPos = minNodePos + dirInfo[dir].pos;
		/*swprintf_s(messageBuffer, 100, L"(%d %d)\n", nextPos._x, nextPos._y);
		OutputDebugString(messageBuffer);*/
		// 원하는 칸인지 확인
		if (nextPos._x < 0 || nextPos._x >= 20 || nextPos._y < 0 || nextPos._y >= 20) {
			continue;
		}

		// 벽인지 확인
		if (global_board._board[nextPos._y][nextPos._x] == Wall) {
 			continue;
		}

		// 다음 가중치 계산
		float nextGValue = openNode->gValue + dirInfo[dir].value;
		float nextHValue = CalHValue(nextPos);
		float nextFValue = nextGValue + nextHValue;

		// openList는 비교 필요 없음.
		Node* pOpenNode = new Node();
		pOpenNode->parentNode = openNode;
		pOpenNode->_nodePos = nextPos;
		pOpenNode->gValue = nextGValue;
		pOpenNode->hValue = nextHValue;
		pOpenNode->fValue = nextFValue;

		if (pOpenNode->_nodePos != _destinationPosition)
			global_board._board[pOpenNode->_nodePos._y][pOpenNode->_nodePos._x] = Open;
		global_board._weight[pOpenNode->_nodePos._y][pOpenNode->_nodePos._x].f = pOpenNode->fValue;
		global_board._weight[pOpenNode->_nodePos._y][pOpenNode->_nodePos._x].g = pOpenNode->gValue;
		global_board._weight[pOpenNode->_nodePos._y][pOpenNode->_nodePos._x].h = pOpenNode->hValue;


		/*swprintf_s(messageBuffer, 100, L"Add Open(%d %d) f:%.3f g:%.3f h:%.3f\n", nextPos._y, nextPos._x, pOpenNode->fValue, pOpenNode->gValue, pOpenNode->hValue);
		OutputDebugString(messageBuffer);*/
		_openPQ.push(pOpenNode);
	}
}

void AStar::RecordPathWithPQ()
{
	while (lastNode)
	{
		global_board._board[lastNode->_nodePos._y][lastNode->_nodePos._x] = Path;
		lastNode = lastNode->parentNode;
	}

	_state = ClearLevel;
}

void AStar::ClearBoardWithPQ()
{
	while (!_openPQ.empty())
	{
		Node* tmp = _openPQ.top(); _openPQ.pop();
		delete tmp;
	}

	for (int i = 0; i < 50; ++i)
	{
		for (int j = 0; j < 50; ++j)
		{
			global_board._board[i][j] = 0;
			global_board._weight[i][j].f = 0;
			global_board._weight[i][j].g = 0;
			global_board._weight[i][j].h = 0;
		}
	}
	SetStartFalseFlag();
	SetDestFalseFlag();

	_state = NoneLevel;
}

void AStar::SearchWithList()
{
	switch (_state)
	{
	case NoneLevel:
		NoneWithList();
		break;
	case InitLevel:
		InitAStarWithList();
		break;
	case SearchLevel:
		SearchDestWithList();
		break;
	case PathLevel:
		RecordPathWithList();
		break;
	case ClearLevel:
		ClearBoardWithList();
		break;
	default:
		break;
	}
}

void AStar::SearchWithPriorityQueue()
{
	switch (_state)
	{
	case NoneLevel:
		NoneWithPQ();
		break;
	case InitLevel:
		InitAStarWithPQ();
		break;
	case SearchLevel:
		SearchDestWithPQ();
		break;
	case PathLevel:
		RecordPathWithPQ();
		break;
	case ClearLevel:
		ClearBoardWithPQ();
		break;
	default:
		break;
	}
}

float AStar::CalHValue(const Vector2& pos) const
{
	int dx = pos._x - GetDestPosX();
	int dy = pos._y - GetDestPosY();

	return static_cast<float>(sqrt( dx * dx + dy * dy ) );
	return static_cast<float>(abs(dx) + abs(dy));
}
