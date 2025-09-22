#include <SFML/Graphics.hpp>
#include <iostream>

#include "Rect.hpp"
#include "Vec2.hpp"
#include "Physics.hpp"
#include "Quadtree.hpp"
#include "Camera.hpp"
#include "RandomUtil.hpp"
#include "VertUtil.hpp"

// callable for quadtree
struct GetRectPhys {
    Rect<float>& operator()(physics::Object* obj) const {
        return obj->boundingBox;
    }
};

namespace app {

struct AppSettings {
    bool useQuadtree = true;
    bool showNodes = true;
    int sceneNumObjects = 10;
    float objectSizeMax = 50.f;
    float objectSizeMin = 10.f;
    float objectVelMax = 50.f;
    float objectVelMin = 20.f;
};

class App {
        
    public:
        // set up the application
        App(AppSettings& appSettings) {
            if (!init(appSettings)) {
                std::cout << "failed to initialize application" << std::endl;
                return;
            }
            this->appSettings = appSettings;
        }

        void Run() {
            std::vector<Rect<float>> nodeRects;
            std::unordered_map<physics::Object*, physics::Object*> objColMap;
            std::vector<sf::Vertex> vertices;

            std::string winTextString;
            sf::Text winText = sf::Text(roboto);
            winText.setCharacterSize(20);
            winText.setFillColor(sf::Color::White);
            winText.setPosition({0, 0});

            int curNumNodes = 1;
            int curNumCollisions = 0;
            int curNumObjects = 0;
            int numFrames = 0;
            float fpsSum = 0;
            float fps;
            float dt;

            bool showNodes = appSettings.showNodes;
            bool useQuadTree = appSettings.useQuadtree;

            sf::Vector2f mouseWorldPos;
            Vec2f centerToMouse;

            while (window.isOpen()) {

                dt = clock.restart().asSeconds();
                fps = 1.f/(dt);
                numFrames++;
                fpsSum += fps;

                mouseWorldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                
                // compute center of view to mouse vector
                centerToMouse = Vec2f(
                    window.mapPixelToCoords(
                        sf::Mouse::getPosition(window)
                    ) - camera.getCenter()
                );
                float mag = centerToMouse.getMag();
                centerToMouse.normalize();
                centerToMouse = centerToMouse * mag * 0.25;

                // handle events

                while (const std::optional event = window.pollEvent()) {
                    if (event->is<sf::Event::Closed>() || 
                        event->is<sf::Event::KeyPressed>() && 
                        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) 
                    {
                        std::cout << "avg FPS = " << fpsSum / static_cast<float>(numFrames) << "\n";
                        window.close();
                    }
                    if (event->is<sf::Event::KeyPressed>()) 
                    {
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
                            if (world.contains(mouseWorldPos)) {
                                Vec2f newObjSize(
                                    getRandInRange(appSettings.objectSizeMin, appSettings.objectSizeMax), 
                                    getRandInRange(appSettings.objectSizeMin, appSettings.objectSizeMax));

                                physics::Object newObj(
                                    Vec2f(
                                        mouseWorldPos.x - (newObjSize.x / 2), 
                                        mouseWorldPos.y - (newObjSize.y / 2)), 
                                    newObjSize, newObjSize.x * newObjSize.y);
                                
                                newObj.velocity = Vec2f(
                                    getRandVelocity(appSettings.objectVelMin, appSettings.objectVelMax), 
                                    getRandVelocity(appSettings.objectVelMin, appSettings.objectVelMax));

                                newObj.color = colors[rand() % colors.size()];
                                pObjs.push_back(newObj);
                            }
                        }
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) { // reset view
                            camera.resetZoom(windowDim);
                        }
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N)) {
                            showNodes = !showNodes;
                        }
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::G)) {
                            if (world.contains(mouseWorldPos)) {
                                Vec2f newObjSize(Vec2f(80.0f, 80.0f));

                                physics::Object newObj(
                                    Vec2f(
                                        mouseWorldPos.x - (newObjSize.x / 2), 
                                        mouseWorldPos.y - (newObjSize.y / 2)), 
                                    newObjSize, newObjSize.x * newObjSize.y);
                                
                                newObj.velocity = Vec2f(
                                    getRandVelocity(appSettings.objectVelMin / 2, appSettings.objectVelMax / 2), 
                                    getRandVelocity(appSettings.objectVelMin / 2, appSettings.objectVelMax / 2));

                                newObj.color = sf::Color::White;
                                pObjs.push_back(newObj);
                            }
                        }
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) {
                            this->pObjs.clear();
                        }
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Equal)) {
                        camera.zoomIn(dt, windowDim, centerToMouse);
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Hyphen)) {
                        camera.zoomOut(dt, windowDim, centerToMouse);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || 
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
                    camera.moveLeft(dt);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || 
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
                    camera.moveUp(dt);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || 
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
                    camera.moveRight(dt);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || 
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
                    camera.moveDown(dt);
                }

                // update physics

                for (auto &obj : pObjs) {
                    obj.update(dt);
                }
                
                if (useQuadTree) {
                    for (int i = 0; i < pObjs.size(); i++) {
                        worldBounds.checkCollision(pObjs[i]);
                        quadTree.add(&pObjs[i]);
                    }
                    objColMap = quadTree.findAllIntersections();
                    curNumCollisions = objColMap.size();
                    curNumObjects = quadTree.getNumObjects();

                    pHandler.resolveColsFromMap(objColMap);

                    if (showNodes) {
                        quadTree.getNodeRects(nodeRects);
                        computeBoxFrameVertsFromVec(nodeRects, vertices, sf::Color(255, 255, 255, 120), 1.0f);
                        nodeRects.clear();
                    }
                    quadTree.clearAll();
                }
                else {
                    pHandler.BRUTE_resolveCols(pObjs);
                }
                computePhysicsObjectVerts(pObjs, vertices);

                computeBoxFrameVerts(worldBounds.boundingBox, vertices, sf::Color::Yellow, 2.f);
                objColMap.clear();
                
                // DRAW TO SCREEN
                window.clear();
                window.setView(camera);
                window.draw(&vertices[0], vertices.size(), sf::PrimitiveType::Triangles);
                vertices.clear();

                drawUI(window, winText, fps, curNumObjects, curNumCollisions);

                window.display();
            }
        }

    private:

        AppSettings appSettings;

        std::array<sf::Color, 12> colors = {
            sf::Color(255, 90, 51), sf::Color(255, 128, 0), 
            sf::Color(255, 255, 51), sf::Color(153, 255, 51), 
            sf::Color(51, 255, 51), sf::Color(51, 255, 153), 
            sf::Color(51, 255, 255), sf::Color(51, 153, 255),
            sf::Color(51, 51, 255), sf::Color(153, 51, 255), 
            sf::Color(255, 51, 255), sf::Color(255, 51, 153)};

        sf::Vector2u screenRes;
        Camera camera;
        sf::RenderWindow window;
        sf::Clock clock;
        sf::RenderTexture textCanvas;
        sf::Font roboto;

        physics::Boundary worldBounds;
        physics::PhysicsHandler pHandler;
        std::vector<physics::Object> pObjs;
        Rect<float> world;
        sf::Vector2u windowDim;
        
        Quadtree<physics::Object*, GetRectPhys> quadTree;

        bool init(AppSettings& appSettings) {
            
            // Init window

            screenRes = sf::VideoMode::getDesktopMode().size;
            int leastScreenDim = (screenRes.x < screenRes.y) ? screenRes.x : screenRes.y;
            leastScreenDim = leastScreenDim - (leastScreenDim / 10);
            windowDim = sf::Vector2u(leastScreenDim * 0.925, leastScreenDim);
            int leastWinDim = (windowDim.x < windowDim.y) ? windowDim.x : windowDim.y;

            Rect<int> worldDimI(
                static_cast<int>(windowDim.x * 0.04545), 
                static_cast<int>(windowDim.y * 0.125),
                static_cast<int>(leastWinDim - leastWinDim * 0.0909), 
                static_cast<int>(leastWinDim - leastWinDim * 0.0909));
            
            world = Rect<float>(  
                static_cast<float>(worldDimI.left), 
                static_cast<float>(worldDimI.top),
                static_cast<float>(worldDimI.width), 
                static_cast<float>(worldDimI.height));

            window = sf::RenderWindow{
                sf::VideoMode(windowDim), 
                "Quadtree Physics Demo", 
                sf::Style::Titlebar | sf::Style::Close};

            camera = Camera(
                sf::FloatRect(
                    {0, 0}, 
                    {static_cast<float>(windowDim.x), static_cast<float>(windowDim.y)}
                )
            );
            camera.resetZoom(windowDim);
            window.setView(camera);

            // Init physics

            worldBounds = physics::Boundary(world);

            quadTree = Quadtree<physics::Object*, GetRectPhys>(
                Rect<float>(
                    0, 
                    static_cast<int>(leastWinDim * 0.0909), 
                    leastWinDim, 
                    leastWinDim)
            );

            // init physics objects
            pObjs.reserve(appSettings.sceneNumObjects);
            for (int i = 0; i < appSettings.sceneNumObjects; i++) {
                physics::Object thisObj(
                    Vec2f(  
                        rand() % (worldDimI.width - 300) + worldDimI.left,
                        rand() % (worldDimI.height - 300) + worldDimI.top),
                    Vec2f(getRandInRange(appSettings.objectSizeMin, appSettings.objectSizeMax), 
                    getRandInRange(appSettings.objectSizeMin, appSettings.objectSizeMax)));
                
                thisObj.color = colors[rand() % colors.size()];
                thisObj.velocity = Vec2f(
                    getRandVelocity(appSettings.objectVelMin, appSettings.objectVelMax), 
                    getRandVelocity(appSettings.objectVelMin, appSettings.objectVelMax));
                thisObj.mass = thisObj.boundingBox.width * thisObj.boundingBox.height;
                pObjs.emplace_back(thisObj);
            }

            // load assets

            try {
                textCanvas = sf::RenderTexture({windowDim.x, windowDim.y / 9});
            }
            catch (sf::Exception ex) {
                printf(ex.what());
                return false;
            }
            
            if (!roboto.openFromFile("res/Roboto/Roboto-Regular.ttf")) {
                return false;
            }

            return true;
        }

        void drawUI(
            sf::RenderWindow& window, 
            sf::Text winText, 
            float fps, 
            int currNumObjects,
            int currNumCollisions
        ) {
            std::string winTextString;
            // DRAW TEXT
            textCanvas.clear(sf::Color(255, 255, 255, 0));
            winText.setFillColor(sf::Color(255, 255, 255));
            char c[10];
            sprintf(c, "%.f", fps);
            winTextString = "FPS: " + std::string(c) + 
                            "\nObj Count: " + std::to_string(currNumObjects) +
                            "\nCollision Count: " + std::to_string(currNumCollisions);
            winText.setString(winTextString);
            winText.setPosition({5.f, 2.f});
            textCanvas.draw(winText);

            if (appSettings.useQuadtree) {
                winTextString = "reset view: [R]     place objects: [space] & [G]\nmove: [WASD]    show quadtree: [N]\nzoom: [ctrl +-]     clear: [F]";
                winText.setString(winTextString);
                winText.setPosition({250.f, 2.f});
                textCanvas.draw(winText);

                winTextString = "quadtree: on";
                // winTextString = "quadtree accelerated\ncollision detection";
                winText.setString(winTextString);
                winText.setFillColor(sf::Color(80, 230, 80));
                winText.setPosition({static_cast<float>(windowDim.x) - 150.f, 2.f});
                textCanvas.draw(winText);
            }
            else {
                winTextString = "reset view: [R]     place objects: [space] & [G]\nmove: [WASD]\nzoom: [ctrl +-]     clear: [F]";
                winText.setString(winTextString);
                winText.setPosition({250.f, 2.f});
                textCanvas.draw(winText);

                winTextString = "quadtree: off";
                // winTextString = "brute force\ncollision detection";
                winText.setString(winTextString);
                winText.setFillColor(sf::Color(240, 140, 80));
                winText.setPosition({static_cast<float>(windowDim.x) - 150.f, 2.f});
                textCanvas.draw(winText);
            }

            textCanvas.display();
            
            sf::Sprite uiSprite(textCanvas.getTexture());

            uiSprite.setPosition({
                camera.getCenter().x - camera.getSize().x / 2, 
                camera.getCenter().y - camera.getSize().y / 2});

            uiSprite.setScale({camera.getCurrZoom(), camera.getCurrZoom()});
            
            window.draw(uiSprite);
        }
};

}