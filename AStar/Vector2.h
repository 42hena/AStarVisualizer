#ifndef __VECTOR2_H__
#define __VECTOR2_H__

#include <Windows.h>

class Vector2
{
#pragma region 특수 맴버 함수
public:
    Vector2(int x = 0, int y = 0);
    ~Vector2() = default;

    Vector2(const Vector2& rhs)				= default;
    Vector2(Vector2&& rhs)                  = default;
    Vector2& operator=(const Vector2& rhs)	= default;
    Vector2& operator=(Vector2&& rhs)       = default;
#pragma endregion

public:
    Vector2 operator+(const Vector2& rhs) const;
    Vector2 operator-(const Vector2& rhs) const;
    Vector2 operator*(const Vector2& rhs) const;
    Vector2 operator/(const Vector2& rhs) const;

    bool operator==(const Vector2& rhs) const;
    bool operator!=(const Vector2& rhs) const;

    Vector2& operator+=(const Vector2& rhs);
    Vector2& operator-=(const Vector2& rhs);
    Vector2& operator*=(const Vector2& rhs);
    Vector2& operator/=(const Vector2& rhs);

    bool operator<(const Vector2& rhs) const
    {
        if (_x == rhs._x)
            return _y > rhs._y;
        return _x > rhs._x;
    }
public:
    const char* ToString();

public:
    Vector2 Add(const Vector2& rhs) const;
    Vector2 Substract(const Vector2& rhs) const;
    Vector2 Multiply(const Vector2& rhs) const;
    Vector2 Divide(const Vector2& rhs) const;


public:
    static Vector2 Zero;
    static Vector2 One;
    static Vector2 Up;
    static Vector2 Left;
    static Vector2 Right;
    static Vector2 Down;

#pragma region 맴버 변수
public:
    int _x = 0;
    int _y = 0;
#pragma endregion
};

#endif
