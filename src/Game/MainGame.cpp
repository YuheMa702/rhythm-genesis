#include "MainGame.hpp"
#include "../UI/UploadUI.hpp"
#include <spdlog/spdlog.h>
#include "tinyfiledialogs.h"
#include <random>
#include <fstream>

// Set stuff up for game
MainGame::MainGame(sf::RenderWindow* window) : window(window) {
    auto winSize = window->getSize();
    screenWidth = static_cast<float>(winSize.x);
    screenHeight = static_cast<float>(winSize.y);
    float colWidth = screenWidth / 5;

    font.loadFromFile("../assets/fonts/golem-script.ttf");
    font1.loadFromFile("../assets/fonts/arial.ttf");

    line = sf::RectangleShape({screenWidth, 5.f});
    line.setFillColor(sf::Color::Black);
    line.setPosition({0, TOP_LINE + 25.f});

    line1 = sf::RectangleShape({screenWidth, 5.f});
    line1.setFillColor(sf::Color::White);
    line1.setPosition({0, TOP_LINE + 50.f});

    line2 = sf::RectangleShape({screenWidth, 5.f});
    line2.setFillColor(sf::Color::White);
    line2.setPosition({0, TOP_LINE});

    pauseText.setFont(font);
    pauseText.setString("PAUSED\nPress ESC to Resume");
    pauseText.setCharacterSize(40);
    pauseText.setFillColor(sf::Color::White);
    pauseText.setPosition(screenWidth / 2.f - 150.f, screenHeight / 2.f - 50.f);

    returnToMenuText.setFont(font);
    returnToMenuText.setString("Return to Menu (M)");
    returnToMenuText.setCharacterSize(30);
    returnToMenuText.setFillColor(sf::Color::White);
    returnToMenuText.setPosition(screenWidth / 2.f - returnToMenuText.getGlobalBounds().width / 2.f, screenHeight / 2.f + 50.f);

    laneWidth = screenWidth / 5.f;
    timeToLine = TIME_TO_LINE;
    spawnInterval = 1.0f;
    lastSpawnTime = 0.0f;
    timeStamp = 0;
}

bool MainGame::parseGame(const std::string& filePath, float time) {
    if (!loaded) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            spdlog::error("Failed to open JSON file: {}", filePath);
            return false;
        }
        gameData.clear();
        file >> gameData;
        spawnedNotes.clear();
        spawnedNotes.resize(gameData.size(), false);
        loaded = true;
        firstBlockTime = gameData[0]["time"].get<float>();
    }

    for (size_t i = 0; i < gameData.size(); ++i) {
        if (!spawnedNotes[i] && time >= gameData[i]["time"].get<float>()) {
            for (int lane : gameData[i]["lanes"]) {
                spawnShape(lane, time);
            }
            spawnedNotes[i] = true;
            lastRowTime = time;
            gameOver = true;
            return true;
        }
    }
    return false;
}

void MainGame::spawnShape(int col, float time) {
    sf::RectangleShape newShape({screenWidth / 5, 50.f});
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distColor(0, 9);
    float newX = col * (screenWidth / 5);
    newShape.setFillColor(tileColors[distColor(gen)]);
    newShape.setPosition({newX, -50.f});

    keyLabelHolder.setFont(font);
    keyLabelHolder.setString(std::string(1, keys[col]));
    keyLabelHolder.setCharacterSize(24);
    keyLabelHolder.setFillColor(sf::Color::White);
    keyLabelHolder.setPosition(newX + (screenWidth / 10) - 10, -40.f);

    fallingShapes.push_back({newShape, keyLabelHolder, time});
}

void MainGame::displayScore() {
    backgroundMusic.stop();
    displayScoreLoop();
    // Reset score state to avoid bar stretching on next game
    scoreBreakdown.clear();
    totalScoreGraph.clear();
}


