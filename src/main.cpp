#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <string.h>
#include <random>
#include <limits>
#include <filesystem>
#include <unordered_map>
#include "VertUtil.hpp"
#include "RandomUtil.hpp"
#include "Physics.hpp"
#include "Quadtree.hpp"

// callable for quadtree
struct GetRectPhys {
    Rect<float>& operator()(physics::Object* obj) const {
        return obj->boundingBox;
    }
};

int main() {
    
    const sf::Vector2u windowDim(1100, 1200);

    int leastWinDim = (windowDim.x < windowDim.y) ? windowDim.x : windowDim.y;

    Rect<int> worldDimI(static_cast<int>(windowDim.x * 0.04545), static_cast<int>(windowDim.y * 0.125), 
                        static_cast<int>(leastWinDim - (leastWinDim * 0.0909)), 
                        static_cast<int>(leastWinDim - (leastWinDim * 0.0909)));
    //Rect<int> worldDimI(50, 150, 1000, 1000);
    Rect<float> world(  static_cast<float>(worldDimI.left), static_cast<float>(worldDimI.top),
                        static_cast<float>(worldDimI.width), static_cast<float>(worldDimI.height));

    int sceneNumObjects = 10;
    float objSizeMin = 10.f;
    float objSizeMax = 50.f;
    float objInitVelMin = 20.f;
    float objInitVelMax = 60.f;
    bool showNodes = true;
    bool useQuadtree = true; // use quadtree or use brute force
   
    std::cout << "Enter object count: ";
    std::cin >> sceneNumObjects;
    while(!std::cin) {
        std::cout << "invalid input, please enter a number\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter object count: ";
        std::cin >> sceneNumObjects;
    }

    std::cout << "Enter object size range 'min max': ";
    std::cin >> objSizeMin >> objSizeMax;
    while (!std::cin) {
        std::cout << "Invalid input, please enter two nums 'min max'\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter object size range 'min max': ";
        std::cin >> objSizeMin >> objSizeMax;
    }

    std::cout << "Enter object velocity range 'min max': ";
    std::cin >> objInitVelMin >> objInitVelMax;
    while (!std::cin) {
        std::cout << "Invalid input, please enter two nums 'min max'\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter object velocity range 'min max': ";
        std::cin >> objInitVelMin >> objInitVelMax;
    }

    std::cout << "Collision Detection: Quadtree Accelerated (1), Bruteforce (0): ";
    std::cin >> useQuadtree;
    while (!std::cin) {
        std::cout << "Invalid input, enter '1' or '0'\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Collision Deterction: Quadtree accelerated (1), Bruteforce (0): ";
        std::cin >> useQuadtree;
    }

    std::cout << "Press ENTER to start";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear cin buffer
    if (std::cin.get() == '\n') {
        std::cout << "Generating Scene...\n";
    }
    
    auto window = sf::RenderWindow{ {windowDim.x, windowDim.y}, "Quadtree Demonstration", 
                                    sf::Style::Titlebar | sf::Style::Close};

    // color pallete
    std::array<sf::Color, 12> colors = {
        sf::Color(255, 90, 51), sf::Color(255, 128, 0), sf::Color(255, 255, 51), sf::Color(153, 255, 51), 
        sf::Color(51, 255, 51), sf::Color(51, 255, 153), sf::Color(51, 255, 255), sf::Color(51, 153, 255),
        sf::Color(51, 51, 255), sf::Color(153, 51, 255), sf::Color(255, 51, 255), sf::Color(255, 51, 153)};
    
    sf::View cam(sf::FloatRect(0, 0, static_cast<float>(windowDim.x), static_cast<float>(windowDim.y)));

    window.setView(cam);

    sf::RenderTexture textCanvas;
    if (!textCanvas.create(windowDim.x, windowDim.y / 9)) {
        return EXIT_FAILURE;
    }

    const float maxZoom = 3.0f;
    const float minZoom = 0.05f;
    const float defaultZoom = 1.f;
    const float zoomAmount = 0.07f;
    float zoom = 1.f;
    float moveAmount = 200.f;

    sf::Clock clock;
    float dt;

    //Rect<float> quadTreeDim(0, 100, 1100, 1100);
    Rect<float> quadTreeDim(0, static_cast<int>(leastWinDim * 0.0909), leastWinDim, leastWinDim);
    Quadtree<physics::Object*, GetRectPhys> qt(quadTreeDim);
    
    // world boundary
    physics::Boundary worldBounds(world);
    physics::PhysicsHandler pHandler;
    std::vector<physics::Object> physObjs;
    
    // load objects
    physObjs.reserve(sceneNumObjects);
    for (int i = 0; i < sceneNumObjects; i++) {
        physics::Object thisObj(
            Vec2f(  rand() % (worldDimI.width - 300) + worldDimI.left,
                    rand() % (worldDimI.height - 300) + worldDimI.top),
            Vec2f(getRandInRange(objSizeMin, objSizeMax), getRandInRange(objSizeMin, objSizeMax)));
        
        thisObj.color = colors[rand() % colors.size()];
        thisObj.velocity = Vec2f(getRandVelocity(objInitVelMin, objInitVelMax), getRandVelocity(objInitVelMin, objInitVelMax));
        thisObj.mass = thisObj.boundingBox.width * thisObj.boundingBox.height;
        physObjs.emplace_back(thisObj);
    }
    
    std::vector<Rect<float>> nodeRects;
    std::unordered_map<physics::Object*, physics::Object*> objColMap;
    std::vector<sf::Vertex> verticies;
    sf::Font roboto;
    
    if (!roboto.loadFromFile("res/Roboto/Roboto-Regular.ttf")) {
        return EXIT_FAILURE;
    }
    
    std::string winTextString;
    sf::Text winText;
    winText.setFont(roboto);
    winText.setCharacterSize(25);
    winText.setFillColor(sf::Color::White);
    winText.setPosition(0.f, 0.f);

    int curNumNodes = 1;
    int curNumCollisions = 0;
    int curNumObjects = 0;
    int numFrames = 0;
    float fpsSum = 0;

    std::cout << "running...\n";
    while (window.isOpen()) {
        
        dt = clock.restart().asSeconds();
        float fps = 1.f/(dt);
        numFrames++;
        fpsSum += fps;

        // mouse position
        sf::Vector2i sfMousePos = sf::Mouse::getPosition(window);
        sf::Vector2f sfMouseWorldPos = window.mapPixelToCoords(sfMousePos);
        
        // compute center of view to mouse vector
        Vec2f centerToMouse(sfMouseWorldPos - cam.getCenter());
        float mag = centerToMouse.getMag();
        centerToMouse.normalize();
        centerToMouse = centerToMouse * mag * 0.25;

        sf::Event event;
        while (window.pollEvent(event)) { // barebones event handling
            if (event.type == sf::Event::Closed || 
                (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))) {
                std::cout << "avg FPS = " << fpsSum / static_cast<float>(numFrames) << "\n";
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || 
                    sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {
                    if (event.key.code == sf::Keyboard::Equal) {
                        zoom -= zoomAmount;
                        if (zoom < minZoom)
                            zoom = minZoom;
                        cam.setSize(windowDim.x * zoom, windowDim.y * zoom);
                        cam.move(centerToMouse.x, centerToMouse.y);
                    }
                    if (event.key.code == sf::Keyboard::Hyphen) {
                        zoom += zoomAmount;
                        if (zoom > maxZoom)
                            zoom = maxZoom;
                        cam.setSize(windowDim.x * zoom, windowDim.y * zoom);
                        cam.move(-centerToMouse.x, -centerToMouse.y);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                    if (world.contains(sfMouseWorldPos)) {
                        Vec2f newObjSize(getRandInRange(objSizeMin, objSizeMax), getRandInRange(objSizeMin, objSizeMax));
                        physics::Object newObj(
                            Vec2f(sfMouseWorldPos.x - (newObjSize.x / 2), sfMouseWorldPos.y - (newObjSize.y / 2)), 
                            newObjSize, newObjSize.x * newObjSize.y);
                        newObj.velocity = Vec2f(getRandVelocity(objInitVelMin, objInitVelMax), 
                                                getRandVelocity(objInitVelMin, objInitVelMax));
                        newObj.color = colors[rand() % colors.size()];
                        physObjs.push_back(newObj);
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) { // reset view
                    zoom = defaultZoom;
                    cam.setSize(windowDim.x, windowDim.y);
                    cam.setCenter(windowDim.x / 2, windowDim.y / 2);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
                    showNodes = !showNodes;
                }
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            cam.move(-moveAmount * dt, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            cam.move(0, -moveAmount * dt);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            cam.move(moveAmount * dt, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            cam.move(0, moveAmount * dt);
        }
        
        for (auto &obj : physObjs) {
            obj.update(dt); // update physics objects
        }

        for (auto &obj : physObjs) {
            worldBounds.checkCollision(obj); // resolve collisions with world boundary
        }
        
        if (useQuadtree) {
            for (int i = 0; i < physObjs.size(); i++) {
                qt.add(&physObjs[i]);
            }
            objColMap = qt.findAllIntersections();
            curNumCollisions = objColMap.size();
            curNumObjects = qt.getNumObjects();

            pHandler.resolveColsFromMap(objColMap);

            if (showNodes) {
                qt.getNodeRects(nodeRects);
                computeBoxFrameVertsFromVec(nodeRects, verticies, sf::Color(255, 255, 255, 120), 0.8f);
                nodeRects.clear();
            }
            qt.clearAll();
        }
        else {
            pHandler.BRUTE_resolveCols(physObjs);
        }
        computePhysicsObjectVertsFromVec(physObjs, verticies);

        computeBoxFrameVerts(worldBounds.boundingBox, verticies, sf::Color::Yellow, 1.f);
        objColMap.clear();
        
        
        
        

        
        // DRAW VERTICIES
        window.clear();
        window.setView(cam);
        window.draw(&verticies[0], verticies.size(), sf::Quads);
        
        verticies.clear(); // clear vector

        // DRAW TEXT
        textCanvas.clear(sf::Color(255, 255, 255, 0));
        winText.setFillColor(sf::Color(255, 255, 255));
        char c[10];
        sprintf(c, "%.f", fps);
        winTextString = "FPS: " + std::string(c);
        winText.setString(winTextString);
        winText.setPosition(5.f, 2.f);
        textCanvas.draw(winText);

        winTextString = "Obj Count: " + std::to_string(curNumObjects);
        winText.setPosition(5.f, 32.f);
        winText.setString(winTextString);
        textCanvas.draw(winText);

        winTextString = "Collision Count: " + std::to_string(curNumCollisions);
        winText.setPosition(5.f, 63.f);
        winText.setString(winTextString);
        textCanvas.draw(winText);

        if (useQuadtree) {
            winTextString = "reset view: [R]     place objects: [space]\nmove: [WASD]    show quadtree: [N]\nzoom: [ctrl +-]";
            winText.setString(winTextString);
            winText.setPosition(300.f, 2.f);
            textCanvas.draw(winText);

            winTextString = "quadtree accelerated\ncollision detection";
            winText.setString(winTextString);
            winText.setFillColor(sf::Color(80, 230, 80));
            winText.setPosition(static_cast<float>(windowDim.x) - 250.f, 2.f);
            textCanvas.draw(winText);
        }
        else {
            winTextString = "reset view: [R]     place objects: [space]\nmove: [WASD]\nzoom: [ctrl +-]";
            winText.setString(winTextString);
            winText.setPosition(300.f, 2.f);
            textCanvas.draw(winText);

            winTextString = "brute force\ncollision detection";
            winText.setString(winTextString);
            winText.setFillColor(sf::Color(240, 140, 80));
            winText.setPosition(static_cast<float>(windowDim.x) - 250.f, 2.f);
            textCanvas.draw(winText);
        }

        textCanvas.display();
        sf::Sprite uiSprite(textCanvas.getTexture());
        uiSprite.setPosition(cam.getCenter().x - cam.getSize().x / 2, cam.getCenter().y - cam.getSize().y / 2);
        uiSprite.setScale(zoom, zoom);
        
        window.draw(uiSprite);

        window.display();
    }
}
