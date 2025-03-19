#ifndef MAIN_GAME_H
#define MAIN_GAME_H

#include <SFML/Graphics.hpp>
#include "../UI/UIManager.hpp"
#include <vector>
#include <string>

class MainGame {
private:
    UIManager uiManager;
    sf::RenderWindow* window;
    sf::RectangleShape shape;
    sf::RectangleShape line;
    sf::FloatRect boundingBoxBlock;
    sf::FloatRect boundingBoxLine;
    float screenWidth;
    float screenHeight;
    bool shapeAlive;
    float timeToLine;
    float speed;
    float distance;
    float start;
    sf::Clock clock;

public:
    MainGame(sf::RenderWindow* window);
    void run();
};

#endif
