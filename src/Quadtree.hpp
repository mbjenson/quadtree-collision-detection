#pragma once
#include <iostream>
#include <type_traits>
#include <math.h>
#include <vector>
#include <array>
#include <memory>
#include <cassert>
#include "Vec2.hpp"
#include "Rect.hpp"

/**
 * @brief quad tree data structure class.
 *          Computes bounding boxes on the fly to save space.
 * 
 * @param T         : data type stored in quadtree. Should be POD type or pointer
 * @param GetRect   : callable used to get a rect from the data stored in the quadtree.
 * 
 *                      The simplest way to use the quadtree with a prexisting class is to create a 
 *                      struct MyStruct and overload its operator() method with the appropriate
 *                      'GetRect' functionality. Now, MyStruct is a callable object (i.e. MyStruct()).
 * 
*                       e.g., if the quadtree is storing 'some object' pointer, inside which is a bounding box,
*                       my GetRect callable will be a function that, given the pointer, returns the
*                       bounding box therein.
 *                   
 * @param Equal     : callable used to test equality between two values stored in quadtree. By default,
 *                      uses std::equal_to which works in most basic cases I have tested with object pointers.
*/
template<typename T, typename GetRect, typename Equal = std::equal_to<T>>
class Quadtree {

    static_assert(std::is_convertible_v<std::invoke_result_t<GetRect, const T&>, Rect<float>>,
        "GetRect must be a callable signature Rect<float>(const T&)");
    static_assert(std::is_convertible_v<std::invoke_result_t<Equal, const T&, const T&>, bool>,
        "Equal must be a callable of signature bool(const T&, const T&)");

public:
    Quadtree(const Rect<float>& rect, const GetRect& getRect = GetRect(), const Equal& equal = Equal()) : 
        mRect(rect),
        mRoot(std::make_unique<Node>()),
        mGetRect(getRect),
        mEqual(equal)
    {}

    Quadtree() {}
   
    // Public versions which call the private, auxillary methods

    void add(const T& value) {
        add(mRoot.get(), 0, mRect, value);
    }

    void remove(const T& value) {
        remove(mRoot.get(), mRect, value);
    }

    void clearAll() {
        removeAllValues(mRoot.get());
    }

    std::vector<T> query(const Rect<float>& rect) const {
        auto values = std::vector<T>();
        query(mRoot.get(), mRect, rect, values);
        return values;
    }
    
    std::unordered_map<T, T> findAllIntersections() const {
        auto intersections = std::unordered_map<T, T>();
        findAllIntersections(mRoot.get(), intersections);
        return intersections;
    }

    void getNodeRects(std::vector<Rect<float>>& rects) {
        rects.push_back(mRect);
        getNodeRects(rects, mRoot.get(), mRect);
    }

    int getNumObjects() {
        return getNumContents(mRoot.get());
    }

    int getNumNodes() {
        return 1 + getNumNodes(mRoot.get());
    }
    
private:

    static constexpr auto Threshold = std::size_t(16);
    static constexpr auto MaxDepth = std::size_t(8);

    struct Node {
        std::array<std::unique_ptr<Node>, 4> children;
        std::vector<T> values;
    };

    Rect<float> mRect;
    std::unique_ptr<Node> mRoot;
    GetRect mGetRect;
    Equal mEqual;
    
    /**
     * @brief return true if 'node' has no children
    */
    bool isLeaf(const Node* node) const {
        return !static_cast<bool>(node->children[0]);
    }

    /**
     * @brief takes node rect in quadtree and quadrant index and creates appropriate child rect
     * 
     * @param rect  : parent rect
     * @param i     : quadtrant index (indexed from left to right in row major order)
     * @return      : child rect wihch is used to init a new node
    */
    Rect<float> computeRect(const Rect<float>& rect, int i) const {
        auto origin = rect.getTopLeft();
        auto childSize = rect.getSize() / 2.0;
        switch(i) {
            case 0:
                return Rect<float>(origin, childSize);
            case 1:
                return Rect<float>(Vec2<float>(origin.x + childSize.x, origin.y), childSize);
            case 2:
                return Rect<float>(Vec2<float>(origin.x, origin.y + childSize.y), childSize);
            case 3:
                return Rect<float>(Vec2<float>(origin.x + childSize.x, origin.y + childSize.y), childSize);
            default:
                assert(false && "Invalid child index");
                return Rect<float>();
        }
    }

    /**
     * @brief takes a quadtree node rect and a value rect and returns which 
     *          quadrant the value rect lies in
     * 
     * @param nodeRect  : parent rect
     * @param valueRect : rect associated with a value contained in quadtree
     * @return          : quadtrant number
    */
    int getQuadrant(const Rect<float>& nodeRect, const Rect<float>& valueRect) const {
        auto center = nodeRect.getCenter();
        if (valueRect.getRight() < center.x) {
            if (valueRect.getBottom() < center.y)
                return 0;
            else if (valueRect.top >= center.y)
                return 2;
            else
                return -1;
        }
        else if (valueRect.left >= center.x) {
            if (valueRect.getBottom() < center.y)
                return 1;
            else if (valueRect.top >= center.y)
                return 3;
            else
                return -1;
        }
        else
            return -1;
    }

