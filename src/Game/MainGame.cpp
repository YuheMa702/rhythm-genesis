#include "MainGame.hpp"
#include "../UI/UploadUI.hpp"
#include <spdlog/spdlog.h>
#include "tinyfiledialogs.h"
#include <random>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

//set stuff up for game
MainGame::MainGame(sf::RenderWindow* window) : window(window) {
    auto winSize = window->getSize();
    screenWidth = static_cast<float>(winSize.x);
    screenHeight = static_cast<float>(winSize.y);
    float colWidth = screenWidth / 5;

    font.loadFromFile("../assets/fonts/golem-script.ttf");

    line = sf::RectangleShape({screenWidth, 5.f});
    line.setFillColor(sf::Color::Black);
    line.setPosition({0, TOP_LINE + 25.f});

    //create the timing lines at bottom of screen
    line1 = sf::RectangleShape({screenWidth, 5.f});
    line1.setFillColor(sf::Color::White);
    line1.setPosition({0, TOP_LINE + 50.f});

    line2 = sf::RectangleShape({screenWidth, 5.f});
    line2.setFillColor(sf::Color::White);
    line2.setPosition({0, TOP_LINE});
    
    timeToLine = TIME_TO_LINE;
    //dont think i need these anymore?
    spawnInterval = 1.0f;
    lastSpawnTime = 0.0f;

    
    //might not need this
    timeStamp = 0;
}

//read from JSON file, create falling notes at correct time stamps
bool MainGame::parseGame(const std::string& filePath, float time){
    static std::vector<bool> spawnedNotes;
    static json gameData;

    if (!loaded) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            spdlog::error("Failed to open JSON file: {}", filePath);
            return false;
        }
        file >> gameData;
        spawnedNotes.resize(gameData.size(), false);
        loaded = true;
        firstBlockTime = gameData[0]["time"].get<float>();
    }
    
    for (size_t i = 0; i < gameData.size(); i++) {
        if (!spawnedNotes[i] && time >= gameData[i]["time"].get<float>()) {
            for (int lane : gameData[i]["lanes"]) {
                spawnShape(lane);
                gameOver = true;
            }
            spawnedNotes[i] = true;
            return true;
        }
    }
    return false;
}   

//create a falling block at time/location set by JSON parser
void MainGame::spawnShape(int col) {
    sf::RectangleShape newShape({screenWidth / 5, 50.f});
    newShape.setFillColor(sf::Color::Green);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distColor(0, 255);
    float newX = col * (screenWidth / 5);

    newShape.setFillColor(sf::Color(distColor(gen), distColor(gen), distColor(gen)));
    newShape.setPosition({newX, -50.f});

    keyLabelHolder.setFont(font);
    keyLabelHolder.setString(std::string(1, keys[col]));
    keyLabelHolder.setCharacterSize(24);
    keyLabelHolder.setFillColor(sf::Color::White);
    keyLabelHolder.setPosition(newX + (screenWidth / 10) - 10, -40.f);

    fallingShapes.push_back({newShape, keyLabelHolder, clock.getElapsedTime().asSeconds()});
}

//displays the score once game finishes, then returns to main menu
void MainGame::displayScore() {
    backgroundMusic.stop();
    float startX = 50.f;
    float boxWidth = ((screenWidth - 100.f) / scoreBreakdown.size()) - 5.f;
    float boxHeight = 20.f;
    window->clear(sf::Color::Black);

    //displays a line at the top with a colored box for each individual hit;
    //red = bad, yellow = good, green = excellent
    for (const auto& hit : scoreBreakdown) {
        sf::RectangleShape hitBox(sf::Vector2f(boxWidth, boxHeight));
        if (hit == "excellent") {
            hitBox.setFillColor(sf::Color::Green);
        } else if (hit == "good") {
            hitBox.setFillColor(sf::Color::Yellow);
        } else {
            hitBox.setFillColor(sf::Color::Red);
        }
        hitBox.setPosition(startX, 50.f);
        window->draw(hitBox);
        startX += boxWidth + 5.f;
    }

    //displays a graph that shows score over time
    startX = 50.f;
    boxWidth = (screenWidth - 100.f) / totalScoreGraph.size();
    for (const auto& tot : totalScoreGraph) {
        boxHeight = tot;
        sf::RectangleShape totBox(sf::Vector2f(boxWidth, boxHeight));
        totBox.setFillColor(sf::Color::Green);
        totBox.setPosition(startX, 500.f - boxHeight);
        window->draw(totBox);
        startX += boxWidth;
    }

    //display actual score
    scoreText.setFont(font);
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setCharacterSize(50);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(screenWidth / 2 - scoreText.getGlobalBounds().width / 2, screenHeight / 2);

    window->draw(scoreText);

    window->display();
    sf::sleep(sf::seconds(4));
}

