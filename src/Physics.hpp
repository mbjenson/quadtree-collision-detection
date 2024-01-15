#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Rect.hpp"
#include "Vec2.hpp"


/**
 * Rudementary physics system
*/
namespace physics {

class Object {
public:

    float mass;
    Vec2f velocity;
    Vec2f position; // top left corner of box
    Rect<float> boundingBox;
    sf::Color color;

    Object(Vec2f _position = Vec2f(), Vec2f _size = Vec2f(), float _mass = 1.f) :
        position(_position),
        boundingBox(Rect<float>(_position.x, _position.y,_size.x, _size.y)),
        mass(_mass)
    {}

    void update(float dt) {
        position = position + velocity * dt;
        boundingBox = Rect<float>(position.x, position.y, boundingBox.getSize().x, boundingBox.getSize().y);
    }

    void updateBoundingBox() {
        boundingBox = Rect<float>(position.x, position.y, boundingBox.getSize().x, boundingBox.getSize().y);
    }

    void move(Vec2f distance) {
        position = position + distance;
    }
};

// objects can exist inside of the boundary
class Boundary {
public:
    Rect<float> boundingBox;

    Boundary(Rect<float> _boundingBox = Rect<float>()) : boundingBox(_boundingBox) {}
    
    void checkCollision(Object& obj) {
        // if object is not inside boundary
        if (!boundingBox.contains(obj.boundingBox)) {
            // check which part is not inside
            if (obj.boundingBox.left < boundingBox.left) {
                obj.position.x += (boundingBox.left - obj.boundingBox.left);
                obj.velocity.x = -obj.velocity.x;
            }
            if (obj.boundingBox.getRight() > boundingBox.getRight()) {
                obj.position.x += (boundingBox.getRight() - obj.boundingBox.getRight());
                obj.velocity.x = -obj.velocity.x;
            }
            if (obj.boundingBox.top < boundingBox.top) {
                obj.position.y += (boundingBox.top - obj.boundingBox.top);
                obj.velocity.y = -obj.velocity.y;
            }
            if (obj.boundingBox.getBottom() > boundingBox.getBottom()) {
                obj.position.y += (boundingBox.getBottom() - obj.boundingBox.getBottom());
                obj.velocity.y = -obj.velocity.y;
            }
        }
    }
};

// class for handling groups of physics objects
// contains functionality to deal with collisions within those groups
class PhysicsHandler {
public:

    PhysicsHandler() {}

