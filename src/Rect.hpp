#pragma once
#include <math.h>
#include "Vec2.hpp"


template<typename T>
class Rect {
public:
    T left;
    T top;
    T width;
    T height;

    constexpr Rect(T _left = 0, T _top = 0, T _width = 0, T _height = 0) noexcept : 
        left(_left), top(_top), width(_width), height(_height) {}
    
    constexpr Rect(const Vec2<T>& position, const Vec2<T>& size) noexcept :
        left(position.x), top(position.y), width(size.x), height(size.y) {}

    constexpr T getRight() const noexcept {
        return left + width;
    }

    constexpr T getBottom() const noexcept {
        return top + height;
    }

    constexpr Vec2<T> getTopLeft() const noexcept {
        return Vec2<T>(left, top);
    }

    constexpr Vec2<T> getCenter() const noexcept {
        return Vec2<T>(left + width / 2, top + height / 2);
    }

    constexpr Vec2<T> getSize() const noexcept {
        return Vec2<T>(width, height);
    }

    const bool operator ==(const Rect& rect) {
        return left == rect.left && top == rect.top && width == rect.width && height == rect.height;
    }

    void setPosition(const Vec2<T>& position) {
        left = position.x;
        top = position.y;
    }

    void setPosition(Vec2<T>& position) {
        left = position.x;
        top = position.y;
    }

    void setPosition(const T& x, const T& y) {
        left = x;
        top = y;
    }

    void setPosition(T& x, T& y) {
        left = x;
        top = y;
    }
    
    constexpr bool contains(const Rect<T>& rect) const noexcept {
        return left <= rect.left && rect.getRight() <= getRight() && 
            top <= rect.top && rect.getBottom() <= getBottom();
    }

    constexpr bool contains(const Vec2<T>& vec) const noexcept {
        return vec.x <= getRight() && vec.x >= left &&
            vec.y <= getBottom() && vec.y >= top;
    }

    constexpr bool intersects(const Rect<T>& rect) const noexcept {
        return !(left >= rect.getRight() || getRight() <= rect.left ||
            top >= rect.getBottom() || getBottom() <= rect.top);
    }
};