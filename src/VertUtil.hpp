#include <SFML/Graphics.hpp>
#include <vector>
#include "Physics.hpp"

/*
Different functions for computing verticies from other types
*/

#ifndef VERT_CONV_UTIL_H
#define VERT_CONV_UTIL_H

// Uses provided color
inline void computeBoxFrameVerts(   const physics::Object& obj, std::vector<sf::Vertex>& verts, 
                                    sf::Color color, float _lineWidth = 2.f) {
    Rect<float> rect = obj.boundingBox;
    // top
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left, rect.top), color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + rect.width, rect.top), color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + rect.width, rect.top + _lineWidth), color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left, rect.top + _lineWidth), color));
    // left
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left, rect.top), color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + _lineWidth, rect.top), color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + _lineWidth, rect.top + rect.height), color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left, rect.top + rect.height), color));
    // right
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + rect.width - _lineWidth, rect.top), color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + rect.width, rect.top), color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + rect.width, rect.top + rect.height), color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + rect.width - _lineWidth, rect.top + rect.height), color));
    // bottom
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left, rect.top + rect.height - _lineWidth), color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + rect.width, rect.top + rect.height - _lineWidth), color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + rect.width, rect.top + rect.height), color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left, rect.top + rect.height), color));
}

// uses the physics objects color
inline void computeBoxFrameVerts(   const physics::Object& obj, std::vector<sf::Vertex>& verts, 
                                    float _lineWidth = 2.f)
{
    Rect<float> rect = obj.boundingBox;
    // top
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left, rect.top), obj.color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + rect.width, rect.top), obj.color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + rect.width, rect.top + _lineWidth), obj.color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left, rect.top + _lineWidth), obj.color));
    // left
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left, rect.top), obj.color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + _lineWidth, rect.top), obj.color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + _lineWidth, rect.top + rect.height), obj.color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left, rect.top + rect.height), obj.color));
    // right
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + rect.width - _lineWidth, rect.top), obj.color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + rect.width, rect.top), obj.color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + rect.width, rect.top + rect.height), obj.color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + rect.width - _lineWidth, rect.top + rect.height), obj.color));
    // bottom
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left, rect.top + rect.height - _lineWidth), obj.color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + rect.width, rect.top + rect.height - _lineWidth), obj.color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + rect.width, rect.top + rect.height), obj.color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left, rect.top + rect.height), obj.color));
}

inline void computeBoxFrameVerts(   const Rect<float>& rect, std::vector<sf::Vertex>& verts, 
                                    sf::Color color = sf::Color::White, float _lineWidth = 1.f)
{
    // top
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left, rect.top), color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + rect.width, rect.top + _lineWidth), color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + rect.width, rect.top), color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left, rect.top + _lineWidth), color));
    // left
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left, rect.top), color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + _lineWidth, rect.top), color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + _lineWidth, rect.top + rect.height), color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left, rect.top + rect.height), color));
    // right
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + rect.width - _lineWidth, rect.top), color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + rect.width, rect.top), color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + rect.width, rect.top + rect.height), color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + rect.width - _lineWidth, rect.top + rect.height), color));
    // bottom
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left, rect.top + rect.height - _lineWidth), color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + rect.width, rect.top + rect.height - _lineWidth), color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left + rect.width, rect.top + rect.height), color));
    verts.push_back(sf::Vertex(sf::Vector2f(rect.left, rect.top + rect.height), color));
}

