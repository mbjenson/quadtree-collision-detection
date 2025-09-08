#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <string.h>
#include <random>
#include <limits>
#include <filesystem>
#include <unordered_map>
#include "App.hpp"

app::AppSettings getAppSettings() {
    app::AppSettings appSettings;

    std::cout << "Enter object count: ";
    std::cin >> appSettings.sceneNumObjects;
    while(!std::cin) {
        std::cout << "invalid input, please enter a number\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter object count: ";
        std::cin >> appSettings.sceneNumObjects;
    }
    
    std::cout << "Enter object size range 'min max': ";
    std::cin >> appSettings.objectSizeMin >> appSettings.objectSizeMax;
    while (!std::cin) {
        std::cout << "Invalid input, please enter two nums 'min max'\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter object size range 'min max': ";
        std::cin >> appSettings.objectSizeMin >> appSettings.objectSizeMax;
    }

    std::cout << "Enter object velocity range 'min max': ";
    std::cin >> appSettings.objectVelMin >> appSettings.objectVelMax;
    while (!std::cin) {
        std::cout << "Invalid input, please enter two nums 'min max'\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter object velocity range 'min max': ";
        std::cin >> appSettings.objectVelMin >> appSettings.objectVelMax;
    }

    std::cout << "Collision Detection: Quadtree Accelerated (1), Bruteforce (0): ";
    std::cin >> appSettings.useQuadtree;
    while (!std::cin) {
        std::cout << "Invalid input, enter '1' or '0'\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Collision Deterction: Quadtree accelerated (1), Bruteforce (0): ";
        std::cin >> appSettings.useQuadtree;
    }

    std::cout << "Press ENTER to start";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear cin buffer
    if (std::cin.get() == '\n') {
        std::cout << "Generating Scene...\n";
    }

    return appSettings;
}

int main() {
    app::AppSettings appSettings = getAppSettings();
    app::App app = app::App(appSettings);
    app.Run();
}