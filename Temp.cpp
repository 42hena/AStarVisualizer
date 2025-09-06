
//void AStar::ProcessStep()
//{
//	if (_pq.empty()) {
//		Node* newNode = new Node();
//		newNode->parentNode = nullptr;
//		newNode->gValue = 0;
//		newNode->hValue = std::abs(endY - startY) + std::abs(endX - startX);
//		newNode->fValue = newNode->gValue + newNode->hValue;
//		newNode->yPos = startY;
//		newNode->xPos = startX;
//
//		global_board._board[newNode->yPos][newNode->xPos] = Open;// open
//
//		global_board._weight[startY][startX].g = newNode->gValue;
//		global_board._weight[startY][startX].h = newNode->hValue;
//		global_board._weight[startY][startX].f = newNode->fValue;
//
//		_pq.push(newNode);
//		return;
//	}
//
//	// PQ가 남아있어.
//	Node* endNode = nullptr;
//	Node* node = _pq.top();
//	_pq.pop();
//
//	_closeList.insert(std::pair<int, ignt>(node->yPos, node->xPos));
//	global_board._board[node->yPos][node->xPos] = Close;
//	{
//		wchar_t buffer[100];
//		swprintf(buffer, 100, L"(%d, %d)가 pq에서 빠졌습니다. f:%.2lf g:%.2lf h:%.2lf\n", node->yPos, node->xPos, node->fValue, node->gValue, node->hValue);
//		OutputDebugString(buffer);
//	}
//
//	if (node->yPos == endY && node->xPos == endX) {
//		wchar_t buffer[100];
//		swprintf(buffer, 100, L"도착했습니다\n");
//		OutputDebugString(buffer);
//		endNode = node;
//	}
//
//	for (int i = 0; i < 8; ++i)
//	{
//		int ny = node->yPos + dy[i];
//		int nx = node->xPos + dx[i];
//
//		if (IsValidPosition(ny, nx) == false) {
//			continue;
//		}
//
//		if (global_board._board[ny][nx] == 2)
//			continue;
//		auto it = _closeList.find(std::pair<int, int>(ny, nx));
//		auto openIt = _openList.find(std::pair<int, int>(ny, nx));
//		if (it == _closeList.end() && openIt == _openList.end()) {
//
//			global_board._board[ny][nx] = 5;
//			Node* newNode = new Node();
//			newNode->parentNode = node;
//			//newNode->gValue = node->gValue + 1;// 문제가 조금 발생할 수 도 있을거 같음.
//
//			if (i < 4) // 상하좌우
//				newNode->gValue = node->gValue + 1;
//			else       // 대각선
//				newNode->gValue = node->gValue + sqrt(2);
//
//
//			/*if (i < 4)
//				newNode->hValue = std::abs(endY - ny) + std::abs(endX - nx);
//			else
//				newNode->hValue = sqrt(std::abs(endY - ny) * std::abs(endY - ny) + std::abs(endX - nx)* std::abs(endX - nx));*/
//
//
//			newNode->hValue = std::abs(endY - ny) + std::abs(endX - nx);
//
//
//			newNode->fValue = newNode->gValue + newNode->hValue;
//
//			global_board._weight[ny][nx].g = newNode->gValue;
//			global_board._weight[ny][nx].h = newNode->hValue;
//			global_board._weight[ny][nx].f = newNode->fValue;
//
//			newNode->yPos = ny;
//			newNode->xPos = nx;
//			_openList.insert({ ny, nx });
//			_pq.push(newNode);
//
//			{
//				wchar_t buffer[100];
//				swprintf(buffer, 100, L"(%d, %d)를 openList에 넣습니다.f:%.2lf g:%.2lf h:%.2lf\n", newNode->yPos, newNode->xPos, newNode->fValue, newNode->gValue, newNode->hValue);
//				OutputDebugString(buffer);
//			}
//		}
//		else {
//			{
//				wchar_t buffer[100];
//				swprintf(buffer, 100, L"(%d, %d)는 존재합니다.\n", ny, nx);
//				OutputDebugString(buffer);
//			}
//		}
//	}
//	{
//		wchar_t buffer[100];
//		swprintf(buffer, 100, L"\n");
//		OutputDebugString(buffer);
//	}
//
//
//	if (endNode == nullptr)
//		return;
//
//
//
//	int lx, ly;
//
//	while (endNode != nullptr)
//	{
//		lx = endNode->xPos;
//		ly = endNode->yPos;
//		{
//			wchar_t buffer[100];
//			swprintf(buffer, 100, L"(%d, %d)는 존재합니다.\n", endNode->yPos, endNode->xPos);
//			OutputDebugString(buffer);
//			global_board._board[endNode->yPos][endNode->xPos] = 6;// open
//		}
//		endNode = endNode->parentNode;
//	}
//
//	global_board._board[endY][endX] = 3;
//}
//
//void AStar::ProcessAll()
//{
//	DebugBreak();
//	Node* endNode = nullptr;
//	while (true)
//	{
//		if (_pq.empty())
//			break;
//		Node* node = _pq.top();
//		_pq.pop();
//		_closeList.insert(std::pair<int, int>(node->yPos, node->xPos));
//		global_board._board[node->yPos][node->xPos] = 4;
//		{
//			wchar_t buffer[100];
//			swprintf(buffer, 100, L"(%d, %d)가 pq에서 빠졌습니다. f:%.2lf g:%.2lf h:%.2lf\n", node->yPos, node->xPos, node->fValue, node->gValue, node->hValue);
//			OutputDebugString(buffer);
//		}
//		if (node->yPos == endY && node->xPos == endX) {
//			wchar_t buffer[100];
//			swprintf(buffer, 100, L"도착했습니다\n");
//			OutputDebugString(buffer);
//			endNode = node;
//			break;
//		}
//		for (int i = 0; i < 8; ++i)
//		{
//			int ny = node->yPos + dy[i];
//			int nx = node->xPos + dx[i];
//			if (IsValidPosition(ny, nx) == false) {
//				continue;
//			}
//
//			if (global_board._board[ny][nx] == 2)
//				continue;
//			auto it = _closeList.find(std::pair<int, int>(ny, nx));
//			auto openIt = _openList.find(std::pair<int, int>(ny, nx));
//			if (it == _closeList.end() && openIt == _openList.end()) {
//
//				global_board._board[ny][nx] = 5;
//				Node* newNode = new Node();
//				newNode->parentNode = node;
//				newNode->gValue = node->gValue + 1;// 문제가 조금 발생할 수 도 있을거 같음.
//				newNode->hValue = std::abs(endY - ny) + std::abs(endX - nx);
//				newNode->fValue = newNode->gValue + newNode->hValue;
//
//				global_board._weight[ny][nx].g = newNode->gValue;
//				global_board._weight[ny][nx].h = std::abs(endY - ny) + std::abs(endX - nx);
//				global_board._weight[ny][nx].f = newNode->fValue;
//
//				newNode->yPos = ny;
//				newNode->xPos = nx;
//				_openList.insert({ ny, nx });
//				_pq.push(newNode);
//
//				{
//					wchar_t buffer[100];
//					swprintf(buffer, 100, L"(%d, %d)를 openList에 넣습니다.f:%.2lf g:%.2lf h:%.2lf\n", newNode->yPos, newNode->xPos, newNode->fValue, newNode->gValue, newNode->hValue);
//					OutputDebugString(buffer);
//				}
//			}
//			else {
//				{
//					wchar_t buffer[100];
//					swprintf(buffer, 100, L"(%d, %d)는 존재합니다.\n", ny, nx);
//					OutputDebugString(buffer);
//				}
//			}
//		}
//		{
//			wchar_t buffer[100];
//			swprintf(buffer, 100, L"\n");
//			OutputDebugString(buffer);
//		}
//	}
//
//
//
//
//	while (endNode != nullptr)
//	{
//
//		{
//			wchar_t buffer[100];
//			swprintf(buffer, 100, L"(%d, %d)는 존재합니다.\n", endNode->yPos, endNode->xPos);
//			OutputDebugString(buffer);
//			global_board._board[endNode->yPos][endNode->xPos] = 6;// open
//		}
//		endNode = endNode->parentNode;
//	}
//
//	global_board._board[endY][endX] = 3;
//}
//
//bool AStar::IsValidPosition(int posY, int posX)
//{
//	if (posY < 0 || posY >= 100 || posX < 0 || posX >= 100)
//		return false;
//	return true;
//}

/*
*
*/
