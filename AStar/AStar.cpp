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

AStarWithList::AStarWithList()
	: _startPosition(-1, -1),
	_destinationPosition(-1, -1),
	_state(NoneLevel)
{
}
AStarWithList::~AStarWithList()
{
	ClearOpenList();
	ClearCloseList();
}
/*
*		공개 함수
*/


Vector2 AStarWithList::GetStartPosition() const
{
	return _startPosition;
}

Vector2 AStarWithList::GetDestPosition() const
{
	return _destinationPosition;
}

void AStarWithList::SetStartPosition(const Vector2& pos)
{
	if (_hasStartPosFlag) {
		// TODO: board 필요
	}

	_startPosition = pos;
}

void AStarWithList::SetDestPosition(const Vector2& pos)
{
	if (_hasStartPosFlag) {
		// TODO: board 필요
	}

	_destinationPosition = pos;
}

void AStarWithList::Clear()
{
	// TODO: 정리하는 함수가 될 듯
}

void AStarWithList::ClearOpenList()
{
	// openList에 들어있는 노드 삭제
	for (auto node : _openList) {
		delete node;
	}

	// 접근하지 못하도록 clear
	_openList.clear();
}

void AStarWithList::ClearCloseList()
{
	// closeList에 들어있는 노드 삭제
	for (auto node : _closeList) {
		delete node;
	}

	// 접근하지 못하도록 clear
	_closeList.clear();
}

bool AStarWithList::IsValidPosition(int x, int y)
{
	// 경계를 벗어남
	if (x < 0 || x >= Board::MAX_WIDTH || y < 0 || y >= Board::MAX_HEIGHT) {
		return false;
	}
	return true;
}

bool AStarWithList::IsExistInCloseList(Vector2 pos)
{
	bool alreadySearchFlag = false;
	for (auto closeListIter = _closeList.begin(); closeListIter != _closeList.end(); ++closeListIter)
	{
		Node* pCompareNode = *closeListIter;
		if (pos == pCompareNode->pos)
		{
			alreadySearchFlag = true;
			break;
		}
	}
	return alreadySearchFlag;
}

bool AStarWithList::FindNodeByPosInOpenList(Vector2 nextPos, Weight nextWeight, Node** pFindNode)
{
	bool findNodeFlag = false;

	for (auto openListIter = _openList.begin(); openListIter != _openList.end(); ++openListIter)
	{
		Node* cmpNode = *openListIter;
		if (cmpNode->pos == nextPos)
		{
			findNodeFlag = true;
			*pFindNode = cmpNode;
			break;
		}
	}
	return findNodeFlag;
}

void AStarWithList::NoneWithList()
{
	// case 시작 위치와 목적 위치가 정해지지 않음
	if (HasStartPos() == false || HasDestPos() == false) {
		return;
	}

	// 상태 변경
	_state = InitLevel;
}

void AStarWithList::InitAStarWithList()
{
	// 시작 노드 초기화
	Node* newNode = new Node();
	newNode->parentNode = nullptr;
	
	newNode->gValue = 0;
	newNode->hValue = CalHValue(_startPosition);
	newNode->fValue = newNode->hValue;

	newNode->weight = Weight{ 0, CalHValue(_startPosition), CalHValue(_startPosition )};
	newNode->pos = _startPosition;

	// 시작 노드를 openList에 삽입
	_openList.push_back(newNode);


 	global_board._board[newNode->pos._y][newNode->pos._x] = Open;
	global_board._weight[newNode->pos._y][newNode->pos._x].f = newNode->fValue;
	global_board._weight[newNode->pos._y][newNode->pos._x].g = newNode->gValue;
	global_board._weight[newNode->pos._y][newNode->pos._x].h = newNode->hValue;
	global_board.p[newNode->pos._y][newNode->pos._x] = newNode;
	
	// TODO: 왜 시작노드에 부모가 있어? + 부모가 필요할까?

	int boardXIndex = newNode->pos._x;
	int boardYIndex = newNode->pos._y;

	// 부모 설정
	global_board._boardInfo[boardYIndex][boardXIndex].parent = nullptr;

	// 가중치 설정
	global_board._boardInfo[boardYIndex][boardXIndex].weight = newNode->weight;

	// 보드 그리드의 상태값 변경.
	global_board._boardInfo[boardYIndex][boardXIndex].type = Open;

	// 상태 변경
	_state = SearchLevel;
}