    // basic AABB collision resolution
    /// @brief resolve single collision between two physics Objects using AABB collision detection and simple resolution
    /// @param first physics object reference
    /// @param second physics object reference
    void resolveCol(physics::Object& first, physics::Object& second) {

        Rect<float> box1 = first.boundingBox;
        Rect<float> box2 = second.boundingBox;

        // distance the first object must move to solve collision in the _ direction
        float toRightDist = 0;
        float toLeftDist = 0;
        float toBottomDist = 0;
        float toTopDist = 0;

        float absToRightDist = 0;
        float absToLeftDist = 0;
        float absToTopDist = 0;
        float absToBotDist = 0;

        float firstMassPerc = first.mass / (first.mass + second.mass);
        float secondMassPerc = second.mass / (first.mass + second.mass);

        int eRes = 0; // in which direction was first corrected

        // check for box-in-box collision (for spawning in boxes)
        if (box1.contains(box2)) {

            toLeftDist = box2.getRight() - box1.left;
            toRightDist = box1.getRight() - box2.left;
            toTopDist = box2.getBottom() - box1.top;
            toBottomDist = box1.getBottom() - box2.top;

            absToRightDist = abs(toRightDist);
            absToLeftDist = abs(toLeftDist);
            absToTopDist = abs(toTopDist);
            absToBotDist = abs(toBottomDist);

            float dists[] = {absToRightDist, absToLeftDist, absToTopDist, absToBotDist};
            float min = *std::min_element(dists, dists + (sizeof(dists) / sizeof(dists[0])));

            if (min == absToRightDist) {
                first.move(Vec2f(-toRightDist * secondMassPerc, 0));
                second.move(Vec2f(toRightDist * firstMassPerc, 0));
                eRes = 0;
            }
            else if (min == absToLeftDist) {
                first.move(Vec2f(toLeftDist * secondMassPerc, 0));
                second.move(Vec2f(-toLeftDist * firstMassPerc, 0));
                eRes = 1;
            }
            else if (min == absToBotDist) {
                first.move(Vec2f(0, -toBottomDist * secondMassPerc));
                second.move(Vec2f(0, toBottomDist * firstMassPerc));
                eRes = 2;
            }
            else if (min == absToTopDist) {
                first.move(Vec2f(0, toTopDist * secondMassPerc));
                second.move(Vec2f(0, -toTopDist * firstMassPerc));
                eRes = 3;
            }
        }
        else if (box2.contains(box1)) {

            toLeftDist = box1.getRight() - box2.left;
            toRightDist = box2.getRight() - box1.left;
            toTopDist = box1.getBottom() - box2.top;
            toBottomDist = box2.getBottom() - box1.top;

            absToRightDist = abs(toRightDist);
            absToLeftDist = abs(toLeftDist);
            absToTopDist = abs(toTopDist);
            absToBotDist = abs(toBottomDist);

            float dists[] = {absToRightDist, absToLeftDist, absToTopDist, absToBotDist };
            float min = *std::min_element(dists, dists + (sizeof(dists) / sizeof(dists[0])));
            if (min == absToRightDist) {
                //std::cout << "move to the right\n";
                first.move(Vec2f(toRightDist * secondMassPerc, 0));
                second.move(Vec2f(-toRightDist * firstMassPerc, 0));
                eRes = 0;
            }
            else if (min == absToLeftDist) {
                first.move(Vec2f(-toLeftDist * secondMassPerc, 0));
                second.move(Vec2f(toLeftDist * firstMassPerc, 0));
                eRes = 1;
            }
            else if (min == absToBotDist) {
                first.move(Vec2f(0, toBottomDist * secondMassPerc));
                second.move(Vec2f(0, -toBottomDist * firstMassPerc));
                eRes = 2;
            }
            else if (min == absToTopDist) {
                first.move(Vec2f(0, -toTopDist * secondMassPerc));
                second.move(Vec2f(0, toTopDist * firstMassPerc));
                eRes = 3;
            }
        }
        else {
            int bEdges[4] = {0, 0, 0, 0}; // keep track of which edges are overlapping

            if (box1.left >= box2.left && box1.left <= box2.getRight()) {
                bEdges[0] = 1;
                toRightDist = (box2.getRight() - box1.left);
            }
            if (box1.getRight() <= box2.getRight() && box1.getRight() >= box2.left) {
                bEdges[1] = 1;
                toLeftDist = (box2.left - box1.getRight());
            }
            if (box1.top >= box2.top && box1.top <= box2.getBottom()) {
                bEdges[2] = 1;
                toBottomDist = (box2.getBottom() - box1.top);
            }
            if (box1.getBottom() <= box2.getBottom() && box1.getBottom() >= box2.top) {
                bEdges[3] = 1;
                toTopDist = (box2.top - box1.getBottom());
            }

            int bEdgeSum = 0; // get total num edge overlaps
            for (int i : bEdges) {
                bEdgeSum += i;
            }
            
            absToRightDist = abs(toRightDist);
            absToLeftDist = abs(toLeftDist);
            absToTopDist = abs(toTopDist);
            absToBotDist = abs(toBottomDist);

            if (bEdgeSum > 1) {
                if (bEdges[0] && bEdges[2]) {
                    if (absToRightDist <= absToBotDist) {
                        first.move(Vec2f(toRightDist * secondMassPerc, 0));
                        second.move(Vec2f(-toRightDist * firstMassPerc, 0));
                        eRes = 0;
                    }
                    else {
                        first.move(Vec2f(0, toBottomDist * secondMassPerc));
                        second.move(Vec2f(0, -toBottomDist * firstMassPerc));
                        eRes = 2;
                    }
                }
                else if (bEdges[0] && bEdges[3]) {
                    if (absToRightDist <= absToTopDist) {
                        first.move(Vec2f(toRightDist * secondMassPerc, 0));
                        second.move(Vec2f(-toRightDist * firstMassPerc, 0));
                        eRes = 0;
                    }
                    else {
                        first.move(Vec2f(0, toTopDist * secondMassPerc));
                        second.move(Vec2f(0, -toTopDist * firstMassPerc));
                        eRes = 3;
                    }
                }
                else if (bEdges[1] && bEdges[2]) {
                    if (absToLeftDist <= absToBotDist) {
                        first.move(Vec2f(toLeftDist * secondMassPerc, 0));
                        second.move(Vec2f(-toLeftDist * firstMassPerc, 0));
                        eRes = 1;
                    }
                    else {
                        first.move(Vec2f(0, toBottomDist * secondMassPerc));
                        second.move(Vec2f(0, -toBottomDist * firstMassPerc));
                        eRes = 2;
                    }
                }
                else if (bEdges[1] && bEdges[3]) {
                    if (absToLeftDist <= absToTopDist) {
                        first.move(Vec2f(toLeftDist * secondMassPerc, 0));
                        second.move(Vec2f(-toLeftDist * firstMassPerc, 0));
                        eRes = 1;
                    }
                    else {
                        first.move(Vec2f(0, toTopDist * secondMassPerc));
                        second.move(Vec2f(0, -toTopDist * firstMassPerc));
                        eRes = 3;
                    }
                }
            }
            else if (bEdgeSum == 1) {
                if (bEdges[0]) {
                    first.move(Vec2f(toRightDist * secondMassPerc, 0));
                    second.move(Vec2f(-toRightDist * firstMassPerc, 0));
                    eRes = 0;
                }
                if (bEdges[1]) {
                    first.move(Vec2f(toLeftDist * secondMassPerc, 0));
                    second.move(Vec2f(-toLeftDist * firstMassPerc, 0));
                    eRes = 1;
                }
                if (bEdges[2]) {
                    first.move(Vec2f(0, toBottomDist * secondMassPerc));
                    second.move(Vec2f(0, -toBottomDist * firstMassPerc));
                    eRes = 2;
                }
                if (bEdges[3]) {
                    first.move(Vec2f(0, toTopDist * secondMassPerc));
                    second.move(Vec2f(0, -toTopDist * firstMassPerc));
                    eRes = 3;
                }
            }
            else {
                return;
            }
        }
        first.updateBoundingBox();
        second.updateBoundingBox();

        // conservation of momentum
        Vec2f box1FinalVel;
        Vec2f box2FinalVel;

        box2FinalVel.x = 
            (2*(first.mass * first.velocity.x) + (second.mass * second.velocity.x) - 
            (first.mass * second.velocity.x)) / (first.mass + second.mass);
        box1FinalVel.x = 
            ((first.mass * first.velocity.x) + (second.mass * second.velocity.x) - 
            (second.mass * box2FinalVel.x)) / first.mass;

        box2FinalVel.y = 
            (2*(first.mass * first.velocity.y) + (second.mass * second.velocity.y) - 
            (first.mass * second.velocity.y)) / (first.mass + second.mass);
        box1FinalVel.y = 
            ((first.mass * first.velocity.y) + (second.mass * second.velocity.y) -
            (second.mass * box2FinalVel.y)) / first.mass;

        if (eRes == 0) { // if we moved box to the right
            first.velocity.x = box1FinalVel.x;
            second.velocity.x = box2FinalVel.x;
            // y remains unchanged
        }
        if (eRes == 1) { // if we moved to the left
            first.velocity.x = box1FinalVel.x;
            second.velocity.x = box2FinalVel.x;
            // y remains unchanged
        }
        if (eRes == 2) { // if we moved down
            // x remains unchanged
            first.velocity.y = box1FinalVel.y;
            second.velocity.y = box2FinalVel.y;
        }
        if (eRes == 3) { // if we moved up
            // x remains unchanged
            first.velocity.y = box1FinalVel.y;
            second.velocity.y = box2FinalVel.y;
        }
    }
    
    void resolveColsFromVec(std::vector<std::pair<physics::Object*, physics::Object*>>& collisionList) {
        for (std::pair<physics::Object*, physics::Object*> pair : collisionList) {
            resolveCol(*pair.first, *pair.second);
        }
    }

    // solves collisions from a map of of physics object key-value pairs
    void resolveColsFromMap(std::unordered_map<physics::Object*, physics::Object*>& collisionList) {
        for (std::pair<physics::Object*, physics::Object*> pair : collisionList) {
            resolveCol(*pair.first, *pair.second);
        }
    }
    
    // brute force given a list of physics objects
    void BRUTE_resolveCols(std::vector<physics::Object>& objects) {
        for (int i = 0; i < objects.size(); ++i) {
            for (int j = 0; j < objects.size(); ++j) {
                if (objects[i].boundingBox.intersects(objects[j].boundingBox)) {
                    resolveCol(objects[i], objects[j]);
                }
            }
        }
    }

};

}