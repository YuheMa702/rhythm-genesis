#include "MainGame.hpp"
#include "../UI/UploadUI.hpp"
#include <spdlog/spdlog.h>
#include "tinyfiledialogs.h"
#include <random>
#include <fstream>


//set stuff up for game
MainGame::MainGame(sf::RenderWindow* window) : window(window) {
    auto winSize = window->getSize();
    screenWidth = static_cast<float>(winSize.x);
    screenHeight = static_cast<float>(winSize.y);
    float colWidth = screenWidth / 5;

    font.loadFromFile("../assets/fonts/golem-script.ttf");
    font1.loadFromFile("../../../../Windows/Fonts/arial.ttf"); //add font to proj folder

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
bool MainGame::parseGame(const std::string& filePath, float time) {
    // static std::vector<bool> spawnedNotes;
    // static json gameData;

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
            if(gameData[i]["num_notes"].get<int>() > 3 || filePath == "../src/Game/gameTest.json"){ //fix this
                if (lastRowTime < 0 || (time - lastRowTime >= rowCooldown)) {
                    if (fallingShapes.size() >= maxOnScreenNotes) {
                        return false;
                    }

                    size_t notesSpawned = 0;
                    for (int lane : gameData[i]["lanes"]) {
                        if (notesSpawned >= maxNotesPerRow) break;
                        spawnShape(lane);
                        notesSpawned++;
                    }

                    spawnedNotes[i] = true;
                    lastRowTime = time;
                    gameOver = true;
                    return true;
                }
            }
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
    sf::RectangleShape graphBackground(sf::Vector2f(screenWidth - 100.f, 150.f));
    graphBackground.setFillColor(sf::Color(30, 30, 30));
    graphBackground.setPosition(50.f, 350.f);
    window->draw(graphBackground);
    float maxScore = totalScoreGraph.empty() ? 1.f : *std::max_element(totalScoreGraph.begin(), totalScoreGraph.end());
    float graphHeight = 150.f;
    float graphBottomY = 500.f;
    float graphTopY = graphBottomY - graphHeight;

    sf::Vertex yAxis[] = {
        sf::Vertex(sf::Vector2f(50.f, 350.f)),
        sf::Vertex(sf::Vector2f(50.f, 500.f))
    };
    window->draw(yAxis, 2, sf::Lines);

    std::vector<std::pair<float, std::string>> yLabels = {
        {graphBottomY, "0"},
        {(graphTopY + graphBottomY) / 2, "50"},
        {graphTopY, "100"}
    };

    for (const auto& [y, text] : yLabels) {
        sf::Text label;
        label.setFont(font);
        label.setString(text);
        label.setCharacterSize(14);
        label.setFillColor(sf::Color::White);
        label.setPosition(10.f, y - 10.f);
        window->draw(label);
        sf::Text percent;
        percent.setFont(font1);
        percent.setString("%");
        percent.setCharacterSize(14);
        percent.setFillColor(sf::Color::White);
        percent.setPosition(label.getPosition().x + label.getLocalBounds().width + 2.f, y - 10.f);
        window->draw(percent);
    }

    // graph bars + timestamps
    startX = 50.f;
    boxWidth = (screenWidth - 100.f) / totalScoreGraph.size();
    int timeMarker = 0;

    for (size_t i = 0; i < totalScoreGraph.size(); ++i) {
        float normalized = totalScoreGraph[i] / maxScore;
        float barHeight = normalized * graphHeight;
        sf::RectangleShape bar(sf::Vector2f(boxWidth, barHeight));
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

        startX += boxWidth;
    }

    //display actual score
    scoreText.setFont(font);
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setCharacterSize(50);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(screenWidth / 2 - scoreText.getGlobalBounds().width / 2, screenHeight / 3);

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

void MainGame::reset() {
    fallingShapes.clear();
    scoreBreakdown.clear();
    totalScoreGraph.clear();

    score = 0;
    scoreTimer = 0.f;
    gameOver = false;
    loaded = false;
    spawnedShape = false;

    lastRowTime = -1.0f;
    firstBlockTime = 500.f;

    clock.restart();
    backgroundMusic.stop();
    
    pressed = {false, false, false, false, false};
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
                        score += 10;
                        it->shape.setFillColor(sf::Color::Green);
                        scoreBreakdown.push_back("excellent");
                    }else if((diff <= 15.f && diff >= 5.f) || (diff >= -15.f && diff <= -5.f)){
                        score += 5;
                        it->shape.setFillColor(sf::Color::Yellow);
                        scoreBreakdown.push_back("good");
                    }else if((diff <= 45.f && diff > 15.f) || (diff >= -45.f && diff < -15.f)){
                        score += 1;
                        it->shape.setFillColor(sf::Color::Red);
                        scoreBreakdown.push_back("bad");
                    }
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

