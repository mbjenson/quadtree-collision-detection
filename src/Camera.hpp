#include <SFML/Graphics.hpp>
#include "Vec2.hpp"

class Camera : public sf::View {
    public:
        Camera(sf::FloatRect dim) : sf::View(dim) {}

        void zoomIn(float dt, sf::Vector2u windowDim, Vec2f centerToMouse) {
            currZoom -= zoomSpeed * dt;
            if (currZoom < minZoom) {
                currZoom = minZoom;
            }
            setSize({windowDim.x * currZoom, windowDim.y * currZoom});
            move({(centerToMouse.x / currZoom) * dt, (centerToMouse.y / currZoom) * dt});
        }

        void zoomOut(float dt, sf::Vector2u windowDim, Vec2f centerToMouse) {
            currZoom += zoomSpeed * dt;
            if (currZoom > maxZoom) {
                currZoom = maxZoom;
            }
            setSize({windowDim.x * currZoom, windowDim.y * currZoom});
            move({(-centerToMouse.x / currZoom ) * dt, (-centerToMouse.y / currZoom ) * dt});
        }

        void resetZoom(sf::Vector2u windowDim) {
            currZoom = defaultZoom;
            setSize({
                static_cast<float>(windowDim.x),
                static_cast<float>(windowDim.y)
            });
            setCenter({
                static_cast<float>(windowDim.x) / 2.0f, 
                static_cast<float>(windowDim.y) / 2.0f
            });
        }

        float getCurrZoom() {
            return currZoom;
        }

    private:
        const float maxZoom = 3.0f;
        const float minZoom = 0.05f;
        const float defaultZoom = 1.f;
        const float zoomSpeed = 3.f;

        const float movementSpeed = 200.f;

        float currZoom = 1.f;
};