// computes the verticies that make up the wireframe outline of a physics object's
// bounding box and places it in the outVerts vertex vector
inline void computeBoxFrameVertsFromVec(std::vector<Rect<float>>& rects, std::vector<sf::Vertex>& outVerts,
                                        sf::Color color, float lineWidth = 2.f) {
    outVerts.reserve(outVerts.size() + (rects.size() * 16));
    for (const Rect<float> rect : rects) {
        // top
        outVerts.emplace_back(sf::Vector2f(rect.left, rect.top), color);
        outVerts.emplace_back(sf::Vector2f(rect.left + rect.width, rect.top), color);
        outVerts.emplace_back(sf::Vector2f(rect.left + rect.width, rect.top + lineWidth), color);
        outVerts.emplace_back(sf::Vector2f(rect.left, rect.top + lineWidth), color);
        // left
        outVerts.emplace_back(sf::Vector2f(rect.left, rect.top), color);
        outVerts.emplace_back(sf::Vector2f(rect.left + lineWidth, rect.top), color);
        outVerts.emplace_back(sf::Vector2f(rect.left + lineWidth, rect.top + rect.height), color);
        outVerts.emplace_back(sf::Vector2f(rect.left, rect.top + rect.height), color);
        // right
        outVerts.emplace_back(sf::Vector2f(rect.left + rect.width - lineWidth, rect.top), color);
        outVerts.emplace_back(sf::Vector2f(rect.left + rect.width, rect.top), color);
        outVerts.emplace_back(sf::Vector2f(rect.left + rect.width, rect.top + rect.height), color);
        outVerts.emplace_back(sf::Vector2f(rect.left + rect.width - lineWidth, rect.top + rect.height), color);
        // bottom
        outVerts.emplace_back(sf::Vector2f(rect.left, rect.top + rect.height - lineWidth), color);
        outVerts.emplace_back(sf::Vector2f(rect.left + rect.width, rect.top + rect.height - lineWidth), color);
        outVerts.emplace_back(sf::Vector2f(rect.left + rect.width, rect.top + rect.height), color);
        outVerts.emplace_back(sf::Vector2f(rect.left, rect.top + rect.height), color);
    }
}

template<typename T>
inline void computeRectVerts(   const Rect<T>& rect, std::vector<sf::Vertex>& outVerts, 
                                const sf::Color& color) {
    outVerts.reserve(outVerts.size() + 4);
    outVerts.emplace_back(sf::Vector2f(rect.left, rect.top), color);
    outVerts.emplace_back(sf::Vector2f(rect.getRight(), rect.top), color);
    outVerts.emplace_back(sf::Vector2f(rect.getRight(), rect.getBottom()), color);
    outVerts.emplace_back(sf::Vector2f(rect.left, rect.getBottom()), color);
}

template<typename T>
inline void computeRectVertsFromVec(std::vector<Rect<T>>& rects, std::vector<sf::Vertex>& outVerts, 
                                    sf::Color& color) {
    outVerts.reserve(outVerts.size() + rects.size() * 4);
    for (const Rect<T>& rect : rects) {
        outVerts.emplace_back(sf::Vector2f(rect.left, rect.top), color);
        outVerts.emplace_back(sf::Vector2f(rect.getRight(), rect.top), color);
        outVerts.emplace_back(sf::Vector2f(rect.getRight(), rect.getBottom()), color);
        outVerts.emplace_back(sf::Vector2f(rect.left, rect.getBottom()), color);
    }
}

// uses reserve and emplace back; faster than using push_back 
inline void computePhysicsObjectVertsFromVec(   std::vector<physics::Object>& objects, 
                                                std::vector<sf::Vertex>& outVerts) {
    outVerts.reserve(outVerts.size() + objects.size());
    for (const physics::Object& obj : objects) {
        outVerts.emplace_back(sf::Vector2f(obj.boundingBox.left, obj.boundingBox.top), obj.color);
        outVerts.emplace_back(sf::Vector2f(obj.boundingBox.getRight(), obj.boundingBox.top), obj.color);
        outVerts.emplace_back(sf::Vector2f(obj.boundingBox.getRight(), obj.boundingBox.getBottom()), obj.color);
        outVerts.emplace_back(sf::Vector2f(obj.boundingBox.left, obj.boundingBox.getBottom()), obj.color);
    }
}

inline void toSfVertex(std::vector<Vec2f>& vecs, std::vector<sf::Vertex>& outVerts, sf::Color color) {
    outVerts.reserve(outVerts.size() + vecs.size());
    for (const Vec2f& vec : vecs) {
        outVerts.emplace_back(sf::Vertex(sf::Vector2f(vec.x, vec.y), color));
    }
}

#endif