    /**
     * @brief auxillary method to add a node to quadtree
     * 
     * @param node  : node attempting to add value to
     * @param depth : current node depth into quadtree
     * @param rect  : bounding rect associated with node
     * @param value : value to be added to quadtree
    */
    void add(Node* node, std::size_t depth, const Rect<float>& rect, const T& value) {
        assert(node != nullptr);
        assert(rect.contains(mGetRect(value)));
        if (isLeaf(node)) {
            // if possible, insert the value in this node
            if (depth >= MaxDepth || node->values.size() < Threshold)
                node->values.push_back(value);
            // otherwise split the node and retry add
            else {
                split(node, rect);
                add(node, depth, rect, value);
            }
        }
        else {
            auto i = getQuadrant(rect, mGetRect(value));
            // add the value in a child if the value is entirely contained in it
            if (i != -1) {
                add(
                    node->children[static_cast<std::size_t>(i)].get(), 
                    depth + 1, computeRect(rect, i), value);
            }
            else {
                node->values.push_back(value);
            }
        }
    }

    /**
     * @brief split node into 4 children
     * 
     * @param node  : node to be split
     * @param rect  : bounding rect of node
    */
    void split(Node* node, const Rect<float>& rect) {
        assert(node != nullptr);
        assert(isLeaf(node) && "Only leaves can be split");
        // create children
        for (auto& child : node->children)
            child = std::make_unique<Node>();
        // assign values to children
        auto newValues = std::vector<T>();
        for (const auto& value : node->values) {
            auto i = getQuadrant(rect, mGetRect(value));
            if (i != -1)
                node->children[static_cast<std::size_t>(i)]->values.push_back(value);
            else
                newValues.push_back(value);
        }
        node->values = std::move(newValues);
    }

    /**
     * @brief auxillary method that removes node from the quadtree 
     * 
     * @param node  : quadtree node which contains value
     * @param rect  : rect assocated with node
     * @param value : value to be removed from quadtree
     * 
    */
    bool remove(Node* node, const Rect<float>& rect, const T& value) {
        assert(node != nullptr);
        assert(rect.contains(mGetRect(value)));
        if (isLeaf(node)) {
            removeValue(node, value);
            return true;
        }
        else {
            // remove value in a child if the value is entirely contained within it
            auto i = getQuadrant(rect, mGetRect(value));
            if (i != -1) {
                if (remove(
                    node->children[static_cast<std::size_t>(i)].get(), 
                    computeRect(rect, i), value)) {  // old
                    return tryMerge(node);
                }
            }
            // Otherwise, we remove the value from the current node
            else
                removeValue(node, value);
            return false;
        }
    }
    
    /**
     * @brief removes value from the contents of the node
     * 
     * @param node  : node containing value
     * @param value : value to be removed
    */
    void removeValue(Node* node, const T& value) {
        // find value in node->values
        auto it = std::find_if(
                std::begin(node->values),
                std::end(node->values),
                [this, &value](const auto& rhs)
                {
                    return mEqual(value, rhs);
                });
        if (it == std::end(node->values)) {
            std::cout << "value not present in node\n";
        }
        else {
            *it = std::move(node->values.back());
            node->values.pop_back();
        }
    }

    /**
     * @brief recursive function to forceably removes all values from the quadtree
     * @note created to speed up the process of whiping the quadtree. Works well for the
     *          purposes of this project however, other implementations may wish to employ
     *          a more 'purposful' or 'smart' removal operation.
     * 
     * @param node  : parent node whose values will be removed
    */
    bool removeAllValues(Node* node) {
        if (!isLeaf(node)) {
            bool success = true;
            for (auto& child : node->children) {
                if (success) {
                    success = removeAllValues(child.get());
                }
            }
            tryMerge(node);
        }
        node->values.clear();
        return true;
    }

    /**
     * @brief merges a node if the total number of values stored in itself and its children
     *          are less than the defined threshold.
     * 
     * @param node  : quadtree node whose children will try to be merged
     * @return      : true on successful merge
    */
    bool tryMerge(Node* node) {
        assert(node != nullptr);
        assert(!isLeaf(node) && "only interior nodes can be merged");
        auto numValues = node->values.size();
        for (const auto& child : node->children) {
            if (!isLeaf(child.get()))
                return false;
                //continue;
                //return false; old
            numValues += child->values.size();
        }
        if (numValues <= Threshold) {
            node->values.reserve(numValues);
            // merge values of all children
            for (const auto& child : node->children) {
                for (const auto& value : child->values)
                    node->values.push_back(value);
            }
            // remove the children
            for (auto& child : node->children)
                child.reset();
            return true;
        }
        else
            return false;
    }

