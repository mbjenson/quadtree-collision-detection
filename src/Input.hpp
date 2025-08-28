#include <SFML/Graphics.hpp>

namespace input {

enum InputEvent {
    up,
    down,
    left,
    right,
    leftCtrl,
    equals,
    hyphen,
    close,
    n,
    r,
    w,
    a,
    s,
    d,
    space,
    count,
};

class InputHandler {

public:

    InputHandler() {}

    void GetInput(sf::Window& window) {

        for (int& i : inputBits) {
            i = 0;
        }

        while (const std::optional event = window.pollEvent()) {

            if (event->is<sf::Event::Closed>()) {
                inputBits[InputEvent::close] = 1;
            }
            if (event->is<sf::Event::KeyPressed>()) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
                    inputBits[InputEvent::leftCtrl] = 1;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Equal)) {
                    inputBits[InputEvent::equals] = 1;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) ||
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
                    inputBits[InputEvent::up] = 1;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) ||
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
                    inputBits[InputEvent::down] = 1;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) ||
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
                    inputBits[InputEvent::left] = 1;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) ||
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
                    inputBits[InputEvent::right] = 1;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Hyphen)) {
                    inputBits[InputEvent::hyphen] = 1;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N)) {
                    inputBits[InputEvent::n] = 1;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
                    inputBits[InputEvent::space] = 1;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
                    inputBits[InputEvent::r] = 1;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q) ||
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
                    inputBits[InputEvent::close] = 1;
                }
            }
        }
    }

    int getEvent(InputEvent event) {
        return inputBits[event];
    }

private:

    int inputBits[InputEvent::count];
 
/*

getInput()

gets the input using sf functions and stores them in a bitstring, then we use
num to access weather or not it is true
*/

};
}