void AStarWithList::SearchDestWithList()
{
	// Visual Studio 내부 출력창에 사용할 메세지 버퍼.
	wchar_t messageBuffer[256];

	// 탐색 가능한 노드가 없음(OpenList가 비었음)
	if (_openList.empty()) {
		_state = ClearLevel;
		return;
	}

	// 1단계: OpenList에서 가장 작은 F값을 검색
	std::list<Node*>::iterator minIter = _openList.begin();
	for (auto openListIter = _openList.begin(); openListIter != _openList.end(); ++openListIter)
	{
		Node* pCompareNode = *openListIter;
		if ((*minIter)->fValue > pCompareNode->fValue) {
			minIter = openListIter;
		}
	}

	Node* pMinNode = *minIter;
	lastNode = pMinNode;

	// OpenList에서 삭제.
	_openList.erase(minIter);

	// 2단계: 탐색된 노드가 CloseList에 존재(이미 탐색된 경우)하는 지 확인.
	//bool alreadySearchFlag = false;
	//for (auto closeListIter = _closeList.begin(); closeListIter != _closeList.end(); ++closeListIter)
	//{
	//	Node* pCompareNode = *closeListIter;
	//	if (pMinNode->pos == pCompareNode->pos)
	//	{
	//		alreadySearchFlag = true;
	//		break;
	//	}
	//}

	//// 이미 방문했음(closeList에 있음).
	//if (alreadySearchFlag == true) {
	//	return;
	//}
	
	// CloseList에 있는지 확인.
	if (IsExistInCloseList(pMinNode->pos) == true) {
		return;
	}

	// 3단계: CloseList에 삽입( Open-> Close 이므로, Board의 가중치 갱신은 안함. )
	_closeList.push_back(pMinNode);
	global_board._board[pMinNode->pos._y][pMinNode->pos._x] = Close;
	global_board.p[pMinNode->pos._y][pMinNode->pos._x] = pMinNode;

	// 탐색된 노드의 위치가 목적 지점일 경우
	if (pMinNode->pos == _destinationPosition)
	{
		lastNode = pMinNode;
		_state = PathLevel;
		return;
	}

	std::vector<DirInfo> dirInfo{
		{Vector2(1, 0), 1.0f},{Vector2(-1, 0), 1.0f},{Vector2(0, 1), 1.0f},{Vector2(0, -1), 1.0f},
		{Vector2(1, 1), 1.414f},{Vector2(1, -1), 1.414f},{Vector2(-1, 1), 1.414f},{Vector2(-1, -1), 1.414f},
	};

	// 4단계: 다음 방문 가능한 노드를 탐색
	for (int dir = 0; dir < 8; ++dir)
	{
		Vector2 nextPos = pMinNode->pos + dirInfo[dir].pos;
		if (IsValidPosition(nextPos._x, nextPos._y) == false) {
			continue;
		}

		// 다음 방문 노드의 위치가 벽일 경우
		int boardType = global_board._board[nextPos._y][nextPos._x];
		if (boardType == Wall) {
			continue;
		}

		// 다음 방문 노드가 이미 closeList에 존재하는지 확인
		if (IsExistInCloseList(nextPos) == true) {
			continue;
		}
		// closeList에 있는지
		/*bool closeFlag = false;
		for (auto closeListIter = _closeList.begin(); closeListIter != _closeList.end(); ++closeListIter)
		{
			Node* cmpNode = *closeListIter;
			if (cmpNode->pos == nextPos) {
				closeFlag = true;
				break;
			}
		}
		if (closeFlag) {
			continue;
		}*/

		// 다음 가중치 계산
		float nextGValue = pMinNode->gValue + dirInfo[dir].value;
		float nextHValue = CalHValue(nextPos);
		float nextFValue = nextGValue + nextHValue;
		Weight nextWeight{ nextGValue , nextHValue, nextFValue };

		// case 이미 OpenList에 들어있는 경우
		Node* pNodeInOpenList = nullptr;
		if (FindNodeByPosInOpenList(nextPos, nextWeight, &pNodeInOpenList) == true)
		{
			// 나오면 안되는 경우
			if (pNodeInOpenList == nullptr) {
				DebugBreak();
				return;
			}

			// 작은 경우 가중치와 부모 노드 변경.
			if (nextWeight < pNodeInOpenList->weight)
			{
				pNodeInOpenList->weight = nextWeight;
				pNodeInOpenList->parentNode = pMinNode;
			}
		}
		else // OpenList에 없는 경우
		{
			Node* pOpenNode = new Node();
			pOpenNode->parentNode = pMinNode;
			pOpenNode->pos = nextPos;
			pOpenNode->gValue = nextGValue;
			pOpenNode->hValue = nextHValue;
			pOpenNode->fValue = nextFValue;

			pOpenNode->weight = nextWeight;


			int boardYIndex = pOpenNode->pos._y;
			int boardXIndex = pOpenNode->pos._x;
			global_board._weight[boardYIndex][boardXIndex].f = pOpenNode->fValue;
			global_board._weight[boardYIndex][boardXIndex].g = pOpenNode->gValue;
			global_board._weight[boardYIndex][boardXIndex].h = pOpenNode->hValue;

			global_board._board[boardYIndex][boardXIndex] = Open;

			/*if (pOpenNode->pos == _destinationPosition)
				global_board._board[pOpenNode->pos._y][pOpenNode->pos._x] = End;
			else
				global_board._board[pOpenNode->pos._y][pOpenNode->pos._x] = Open;
			global_board._weight[pOpenNode->pos._y][pOpenNode->pos._x].f = pOpenNode->fValue;
			global_board._weight[pOpenNode->pos._y][pOpenNode->pos._x].g = pOpenNode->gValue;
			global_board._weight[pOpenNode->pos._y][pOpenNode->pos._x].h = pOpenNode->hValue;*/

			swprintf_s(messageBuffer, 100, L"Add Open(%d %d) f:%.3f g:%.3f h:%.3f\n", nextPos._y, nextPos._x, pOpenNode->fValue, pOpenNode->gValue, pOpenNode->hValue);
			OutputDebugString(messageBuffer);

			_openList.push_back(pOpenNode);
		}

		/*bool findNodeFlag = false;
		for (auto openListIter = _openList.begin(); openListIter != _openList.end(); ++openListIter)
		{
			Node* cmpNode = *openListIter;
			if (cmpNode->pos == nextPos)
			{
				findNodeFlag = true;
				if (cmpNode->fValue > nextFValue)
				{
					cmpNode->fValue = nextFValue;
					cmpNode->gValue = nextGValue;
					cmpNode->hValue = nextHValue;

					cmpNode->weight = nextWeight;

					cmpNode->parentNode = pMinNode;

					global_board._weight[cmpNode->pos._y][cmpNode->pos._x].f = cmpNode->fValue;
					global_board._weight[cmpNode->pos._y][cmpNode->pos._x].g = cmpNode->gValue;
					global_board._weight[cmpNode->pos._y][cmpNode->pos._x].h = cmpNode->hValue;

					break;
				}
			}
		}
		if (findNodeFlag == false)
		{
			Node* pOpenNode = new Node();
			pOpenNode->parentNode = pMinNode;
			pOpenNode->pos = nextPos;
			pOpenNode->gValue = nextGValue;
			pOpenNode->hValue = nextHValue;
			pOpenNode->fValue = nextFValue;
			if (pOpenNode->pos == _destinationPosition)
				global_board._board[pOpenNode->pos._y][pOpenNode->pos._x] = End;
			else
				global_board._board[pOpenNode->pos._y][pOpenNode->pos._x] = Open;
			global_board._weight[pOpenNode->pos._y][pOpenNode->pos._x].f = pOpenNode->fValue;
			global_board._weight[pOpenNode->pos._y][pOpenNode->pos._x].g = pOpenNode->gValue;
			global_board._weight[pOpenNode->pos._y][pOpenNode->pos._x].h = pOpenNode->hValue;

			swprintf_s(messageBuffer, 100, L"Add Open(%d %d) f:%.3f g:%.3f h:%.3f\n", nextPos._y, nextPos._x, pOpenNode->fValue, pOpenNode->gValue, pOpenNode->hValue);
			OutputDebugString(messageBuffer);

			_openList.push_back(pOpenNode);
		}*/
	}

}

