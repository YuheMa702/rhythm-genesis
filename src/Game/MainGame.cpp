#include "MainGame.hpp"
#include "../UI/UploadUI.hpp"
#include <spdlog/spdlog.h>
#include "tinyfiledialogs.h"
#include <random>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

MainGame::MainGame(sf::RenderWindow* window) : window(window) {
    auto winSize = window->getSize();
    screenWidth = static_cast<float>(winSize.x);
    screenHeight = static_cast<float>(winSize.y);
    float colWidth = screenWidth / 5;

    line = sf::RectangleShape({screenWidth, 10.f});
    line.setFillColor(sf::Color::White);
    line.setPosition({0, 500.f});
    
    timeToLine = 3.f;
    spawnInterval = 1.0f;
    lastSpawnTime = 0.0f;

    if (!texture.loadFromFile("../src/Game/Textures/r.png")) {
        spdlog::info("Error loading texture");
    }

    // sprite.setTexture(texture);
    // sf::Vector2f origin = sprite.getOrigin();
    // sprite.setOrigin = ({origin.x + (sprite.getSize().x / 2), origin.y + (sprite.getSize().y / 2)});
    // sprite.setPosition({colWidth + (colWidth / 2), 20.f});
    // sprite.setScale({.5f, .5f});
    
    timeStamp = 0;
}

void MainGame::spawnShape(int col) {
    sf::RectangleShape newShape({screenWidth / 5, 50.f});
    newShape.setFillColor(sf::Color::Green);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distColor(0, 255);
    // std::uniform_int_distribution<int> distX(0, 4);
    float newX = col * (screenWidth / 5);

    newShape.setFillColor(sf::Color(distColor(gen), distColor(gen), distColor(gen)));
    newShape.setPosition({newX, -50.f});
    fallingShapes.push_back({newShape, clock.getElapsedTime().asSeconds()});
}

bool MainGame::parseGame(const std::string& filePath, float time){
    static std::vector<bool> spawnedNotes;
    static json gameData;
    static bool loaded = false;

    if (!loaded) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            spdlog::error("Failed to open JSON file: {}", filePath);
            return false;
        }
        file >> gameData;
        spawnedNotes.resize(gameData.size(), false);
        loaded = true;
    }
    
    for (size_t i = 0; i < gameData.size(); ++i) {
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

void MainGame::displayScore() {
    font.loadFromFile("../assets/fonts/golem-script.ttf");
    float startX = 50.f;
    float boxWidth = (screenWidth - 100.f) / scoreBreakdown.size();
    float boxHeight = 20.f;
    window->clear(sf::Color::Black);

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

    scoreText.setFont(font);
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setCharacterSize(50);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(screenWidth / 2 - scoreText.getGlobalBounds().width / 2, screenHeight / 2);

    window->draw(scoreText);

    window->display();
    sf::sleep(sf::seconds(5));
}

void MainGame::run(const std::string& filePath) {
    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window->close();
            uiManager.handleEvent(event);
        }
        
        window->clear(sf::Color::Black);

        float currentTime = clock.getElapsedTime().asSeconds();
        spawnedShape = parseGame(filePath, currentTime);

        for (auto it = fallingShapes.begin(); it != fallingShapes.end();) {
            float elapsedTime = currentTime - it->spawnTime;
            float distance = 500.f + 50.f;
            float speed = distance / timeToLine;
            
            it->shape.setPosition(it->shape.getPosition().x, -50.f + speed * elapsedTime);
            
            if (it->shape.getPosition().y > screenHeight) {
                it = fallingShapes.erase(it);
            } else {
                boundingBoxBlock = it->shape.getGlobalBounds();
                boundingBoxLine = line.getGlobalBounds();
                float pos = it->shape.getPosition().x;
                
                if (boundingBoxBlock.intersects(boundingBoxLine) && (((pos) < 100) & ((pos) > -100) & sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)
                |((pos) < 200) & ((pos) > 100) & sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T)
                |((pos) < 400) & ((pos) > 300) & sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y)
                |((pos) < 500) & ((pos) > 400) & sf::Keyboard::isKeyPressed(sf::Keyboard::Key::U)
                |((pos) < 700) & ((pos) > 600) & sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I))) {
                    float posY = it->shape.getPosition().y;
                    float lineY = line.getPosition().y + (5.f);
                    float blockCenter = posY + (it->shape.getSize().y / 2);
                    float diff = blockCenter - lineY;
                    spdlog::info(diff);
                    if(diff < 5.f && diff > -5.f){
                        spdlog::info("excellent");
                        score += 10;
                        scoreBreakdown.push_back("excellent");
                    }else if((diff <= 15.f && diff >= 5.f) || (diff >= -15.f && diff <= -5.f)){
                        spdlog::info("good");
                        score += 5;
                        scoreBreakdown.push_back("good");
                    }else if((diff <= 45.f && diff > 15.f) || (diff >= -45.f && diff < -15.f)){
                        spdlog::info("bad");
                        score += 1;
                        scoreBreakdown.push_back("bad");
                    }
                    it = fallingShapes.erase(it);
                } else {
                    window->draw(it->shape);
                    ++it;
                }
            }
        }

        if (fallingShapes.empty() && gameOver) {
            displayScore();
            break;
        }
        
        window->draw(line);
        // window->draw(sprite);
        window->display();
    }
}
