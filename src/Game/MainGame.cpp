#include "MainGame.hpp"
#include "../UI/UploadUI.hpp"
#include <spdlog/spdlog.h>
#include "tinyfiledialogs.h"

MainGame::MainGame(sf::RenderWindow* window) : window(window) {
    auto winSize = window->getSize();
    screenWidth = static_cast<float>(winSize.x);
    screenHeight = static_cast<float>(winSize.y);

    //may be easier in future to make the blocks and line similar to UI elements
    shape = sf::RectangleShape({120.f, 50.f});
    shape.setFillColor(sf::Color::Green);
    shape.setPosition({50.f, 50.f});
    shapeAlive = true;
    start = -50.f;

    line = sf::RectangleShape({screenWidth, 10.f});
    line.setFillColor(sf::Color::White);
    line.setPosition({0, 500.f});

    distance = 500.f - start;
    timeToLine = 5.f;
    speed = distance / timeToLine;
}

void MainGame::run() {
    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window->close();
            uiManager.handleEvent(event);
        }
        window->clear(sf::Color::Black);

        //calculate/set position of falling block based on time
        float tim = clock.getElapsedTime().asSeconds();
        if(shapeAlive) shape.setPosition({shape.getPosition().x, start + speed * tim});

        //press t key while block is colliding with line to erase (for now) block 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T)){
            boundingBoxBlock = shape.getGlobalBounds();
            boundingBoxLine = line.getGlobalBounds();
            if (boundingBoxBlock.intersects(boundingBoxLine)){
                shapeAlive = false;
            }
                
        }

        if(shapeAlive)
            window->draw(shape);

        window->draw(line);

        window->display();
    }
}