void AStarWithList::RecordPathWithList()
{
	Node* nextNode;
	while (lastNode != nullptr)
	{
		lastNode;
		nextNode = lastNode->parentNode;

		global_board._board[lastNode->pos._y][lastNode->pos._x] = Path;
		if (nextNode != nullptr)
		{
			// Todo: 그림 그려야 할 듯.
		}

		lastNode = nextNode;
	}

	_state = ClearLevel;
}

void AStarWithList::ClearBoardWithList()
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



void AStarWithList::SearchWithList()
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



float AStarWithList::CalHValue(const Vector2& pos) const
{
	int dx = pos._x - GetDestPosX();
	int dy = pos._y - GetDestPosY();

	return static_cast<float>(sqrt( dx * dx + dy * dy ) );
	return static_cast<float>(abs(dx) + abs(dy));
}


// ===============================================================================
// ===============================================================================
// ===============================================================================

/*
*		PQ를 이용한 AStar 최적화
*/

float AStarWithPQ::CalHValue(const Vector2& pos) const
{
	int dx = pos._x - GetDestPosX();
	int dy = pos._y - GetDestPosY();

	return static_cast<float>(sqrt(dx * dx + dy * dy));
	return static_cast<float>(abs(dx) + abs(dy));
}
AStarWithPQ::AStarWithPQ()
{
	_closeSet.clear();
}

