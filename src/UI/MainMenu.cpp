// src/UI/MainMenu.cpp

#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include "MainMenu.hpp"
#include "UploadUI.hpp"
#include "BeatMapEditor.hpp"
#include <spdlog/spdlog.h>
#include "tinyfiledialogs.h" // make sure tinyfiledialogs is available
#include "UILabel.hpp"
namespace fs = std::filesystem;

std::vector<std::string> getOggFiles(const std::string& folderPath) {
    std::vector<std::string> oggFiles;

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".ogg") {
            spdlog::info("getOggFile: {}", entry.path().string());
            oggFiles.push_back(entry.path().filename().stem().string()); // or .string() for full path
        }
    }
    std::sort(oggFiles.begin(), oggFiles.end());  // Sort alphabetically
    return oggFiles;
}

std::vector<std::string> getJsonFiles(const std::string& folderPath) {
    std::vector<std::string> jsonFiles;

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") {
            spdlog::info("getOggFile: {}", entry.path().string());
            jsonFiles.push_back(entry.path().filename().stem().string()); // or .string() for full path
        }
    }
    return jsonFiles;
}



MainMenu::MainMenu(sf::RenderWindow* window) : window(window) {
    auto winSize = window->getSize();
    float screenWidth = static_cast<float>(winSize.x);
    float screenHeight = static_cast<float>(winSize.y);

    float leftPanelWidth = screenWidth / 2.0f;
    float buttonWidth = leftPanelWidth * 0.8f;
    float buttonHeight = screenHeight * 0.1f;
    float leftMargin = (leftPanelWidth - buttonWidth) / 2.0f;
    float verticalGap = (screenHeight - 4 * buttonHeight) / 7.0f;

    int titleFontSize = static_cast<int>(screenHeight * 0.08f);
    title = new UILabel("Rhythm Genesis", leftMargin, verticalGap / 2.0f, titleFontSize);

    float uploadY = verticalGap + titleFontSize + verticalGap;
    float beatMapEditorY = uploadY + buttonHeight + verticalGap;
    float settingsY = beatMapEditorY + buttonHeight + verticalGap;
    float quitY = settingsY + buttonHeight + verticalGap;

    uploadButton = new UIButton("Upload Music", leftMargin, uploadY, buttonWidth, buttonHeight, static_cast<int>(buttonHeight * 0.4f), [this, window]() {
        spdlog::info("Upload Music button clicked");
        const char* filters[] = {"*.mp3", "*.wav", "*.ogg"};
        const char* selected = tinyfd_openFileDialog("Select Music File", "", 3, filters, "Music Files", 0);
        if (selected) {
            spdlog::info("File selected: {}", selected);
            UploadUI uploadUI(window, selected);
            uploadUI.run();
            std::string newSong = uploadUI.getNewSong();
            if (!newSong.empty()) {
                songList->addItem(newSong);
                spdlog::info("New song added: {}", newSong);
            }
        } else {
            spdlog::info("No file selected.");
        }
    });

    beatMapEditorButton = new UIButton("Beat Map Editor", leftMargin, beatMapEditorY, buttonWidth, buttonHeight, static_cast<int>(buttonHeight * 0.4f), [this, window]() {
        spdlog::info("Beat Map Editor button clicked");
        BeatMapEditor editor(window);
        editor.run();
    });

    settingsButton = new UIButton("Settings", leftMargin, settingsY, buttonWidth, buttonHeight, static_cast<int>(buttonHeight * 0.4f), []() {
        spdlog::info("Settings button clicked");
    });

    quitButton = new UIButton("Quit", leftMargin, quitY, buttonWidth, buttonHeight, static_cast<int>(buttonHeight * 0.4f), [this]() {
        spdlog::info("Quit button clicked");
        this->window->close();
    });

    float rightPanelX = screenWidth / 2.0f;
    float songListX = rightPanelX + leftMargin;
    float songListY = verticalGap;
    float songListWidth = (screenWidth / 2.0f) - 2 * leftMargin;
    float songListHeight = screenHeight - 2 * verticalGap;
    int songListFontSize = static_cast<int>(screenHeight * 0.05f);
    float marginX = 20.0f;
    float marginY = songListFontSize * 1.5;

    // hardcoded song list
    // std::vector<std::string> songs = {
    //     "Song 1", "Song 2", "Song 3", "Song 4",
    // };

    songs = getOggFiles("../src/Game/music/");
    //std::vector<std::string> songs = getOggFiles("../assets/music/");
    songList = new UIScrollableList(songListX, songListY, songListWidth, songListHeight, songs, songListFontSize, marginX, marginY);

    uiManager.addElement(title);
    uiManager.addElement(uploadButton);
    uiManager.addElement(beatMapEditorButton);
    uiManager.addElement(settingsButton);
    uiManager.addElement(quitButton);
    uiManager.addElement(songList);
}

MainMenu::~MainMenu() {
    delete title;
    delete songList;
    delete uploadButton;
    delete beatMapEditorButton;
    delete settingsButton;
    delete quitButton;
}

void MainMenu::run() {
    songs = getOggFiles("../src/Game/music/");
    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window->close();
            uiManager.handleEvent(event);
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                //detect song being selected from main menu
                for (auto& label : songList->getLabels()) {
                    if (label->getGlobalBounds().contains(mousePos)) {
                        std::string clickedSong = label->getText();
                        spdlog::info("Song: {}", clickedSong);
                        //hard code selections for now, update this later
                        // if(clickedSong == "Song 1"){
                        //     jsonPath = "../src/Game/json/gameTest.json";
                        //     songPath = "../assets/music/retro-game-arcade-short.ogg";
                        // }else if(clickedSong == "Song 2"){
                        //     jsonPath = "../src/Game/json/clair-de-lune.json";
                        //     songPath = "../assets/music/Clair-de-Lune.ogg";
                        // }else if(clickedSong == "Song 3"){
                        //     jsonPath = "../src/Game/json/clair-de-lune-chords.json";
                        //     songPath = "../assets/music/Clair-de-Lune.ogg";
                        // }else if(clickedSong == "Song 4"){
                        //     jsonPath = "../src/Game/json/clair-de-lune-single.json";
                        //     songPath = "../assets/music/Clair-de-Lune.ogg";
                        // }


                        // if(clickedSong == "Song 1"){
                        //     jsonPath = "../src/Game/json/gameTest.json";
                        //     songPath = "../assets/music/retro-game-arcade-short.ogg";
                        // }else if(clickedSong == "Song 2"){
                        //     jsonPath = "../src/Game/json/clair-de-lune.json";
                        //     songPath = "../assets/music/Clair-de-Lune.ogg";
                        // }

                        for (auto& song : songs) {
                            spdlog::info("song: {}", song);
                            if (clickedSong == song) {
                                jsonPath = "../src/Game/json/" + song + ".json";
                                songPath = "../src/Game/music/" + song + ".ogg";
                            };
                        }

                        menuRunning = false;
                        return;
                    }
                }
            }
        }
        window->clear(sf::Color::Black);
        uiManager.draw(*window);
        window->display();
    }
}