void MainGame::displayScoreLoop() {
    float displayStartTime = clock.getElapsedTime().asSeconds() - totalPausedTime;

    if (scoreBreakdown.empty() || totalScoreGraph.empty()) return;
    float stableScoreBarWidth = ((screenWidth - 100.f) / scoreBreakdown.size()) - 5.f;
    float stableGraphBarWidth = (screenWidth - 100.f) / totalScoreGraph.size();

    while (window->isOpen()) {
        float currentTime = clock.getElapsedTime().asSeconds() - totalPausedTime;
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) return;
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::M) return;
        }
        if (currentTime - displayStartTime >= 5.f) return;

        window->clear(sf::Color::Black);

        float startX = 50.f;
        float boxHeight = 20.f;
        for (const auto& hit : scoreBreakdown) {
            sf::RectangleShape hitBox(sf::Vector2f(stableScoreBarWidth, boxHeight));
            hitBox.setFillColor(hit == "excellent" ? sf::Color::Green : (hit == "good" ? sf::Color::Yellow : sf::Color::Red));
            hitBox.setPosition(startX, 50.f);
            window->draw(hitBox);
            startX += stableScoreBarWidth + 5.f;
        }

        sf::RectangleShape graphBackground(sf::Vector2f(screenWidth - 100.f, 150.f));
        graphBackground.setFillColor(sf::Color(30, 30, 30));
        graphBackground.setPosition(50.f, 350.f);
        window->draw(graphBackground);

        float maxScore = *std::max_element(totalScoreGraph.begin(), totalScoreGraph.end());
        float graphHeight = 150.f;
        float graphBottomY = 500.f;

        sf::Vertex yAxis[] = {
            sf::Vertex(sf::Vector2f(50.f, 350.f)),
            sf::Vertex(sf::Vector2f(50.f, 500.f))
        };
        window->draw(yAxis, 2, sf::Lines);

        std::vector<std::pair<float, std::string>> yLabels = {
            {500.f, "0"}, {(425.f), "50"}, {350.f, "100"}
        };

        for (const auto& [y, text] : yLabels) {
            sf::Text label;
            label.setFont(font);
            label.setString(text);
            label.setCharacterSize(14);
            label.setFillColor(sf::Color::White);
            label.setPosition(10.f, y - 10.f);
            window->draw(label);
        }

        startX = 50.f;
        int timeMarker = 0;

        for (size_t i = 0; i < totalScoreGraph.size(); ++i) {
            float normalized = totalScoreGraph[i] / maxScore;
            float barHeight = normalized * graphHeight;
            sf::RectangleShape bar(sf::Vector2f(stableGraphBarWidth, barHeight));
            bar.setFillColor(sf::Color::Green);
            bar.setPosition(startX, graphBottomY - barHeight);
            window->draw(bar);
            if (i % 10 == 0) {
                sf::Text timeLabel;
                timeLabel.setFont(font);
                timeLabel.setString(std::to_string(timeMarker) + "s");
                timeLabel.setCharacterSize(12);
                timeLabel.setFillColor(sf::Color::White);
                timeLabel.setPosition(startX, 505.f);
                window->draw(timeLabel);
                timeMarker += 2;
            }
            startX += stableGraphBarWidth;
        }

        scoreText.setFont(font);
        scoreText.setString("Score: " + std::to_string(score));
        scoreText.setCharacterSize(50);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(screenWidth / 2 - scoreText.getGlobalBounds().width / 2, screenHeight / 3);
        window->draw(scoreText);

        window->draw(returnToMenuText);
        window->display();
    }
}



void MainGame::playMusic(const std::string& musicPath) {
    if (!backgroundMusic.openFromFile(musicPath)) {
        spdlog::error("music not work");
    } else {
        backgroundMusic.setLoop(true);
        backgroundMusic.setVolume(50);
        backgroundMusic.play();
    }
}

void MainGame::reset() {
    fallingShapes.clear();
    scoreBreakdown.clear();
    totalScoreGraph.clear();
    score = 0;
    scoreTimer = 0.f;
    gameOver = false;
    loaded = false;
    spawnedShape = false;
    totalPausedTime = 0;
    isPaused = false;
    musicStarted = false;
    musicEnded = false;
    musicEndTime = -1.0f;
    lastRowTime = -1.0f;
    firstBlockTime = 0.f;
    clock.restart();
    backgroundMusic.stop();
    pressed = {false, false, false, false, false};
}




