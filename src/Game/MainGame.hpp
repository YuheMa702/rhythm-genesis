#ifndef MAIN_GAME_H
#define MAIN_GAME_H

#include <SFML/Graphics.hpp>
#include "../UI/UIManager.hpp"
#include <vector>
#include <string>
#include <SFML/Audio.hpp>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

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

    std::vector<sf::Keyboard::Key> keyBindings = {
        sf::Keyboard::R,
        sf::Keyboard::T,
        sf::Keyboard::Y,
        sf::Keyboard::U,
        sf::Keyboard::I
    };

    int score = 0;
    std::vector<std::string> scoreBreakdown;
    sf::Font font; 
    sf::Font font1; 
    sf::Text scoreText;
    std::vector<float> totalScoreGraph;
    float scoreTimer;

    sf::Music backgroundMusic;

    float firstBlockTime = 500.f; 

    const float TOP_LINE = 475.f;
    const float TIME_TO_LINE = 3.5f;
    const float DELAY_CONST = .1f

    float lastRowTime = -1.0f;
    const float rowCooldown = 1.f;
    const size_t maxNotesPerRow = 2;
    const size_t maxOnScreenNotes = 10;

    json gameData;
    std::vector<bool> spawnedNotes;

    bool isPaused = false;
    sf::Text pauseText;
    sf::Text returnToMenuText;

    float pauseStartTime = 0.f;
    float totalPausedTime = 0.f;

    float laneWidth;

    std::vector<sf::Color> tileColors = {
        sf::Color(249, 65, 68),
        sf::Color(243, 114, 44),
        sf::Color(248, 150, 30),
        sf::Color(249, 132, 74),
        sf::Color(249, 199, 79),
        sf::Color(144, 190, 109),
        sf::Color(67, 170, 139),
        sf::Color(77, 144, 142),
        sf::Color(87, 117, 144),
        sf::Color(39, 125, 161)
    };

    bool musicStarted;
    

public:
    MainGame(sf::RenderWindow* window);
    void run(const std::string& filePath, const std::string& musicPath);
    void spawnShape(int col, float time);
    bool parseGame(const std::string& filePath, float time);
    void displayScore();
    void playMusic(const std::string& musicPath);
    bool menuRunning;
    bool gameOver = false;
    bool loaded = false;
    sf::Clock clock;
    void reset();
};

#endif