    /**
     * @brief returns the collisions taking place with a novel queryRect
     * 
     * @param node      : quadtree node in question
     * @param rect      : rect associated with quadtree node
     * @param queryRect : novel rect for which new collisions are to be evaluated
     * @param values    : vector that will hold any found value-queryRect intersections
    */
    void query(
        Node* node, 
        const Rect<float>& rect, 
        const Rect<float>& queryRect, 
        std::vector<T>& values) const 
    {
        assert(node != nullptr);
        assert(queryRect.intersects(rect));
        for (const auto& value : node->values) {
            if (queryRect.intersects(mGetRect(value)))
                values.push_back(value);
        }
        if (!isLeaf(node)) {
            for (auto i = std::size_t(0); i < node->children.size(); ++i) {
                auto childRect = computeRect(rect, static_cast<int>(i));
                if (queryRect.intersects(childRect))
                    query(node->children[i].get(), childRect, queryRect, values);
            }
        }
    }
    
    /**
     * @brief returns map of all intersecting objects in the quadtree
     * @note uses unordered_map to check for transitive/duplicate object pairs: (a-b & b-a)
     * 
     * @param node          : node whose values are be evaluated for collisions
     * @param intersections : map of all value-value intersections
    */
    // returns map of all intersecting objects (removes duplicates)
    void findAllIntersections(Node* node, std::unordered_map<T, T>& intersections) const {
        for (int i = 0; i < node->values.size(); ++i) {
            for (int j = 0; j < node->values.size(); ++j) {
                if (j == i){ // remove self intersections
                    continue;
                }
                if (mGetRect(node->values[i]).intersects(mGetRect(node->values[j]))) {
                    // check for duplicate pairs
                    auto iLoc = intersections.find(node->values[i]);
                    auto jLoc = intersections.find(node->values[j]);
                    if (iLoc != intersections.end()) { // if found
                        if (iLoc->second = node->values[j]) { // no need to check for corresponding value; key always has stored value
                            continue;   
                        }
                    }
                    if (jLoc != intersections.end()) {
                        if (jLoc->second = node->values[i]) {
                            continue;
                        }
                    }
                    else {
                        intersections.insert(std::make_pair(node->values[i], node->values[j]));
                    }
                }
            }
        }
        if (!isLeaf(node)) {
            // Values in this node can intersect values in descendants
            for (const auto& child : node->children) {
                for (const auto& value : node->values) {
                    findIntersectionsInDescendants(child.get(), value, intersections);
                }
            }
            // find intersections in children
            for (const auto& child : node->children) {
                findAllIntersections(child.get(), intersections);
            }
        }
    }

    /**
     * @brief recursively checks for any value-value intersections in the descendants of a node
     *          which involve the given value.
     * @note allows bounding boxes to overlap into several quadtree nodes and still be properly
     *          evaluated for any collisions with smaller objects
     * 
     * @param node          : node that contains value
     * @param value         : collisions are checked against this value
     * @param intersections : map of value-value intersections
    */
    void findIntersectionsInDescendants(
        Node* node, 
        const T& value, 
        std::unordered_map<T, T>& intersections) const 
    {
        for (const auto& other : node->values) {
            if (mGetRect(value).intersects(mGetRect(other))) {
                intersections.insert(std::make_pair(value, other));
            }
        }
        if (!isLeaf(node)) {
            for (const auto& child : node->children) {
                findIntersectionsInDescendants(child.get(), value, intersections);
            }
        }
    }
    
    void getNodeRects(std::vector<Rect<float>>& rects, Node* curNode, Rect<float>& pRect) {
        Node* thisNode = curNode;
        if (!isLeaf(thisNode)) {
            for (int i = 0; i < 4; i++) { // add each child rect to rects
                rects.push_back(computeRect(pRect, i));
                getNodeRects(rects, thisNode->children[i].get(), rects.back());
            }
        } 
    }

    int getNumContents(Node* node) {
        int numContents = 0;
        numContents += node->values.size();
        if (!isLeaf(node)) {
            numContents += getNumContents(node->children[0].get());
            numContents += getNumContents(node->children[1].get());
            numContents += getNumContents(node->children[2].get());
            numContents += getNumContents(node->children[3].get());
        }
        return numContents;
    }

    int getNumNodes(Node* node) {
        int numNodes = 0;
        if (!isLeaf(node)) {
            numNodes += 4;
            numNodes += getNumNodes(node->children[0].get());
            numNodes += getNumNodes(node->children[1].get());
            numNodes += getNumNodes(node->children[2].get());
            numNodes += getNumNodes(node->children[3].get());
        }
        return numNodes;
    }
};