void MainGame::run(const std::string& filePath, const std::string& musicPath) {
    while (window->isOpen()) {
        if ((timeToLine - firstBlockTime - 0.1f <= clock.getElapsedTime().asSeconds()) && !musicStarted) {
            playMusic(musicPath);
            musicStarted = true;
        }

        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window->close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape && !musicEnded) {
                    isPaused = !isPaused;
                    if (isPaused) {
                        backgroundMusic.pause();
                        pauseStartTime = clock.getElapsedTime().asSeconds();
                    } else {
                        totalPausedTime += clock.getElapsedTime().asSeconds() - pauseStartTime;
                        float gameTime = clock.getElapsedTime().asSeconds() - totalPausedTime - (timeToLine - firstBlockTime - 0.1f);
                        backgroundMusic.setPlayingOffset(sf::seconds(gameTime));
                        backgroundMusic.play();
                    }
                }
                if ((isPaused || musicEnded) && event.key.code == sf::Keyboard::M) {
                    backgroundMusic.stop();
                    isPaused = false;
                    return;
                }
            }
            uiManager.handleEvent(event);
        }

        float currentTime = clock.getElapsedTime().asSeconds() - totalPausedTime;

        // Check if music has ended
        if (backgroundMusic.getStatus() == sf::Music::Stopped && musicStarted && !musicEnded) {
            musicEnded = true;
            musicEndTime = currentTime;
        }

        // Auto-return to menu 60 seconds after music ends
        if (musicEnded && currentTime - musicEndTime >= 60.0f) {
            backgroundMusic.stop();
            return;
        }

        window->clear(sf::Color::Black);

        if (isPaused) {
            window->draw(pauseText);
            window->draw(returnToMenuText);
            window->display();
            continue;
        }

        scoreTimer += currentTime;
        if (scoreTimer >= 10.f && score > 0) {
            scoreTimer = 0.f;
            totalScoreGraph.push_back(static_cast<float>(score));
        }

        spawnedShape = parseGame(filePath, currentTime);

        for (auto it = fallingShapes.begin(); it != fallingShapes.end();) {
            float elapsedTime = currentTime - it->spawnTime;
            float distance = TOP_LINE;
            float speed = distance / timeToLine;

            if (it->fading) {
                float fadeDuration = 0.33f;
                float timeSinceFade = currentTime - it->fadeTimer;
                float alphaFactor = 1.f - (timeSinceFade / fadeDuration);
                int alpha = static_cast<int>(255 * alphaFactor);
                if (alpha <= 0) {
                    it = fallingShapes.erase(it);
                    continue;
                } else {
                    sf::Color shapeColor = it->shape.getFillColor();
                    sf::Color labelColor = it->keyLabel.getFillColor();
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
                ++it;
                continue;
            }

            it->shape.setPosition(it->shape.getPosition().x, -50.f + speed * elapsedTime);
            it->keyLabel.setPosition(it->keyLabel.getPosition().x, -40.f + speed * elapsedTime);

            if (it->shape.getPosition().y > screenHeight) {
                it = fallingShapes.erase(it);
            } else {
                boundingBoxBlock = it->shape.getGlobalBounds();
                boundingBoxLine = line.getGlobalBounds();
                int laneIndex = static_cast<int>(it->shape.getPosition().x / laneWidth);
                sf::Keyboard::Key expectedKey = keyBindings[laneIndex];

                if (boundingBoxBlock.intersects(boundingBoxLine) && sf::Keyboard::isKeyPressed(expectedKey)) {
                    float posY = it->shape.getPosition().y;
                    float lineY = line.getPosition().y + 5.f;
                    float blockCenter = posY + (it->shape.getSize().y / 2);
                    float diff = blockCenter - lineY;

                    if (std::abs(diff) < 5.f) {
                        score += 10;
                        it->shape.setFillColor(sf::Color::Green);
                        scoreBreakdown.push_back("excellent");
                    } else if (std::abs(diff) <= 15.f) {
                        score += 5;
                        it->shape.setFillColor(sf::Color::Yellow);
                        scoreBreakdown.push_back("good");
                    } else if (std::abs(diff) <= 45.f) {
                        score += 1;
                        it->shape.setFillColor(sf::Color::Red);
                        scoreBreakdown.push_back("bad");
                    }

                    it->fading = true;
                    it->fadeTimer = currentTime;
                } else {
                    window->draw(it->shape);
                    window->draw(it->keyLabel);
                    ++it;
                }
            }
        }

        if (fallingShapes.empty() && gameOver) {
            displayScore();
            // reset();  // Clean slate for next game
            return;
        }

        scoreText.setFont(font);
        scoreText.setString("Score: " + std::to_string(score));
        scoreText.setCharacterSize(30);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(20.f, 20.f);
        window->draw(scoreText);
        window->draw(line1);
        window->draw(line2);

        if (musicEnded) {
            window->draw(returnToMenuText);
        }

        window->display();
    }
}
