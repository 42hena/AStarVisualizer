#ifndef __A_STAR_H__
#define __A_STAR_H__

// #include "Board.h"

#include "Vector2.h"
#include "Weight.h"

#include <list>
#include <queue>
#include <set>

#include <Windows.h>

struct Node
{
public:
	// 부모 포인터
	Node* parentNode = nullptr;

	// grid 좌표 객체
	Vector2	pos;
	
	// 가중치 객체
	Weight weight;
};

struct CompareNode {
	bool operator()(const Node* a, const Node* b) const {
		return b->weight < a->weight; // fCost가 작은 게 더 우선
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

class AStarWithList
{
public:
	AStarWithList();
	~AStarWithList();

public:
	inline HWND GetHandleWindow() const { return _hWnd; }


	Vector2 GetStartPosition() const;
	Vector2 GetDestPosition() const;
	void SetStartPosition(const Vector2& pos);
	void SetDestPosition(const Vector2& pos);


	void Clear();

	void ClearOpenList();
	void ClearCloseList();

private:
	bool IsValidPosition(int posY, int posX);
	


public:/* 리스트 구현 */
	void NoneWithList();
	void InitAStarWithList();
	void SearchDestWithList();
	void RecordPathWithList();
	void ClearBoardWithList();
	void ClearBoardExceptWallWithList();

private:
	bool IsExistInCloseList(Vector2 pos);
	bool FindNodeByPosInOpenList(Vector2 pos, Weight nextWeight, Node** pFindNode);
public:
	void SearchWithList();

	float CalHValue(const Vector2& pos) const;

	std::priority_queue<Node*, std::vector<Node*>, CompareNode>	_openPQ;
	std::set<Vector2>											_closeSet;

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

	/* list로 구현 */
	std::list<Node*> _openList;
	std::list<Node*> _closeList;
	Node* lastNode = nullptr;

	bool _hasStartPosFlag = false;
	bool _hasDestPosFlag = false;

	Vector2 _startPosition;
	Vector2 _destinationPosition;

	HWND _hWnd = nullptr;

	State	_state;
	int		_calState = 0;
};

/*
*		우선순위 큐를 이용하여 AStar를 최적화한 알고리즘.
*/

//class AStarWithPQ
//{
//public:
//	AStarWithPQ();
//	~AStarWithPQ();
//
//public:
//	inline HWND GetHandleWindow() const { return _hWnd; }
//
//
//	Vector2 GetStartPosition() const;
//	Vector2 GetDestPosition() const;
//	void SetStartPosition(const Vector2& pos);
//	void SetDestPosition(const Vector2& pos);
//
//
//	void Clear();
//
//	void ClearOpenList();
//	void ClearCloseList();
//
//private:
//	bool IsValidPosition(int x, int y);
//	
//
//public:		/* PQ 구현 */
//	void NoneWithPQ();
//	void InitAStarWithPQ();
//	void SearchDestWithPQ();
//	void RecordPathWithPQ();
//	void ClearBoardWithPQ();
//
//public:
//	void SearchWithPriorityQueue();
//
//	float CalHValue(const Vector2& pos) const;
//
//	std::priority_queue<Node*, std::vector<Node*>, CompareNode>	_openPQ;
//	std::set<Vector2>											_closeSet;
//
//	inline bool HasStartPos() const { return _hasStartPosFlag; }
//	inline bool HasDestPos() const { return _hasDestPosFlag; }
//
//	inline void SetStartTrueFlag() { _hasStartPosFlag = true; }
//	inline void SetDestTrueFlag() { _hasDestPosFlag = true; }
//	inline void SetStartFalseFlag() { _hasStartPosFlag = false; }
//	inline void SetDestFalseFlag() { _hasDestPosFlag = false; }
//
//	inline int GetStartPosX() const { return _startPosition._x; }
//	inline int GetStartPosY() const { return _startPosition._y; }
//	inline int GetDestPosX() const { return _destinationPosition._x; }
//	inline int GetDestPosY() const { return _destinationPosition._y; }
//
//	Node* _lastCloseNode = nullptr;
//
//	bool _hasStartPosFlag = false;
//	bool _hasDestPosFlag = false;
//
//	Vector2 _startPosition;
//	Vector2 _destinationPosition;
//
//	HWND _hWnd = nullptr;
//
//	State _state;
//};

#endif
