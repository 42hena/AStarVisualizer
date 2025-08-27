#include "Vector2.h"

#include <iostream>

#pragma region STATIC_맴버_초기화
Vector2 Vector2::Zero = Vector2(0, 0);
Vector2 Vector2::One = Vector2(1, 1);
Vector2 Vector2::Up = Vector2(0, 1);
Vector2 Vector2::Left = Vector2(-1, 0);
Vector2 Vector2::Right = Vector2(1, 0);
Vector2 Vector2::Down = Vector2(0, -1);
#pragma endregion


/*
*		Vector2의 특수 맴버 함수
*/

Vector2::Vector2(int x, int y)
	:_x(x), _y(y)
{

}

/*
*		Operator 연산자 오버로딩
*/

/* 사칙 연산 */

Vector2 Vector2::operator+(const Vector2& rhs) const {
	return Vector2(_x + rhs._x, _y + rhs._y);
}

Vector2 Vector2::operator-(const Vector2& rhs) const {
	return Vector2(_x - rhs._x, _y - rhs._y);
}

Vector2 Vector2::operator*(const Vector2& rhs) const {
	return Vector2(_x * rhs._x, _y * rhs._y);
}

Vector2 Vector2::operator/(const Vector2& rhs) const {
	return Vector2(_x / rhs._x, _y / rhs._y);
}

/* 비교 연산 */

bool Vector2::operator==(const Vector2& rhs) const
{
	if ((_x - rhs._x) | _y - rhs._y) {
		return false;
	}
	return true;
}

bool Vector2::operator!=(const Vector2& rhs) const
{
	return !operator==(rhs);
}

/* 대입 연산 */

Vector2& Vector2::operator+=(const Vector2& rhs)
{
	_x += rhs._x;
	_y += rhs._y;
	return *this;
}
Vector2& Vector2::operator-=(const Vector2& rhs)
{
	_x -= rhs._x;
	_y -= rhs._y;
	return *this;
}
Vector2& Vector2::operator*=(const Vector2& rhs)
{
	_x *= rhs._x;
	_y *= rhs._y;
	return *this;
}
Vector2& Vector2::operator/=(const Vector2& rhs)
{
	_x /= rhs._x;
	_y /= rhs._y;
	return *this;
}

/*
*		공개 함수
*/

const char* Vector2::ToString()
{
	static char buffer[256];
	sprintf_s(buffer, 256, "(%d, %d)", _x, _y);

	return buffer;
}

/* 사칙 연산 함수 */

Vector2 Vector2::Add(const Vector2& rhs) const
{
	return Vector2(_x + rhs._x, _y + rhs._y);
}

Vector2 Vector2::Substract(const Vector2& rhs) const
{
	return Vector2(_x - rhs._x, _y - rhs._y);
}

Vector2 Vector2::Multiply(const Vector2& rhs) const
{
	return (*this) * rhs;
}

Vector2 Vector2::Divide(const Vector2& rhs) const
{
	return (*this) / rhs;
}
