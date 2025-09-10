#ifndef VERT_CONV_UTIL_H
#define VERT_CONV_UTIL_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Physics.hpp"

//
// Various functions for computing vertices from other types of shapes
//

// computes the vertices for rendering a wireframe rectangle
inline void computeBoxFrameVerts(
    const Rect<float>& rect,
    std::vector<sf::Vertex>& verts,
    sf::Color color = sf::Color::White,
    float _lineWidth = 1.f
) {
    /*
      0 1         2 3
       ______________
    4 |             | 5
      |             |  
      |             |  
      |             |  
    6 |             | 7
      _______________
      8 9        10 11
    */

    sf::Vertex boxVerts[12]
    {
        sf::Vertex{sf::Vector2f(rect.left, rect.top), color},
        sf::Vertex{sf::Vector2f(rect.left + _lineWidth, rect.top), color},
        sf::Vertex{sf::Vector2f(rect.getRight() - _lineWidth, rect.top), color},
        sf::Vertex{sf::Vector2f(rect.getRight(), rect.top), color},
        sf::Vertex{sf::Vector2f(rect.left, rect.top + _lineWidth), color},
        sf::Vertex{sf::Vector2f(rect.getRight(), rect.top + _lineWidth), color},
        sf::Vertex{sf::Vector2f(rect.left, rect.getBottom() - _lineWidth), color},
        sf::Vertex{sf::Vector2f(rect.getRight(), rect.getBottom() - _lineWidth), color},
        sf::Vertex{sf::Vector2f(rect.left, rect.getBottom()), color},
        sf::Vertex{sf::Vector2f(rect.left + _lineWidth, rect.getBottom()), color},
        sf::Vertex{sf::Vector2f(rect.getRight() - _lineWidth, rect.getBottom()), color},
        sf::Vertex{sf::Vector2f(rect.getRight(), rect.getBottom()), color}
    };

    verts.reserve(24);

    // top
    verts.emplace_back(boxVerts[0]);
    verts.emplace_back(boxVerts[3]);
    verts.emplace_back(boxVerts[5]);
    verts.emplace_back(boxVerts[0]);
    verts.emplace_back(boxVerts[5]);
    verts.emplace_back(boxVerts[4]);

    //left
    verts.emplace_back(boxVerts[0]);
    verts.emplace_back(boxVerts[1]);
    verts.emplace_back(boxVerts[9]);
    verts.emplace_back(boxVerts[0]);
    verts.emplace_back(boxVerts[9]);
    verts.emplace_back(boxVerts[8]);

    // right 
    verts.emplace_back(boxVerts[2]);
    verts.emplace_back(boxVerts[3]);
    verts.emplace_back(boxVerts[11]);
    verts.emplace_back(boxVerts[2]);
    verts.emplace_back(boxVerts[11]);
    verts.emplace_back(boxVerts[10]);

    // bottom
    verts.emplace_back(boxVerts[6]);
    verts.emplace_back(boxVerts[7]);
    verts.emplace_back(boxVerts[11]);
    verts.emplace_back(boxVerts[6]);
    verts.emplace_back(boxVerts[11]);
    verts.emplace_back(boxVerts[8]);
}

// Computes the vertices for rendering an array of rectangle wireframes
inline void computeBoxFrameVertsFromVec(
    std::vector<Rect<float>>& rects, 
    std::vector<sf::Vertex>& outVerts,
    sf::Color color, 
    float lineWidth = 2.f
) {
    for (const Rect<float> rect : rects) 
    {
        computeBoxFrameVerts(rect, outVerts, color, lineWidth);
    }
}

// Computes the vertices for rendering a solid rectangle
template<typename T>
inline void computeRectVerts(
    const Rect<T>& rect, 
    std::vector<sf::Vertex>& outVerts,
    const sf::Color& color
) {
    outVerts.reserve(outVerts.size() + 6);
    outVerts.emplace_back(sf::Vertex{sf::Vector2f(rect.left, rect.top), color});
    outVerts.emplace_back(sf::Vertex{sf::Vector2f(rect.getRight(), rect.top), color});
    outVerts.emplace_back(sf::Vertex{sf::Vector2f(rect.getRight(), rect.getBottom()), color});
    outVerts.emplace_back(sf::Vertex{sf::Vector2f(rect.left, rect.top), color}),
    outVerts.emplace_back(sf::Vertex{sf::Vector2f(rect.getRight(), rect.getBottom()), color}),
    outVerts.emplace_back(sf::Vertex{sf::Vector2f(rect.left, rect.getBottom()), color});
}

// computes the vertices for rendering an array of solid rectanlges
template<typename T>
inline void computeRectVertsFromVec(
    std::vector<Rect<T>>& rects, 
    std::vector<sf::Vertex>& outVerts, 
    sf::Color& color
) {
    for (const Rect<T>& rect : rects) {
        computeRectVerts(rect, outVerts, color);
    }
}

// computes the vertices for rendering an array of physics objects
inline void computePhysicsObjectVerts(
    std::vector<physics::Object>& objects,
    std::vector<sf::Vertex>& outVerts
) {
    outVerts.reserve(outVerts.size() + objects.size() * 6);
    for (const physics::Object& obj : objects) {
        outVerts.emplace_back(sf::Vertex{sf::Vector2f(obj.boundingBox.left, obj.boundingBox.top), obj.color});
        outVerts.emplace_back(sf::Vertex{sf::Vector2f(obj.boundingBox.getRight(), obj.boundingBox.top), obj.color});
        outVerts.emplace_back(sf::Vertex{sf::Vector2f(obj.boundingBox.getRight(), obj.boundingBox.getBottom()), obj.color});
        outVerts.emplace_back(sf::Vertex{sf::Vector2f(obj.boundingBox.left, obj.boundingBox.top), obj.color});
        outVerts.emplace_back(sf::Vertex{sf::Vector2f(obj.boundingBox.getRight(), obj.boundingBox.getBottom()), obj.color});
        outVerts.emplace_back(sf::Vertex{sf::Vector2f(obj.boundingBox.left, obj.boundingBox.getBottom()), obj.color});
    }
}

#endif