// src/UI/MainMenu.cpp
#include "MainMenu.hpp"
#include "UploadUI.hpp"
#include "BeatMapEditor.hpp"
#include <spdlog/spdlog.h>
#include "tinyfiledialogs.h" // make sure tinyfiledialogs is available

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

    std::vector<std::string> songs = {
        "Song 1", "Song 2", "Song 3", "Song 4", "Song 5",
        "Song 6", "Song 7", "Song 8", "Song 9", "Song 10",
        "Song 11", "Song 12", "Song 13", "Song 14", "Song 15",
        "Song 16", "Song 17", "Song 18", "Song 19", "Song 20",
    };
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
    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window->close();
            uiManager.handleEvent(event);
        }
        window->clear(sf::Color::Black);
        uiManager.draw(*window);
        window->display();
    }
}