void MainGame::playMusic(const std::string& musicPath){
    if (!backgroundMusic.openFromFile(musicPath)) {
        spdlog::error("music not work");
    } else {
        backgroundMusic.setLoop(true);
        backgroundMusic.setVolume(50);
        backgroundMusic.play();
    }
}

//this is a very ugly function that I'm too lazy to fix as long as there aren't
//any bugs
void MainGame::run(const std::string& filePath, const std::string& musicPath) {
    while (window->isOpen()) {
        if (timeToLine - firstBlockTime >= clock.getElapsedTime().asSeconds()){
            playMusic(musicPath);
        }
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window->close();
            uiManager.handleEvent(event);
        }
        
        window->clear(sf::Color::Black);

        float currentTime = clock.getElapsedTime().asSeconds();

        //track score for displays at the end
        scoreTimer += currentTime;
        if (scoreTimer >= 10.f && score > 0){
            scoreTimer = 0.f;
            totalScoreGraph.push_back((float)score);
        }
        spawnedShape = parseGame(filePath, currentTime);

        for (auto it = fallingShapes.begin(); it != fallingShapes.end();) {
            float elapsedTime = currentTime - it->spawnTime;
            float distance = TOP_LINE;
            float speed = distance / timeToLine;

            //draw blocks and handle the fade out animations
            if (it->fading == true) {
                float fadeDuration = 0.33f;
                float timeSinceFade = currentTime - it->fadeTimer;
                float alphaFactor = 1.f - (timeSinceFade / fadeDuration);

                sf::Color shapeColor = it->shape.getFillColor();
                sf::Color labelColor = it->keyLabel.getFillColor();
            
                int alpha = static_cast<int>(255 * (alphaFactor));
            
                if (alpha <= 0) {
                    it = fallingShapes.erase(it);
                    continue;
                } else {
                    shapeColor.a = alpha;
                    labelColor.a = alpha;
                    it->shape.setFillColor(shapeColor);
                    it->keyLabel.setFillColor(labelColor);

                    float scaleFactor = 1.f + 0.2f * (1.f - alphaFactor);
                    it->shape.setScale(scaleFactor, scaleFactor);
                    it->keyLabel.setScale(scaleFactor, scaleFactor);
                }
                window->draw(it->shape);
                window->draw(it->keyLabel);
                it++;
                continue;
            } 
            
            //track block positions
            it->shape.setPosition(it->shape.getPosition().x, -50.f + speed * elapsedTime);
            it->keyLabel.setPosition(it->keyLabel.getPosition().x, -40.f + speed * elapsedTime);

            if (it->shape.getPosition().y > screenHeight) {
                it = fallingShapes.erase(it);
            } else {
                boundingBoxBlock = it->shape.getGlobalBounds();
                boundingBoxLine = line.getGlobalBounds();
                float pos = it->shape.getPosition().x;
                
                //handle score tracking and deleting of blocks on key presses
                if (boundingBoxBlock.intersects(boundingBoxLine) && (((pos) < 100) & ((pos) > -100) & sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)
                |((pos) < 200) & ((pos) > 100) & sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T)
                |((pos) < 400) & ((pos) > 300) & sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y)
                |((pos) < 500) & ((pos) > 400) & sf::Keyboard::isKeyPressed(sf::Keyboard::Key::U)
                |((pos) < 700) & ((pos) > 600) & sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I))) {
                    float posY = it->shape.getPosition().y;
                    float lineY = line.getPosition().y + (5.f);
                    float blockCenter = posY + (it->shape.getSize().y / 2);
                    float diff = blockCenter - lineY;
                    if(diff < 5.f && diff > -5.f){
                        spdlog::info("excellent");
                        score += 10;
                        it->shape.setFillColor(sf::Color::Green);
                        scoreBreakdown.push_back("excellent");
                    }else if((diff <= 15.f && diff >= 5.f) || (diff >= -15.f && diff <= -5.f)){
                        spdlog::info("good");
                        score += 5;
                        it->shape.setFillColor(sf::Color::Yellow);
                        scoreBreakdown.push_back("good");
                    }else if((diff <= 45.f && diff > 15.f) || (diff >= -45.f && diff < -15.f)){
                        spdlog::info("bad");
                        score += 1;
                        it->shape.setFillColor(sf::Color::Red);
                        scoreBreakdown.push_back("bad");
                    }
                    // it = fallingShapes.erase(it);
                    it->fading = true;
                    it->fadeTimer = clock.getElapsedTime().asSeconds();
                } else {
                    window->draw(it->shape);
                    window->draw(it->keyLabel);
                    it++;
                }
            }
        }

        if (fallingShapes.empty() && gameOver) {
            displayScore();
            break;
        }
        
        scoreText.setFont(font);
        scoreText.setString("Score: " + std::to_string(score));
        scoreText.setCharacterSize(30);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(20.f, 20.f);
        window->draw(scoreText);
        window->draw(line1);
        window->draw(line2);
        window->display();
    }
}

