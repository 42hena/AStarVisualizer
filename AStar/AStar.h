#ifndef __A_STAR_H__
#define __A_STAR_H__

// #include "Board.h"

#include "Vector2.h"

#include <list>
#include <queue>
#include <set>

#include <Windows.h>


struct Node
{
public:
	Node* parentNode = nullptr;

	/*bool operator<(const Node& rhs) const
	{
		return fValue < rhs.fValue;
	}*/

	Vector2 _nodePos;

	float gValue = 0.0;
	float hValue = 0.0;
	float fValue = 0.0;
};

struct CompareNode {
	bool operator()(const Node* a, const Node* b) const {
		return a->fValue > b->fValue; // fCost가 작은 게 더 우선
	}
};

enum State
{
	NoneLevel,
	InitLevel,
	SearchLevel,
	PathLevel,
	ClearLevel
};

class AStar
{
public:
	AStar();
	//AStar(int sy, int sx, int ey, int ex);
	~AStar() = default;

public:
	inline HWND GetHandleWindow() const { return _hWnd; }


	Vector2 GetStartPosition() const;
	Vector2 GetDestPosition() const;
	void SetStartPosition(const Vector2& pos);
	void SetDestPosition(const Vector2& pos);


	void Clear();

public:
	void ProcessStep();

	void ProcessAll();


private:
	bool IsValidPosition(int posY, int posX);



public:/* 리스트 구현 */
	void None();
	void InitAStar();
	void SearchDest();
	void RecordPath();
	void ClearBoard();

public:
	/* list로 구현 */
	std::list<Node*> _openList;
	std::list<Node*> _closeList;
	Node* lastNode = nullptr;
	

	void SearchWithList();
	void SearchWithPriorityQueue();

	float CalHValue(const Vector2& pos) const;

	/*std::priority_queue<Node*, std::vector<Node*>, CompareNode>	_pq;
	std::set<std::pair<int, int>>				_openList;
	std::set<std::pair<int, int>>				_closeList;*/

	inline bool HasStartPos() const { return _hasStartPosFlag; }
	inline bool HasDestPos() const { return _hasDestPosFlag; }

	inline void SetStartTrueFlag() { _hasStartPosFlag = true; }
	inline void SetDestTrueFlag()  { _hasDestPosFlag = true; }
	inline void SetStartFalseFlag() { _hasStartPosFlag = false; }
	inline void SetDestFalseFlag()  { _hasDestPosFlag = false; }

	inline int GetStartPosX() const { return _startPosition._x; }
	inline int GetStartPosY() const { return _startPosition._y; }
	inline int GetDestPosX() const { return _destinationPosition._x; }
	inline int GetDestPosY() const { return _destinationPosition._y; }

	bool _hasStartPosFlag = false;
	bool _hasDestPosFlag = false;

	Vector2 _startPosition;
	Vector2 _destinationPosition;

	HWND _hWnd = nullptr;

	State _state;
};
#endif
