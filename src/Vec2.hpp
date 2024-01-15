#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string.h>
#include <math.h>


template <typename T>
class Vec2 {
public:
    T x;
    T y;

    constexpr Vec2(T X = 0, T Y = 0) noexcept : x(X), y(Y) {}
    // SFML DEPENDANT
    constexpr Vec2(sf::Vector2<T> sfVec) noexcept : x(sfVec.x), y(sfVec.y) {}

    const T getMag() {
        return sqrt(x * x + y * y);
    }

    const Vec2<T> getNormalized() {
        T mag = getMag();
        if (mag < 0.00001)
            return Vec2();
        T invMag = 1/mag;
        return Vec2(x * invMag, y * invMag);
    }

    void normalize() {
        T mag = getMag();
        if (mag < 0.00001)
            return;
        T invMag = 1/getMag();
        x = x*invMag;
        y = y*invMag;
    }
    
    const T dot(const Vec2<T>& v) {
        return (x * v.x) + (y * v.y);
    }

    const Vec2 operator +(const Vec2& v) {
        return Vec2(x + v.x, y + v.y);
    }
    
    const Vec2 operator -(const Vec2& v) {
        return Vec2(x - v.x, y - v.y);
    }

    void operator -() {
        x = -x;
        y = -y;
    }
    
    Vec2 operator *(const T& c) {
        return Vec2(x * c, y * c); 
    }
    
    const Vec2 operator /(const T& c) {
        T invC = 1/c;
        return Vec2(x * invC, y * invC);
    }
    
    const T operator *(const Vec2& v) {
        return x * v.x + y * v.y;
    }

    const bool operator ==(const Vec2& v) {
        return v.x == x && v.y == y;
    }

    const bool operator !=(const Vec2& v) {
        return v.x != x && v.y != y;
    }

    void operator +=(const Vec2& v) {
        x += v.x;
        y += v.y;
    }

    void operator -=(const Vec2& v) {
        x -= v.x;
        y -= v.y;
    }

    // SFML DEPENDANT
    const Vec2 operator =(const sf::Vector2<T>& v) {
        return Vec2<T>(v.x, v.y);
    }
};

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;