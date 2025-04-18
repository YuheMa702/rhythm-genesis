#ifndef MAIN_GAME_H
#define MAIN_GAME_H

#include <SFML/Graphics.hpp>
#include "../UI/UIManager.hpp"
#include <vector>
#include <string>
#include <SFML/Audio.hpp>

struct FallingShape {
    sf::RectangleShape shape;
    sf::Text keyLabel;
    float spawnTime;
    bool fading = false;
    float fadeTimer = 0.f;
};

class MainGame {
private:
    UIManager uiManager;
    sf::RenderWindow* window;
    sf::RectangleShape shape;
    sf::RectangleShape line;
    sf::RectangleShape line1;
    sf::RectangleShape line2;
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
    std::vector<float> totalScoreGraph;
    float scoreTimer;

    sf::Music backgroundMusic;

    float firstBlockTime = 500.f; 

    const float TOP_LINE = 475.f;
    const float TIME_TO_LINE = 3.f;

public:
    MainGame(sf::RenderWindow* window);
    void run(const std::string& filePath, const std::string& musicPath);
    void spawnShape(int col);
    bool parseGame(const std::string& filePath, float time);
    void displayScore();
    void playMusic(const std::string& musicPath);
    bool menuRunning;
    bool gameOver = false;
    bool loaded = false;
    sf::Clock clock;
};

#endif