AStarWithPQ::~AStarWithPQ()
{
}

/*
*		공개 함수
*/

void AStarWithPQ::NoneWithPQ()
{
	// case 시작 위치와 목적 위치가 정해지지 않음
	if (HasStartPos() == false || HasDestPos() == false) {
		return;
	}

	// 상태 변경
	_state = InitLevel;
}

void AStarWithPQ::InitAStarWithPQ()
{
	// 시작 노드 초기화
	Node* newNode = new Node();
	newNode->parentNode = nullptr;
	newNode->gValue = 0;
	newNode->hValue = CalHValue(_startPosition);
	newNode->fValue = newNode->hValue;
	newNode->pos = _startPosition;

	// 시작 노드를 openPQ에 삽입
	_openPQ.push(newNode);

	global_board._board[newNode->pos._y][newNode->pos._x] = Open;
	global_board._weight[newNode->pos._y][newNode->pos._x].f = newNode->fValue;
	global_board._weight[newNode->pos._y][newNode->pos._x].g = newNode->gValue;
	global_board._weight[newNode->pos._y][newNode->pos._x].h = newNode->hValue;

	// 상태 변경
	_state = SearchLevel;
}

void AStarWithPQ::SearchDestWithPQ()
{
	if (_openPQ.empty()) {
		_state = ClearLevel;
		return;
	}

	// 우선순위 큐에서 가장 가중치가 작은 노드를 뽑음.
	Node* openNode = _openPQ.top(); _openPQ.pop();

	Vector2 minNodePos = openNode->pos;
	
	// Case 1: 현재 노드가 목적지인지 확인.
	if (openNode->pos == _destinationPosition)
	{
		_lastCloseNode = openNode;
		_state = PathLevel;
		return;
	}

	// Case 2: closeList에 현재 노드가 존재하는 지 확인
	auto closeIt = _closeSet.find(openNode->pos);
	if (closeIt != _closeSet.end()) {
		return;
	}// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!여기부터

	// closeList에 마크하기
	_closeSet.insert(openNode->pos);
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
		pOpenNode->pos = nextPos;
		pOpenNode->gValue = nextGValue;
		pOpenNode->hValue = nextHValue;
		pOpenNode->fValue = nextFValue;

		if (pOpenNode->pos != _destinationPosition)
			global_board._board[pOpenNode->pos._y][pOpenNode->pos._x] = Open;
		global_board._weight[pOpenNode->pos._y][pOpenNode->pos._x].f = pOpenNode->fValue;
		global_board._weight[pOpenNode->pos._y][pOpenNode->pos._x].g = pOpenNode->gValue;
		global_board._weight[pOpenNode->pos._y][pOpenNode->pos._x].h = pOpenNode->hValue;

		_openPQ.push(pOpenNode);
	}
}

void AStarWithPQ::RecordPathWithPQ()
{
	Node* pDestNode = _lastCloseNode;
	while (pDestNode)
	{
		global_board._board[pDestNode->pos._y][pDestNode->pos._x] = Path;
		pDestNode = pDestNode->parentNode;
	}

	_state = ClearLevel;
}

void AStarWithPQ::ClearBoardWithPQ()
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

void AStarWithPQ::SearchWithPriorityQueue()
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
