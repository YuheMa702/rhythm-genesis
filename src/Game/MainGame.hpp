#ifndef MAIN_GAME_H
#define MAIN_GAME_H

#include <SFML/Graphics.hpp>
#include "../UI/UIManager.hpp"
#include <vector>
#include <string>

struct FallingShape {
    sf::RectangleShape shape;
    sf::Text keyLabel;
    float spawnTime;
};

class MainGame {
private:
    UIManager uiManager;
    sf::RenderWindow* window;
    sf::RectangleShape shape;
    sf::RectangleShape line;
    sf::FloatRect boundingBoxBlock;
    sf::FloatRect boundingBoxLine;

    float screenWidth, screenHeight;
    bool shapeAlive;
    float timeToLine;
    float speed;
    float distance;
    float start;
    float spawnInterval;
    float lastSpawnTime;

    std::vector<FallingShape> fallingShapes;
    sf::Clock clock;
    sf::Texture texture;
    sf::Sprite sprite;

    int timeStamp;
    bool spawnedShape;
    sf::Text keyLabelHolder;
    std::vector<char> keys = {'R', 'T', 'Y', 'U', 'I'};
    std::vector<bool> pressed = {false, false, false, false, false};

    int score = 0;
    std::vector<std::string> scoreBreakdown;
    sf::Font font; 
    sf::Text scoreText;
    bool gameOver = false;
    std::vector<float> totalScoreGraph;
    float scoreTimer;

public:
    MainGame(sf::RenderWindow* window);
    void run(const std::string& filePath);
    void spawnShape(int col);
    bool parseGame(const std::string& filePath, float time);
    void displayScore();
};

#endif
