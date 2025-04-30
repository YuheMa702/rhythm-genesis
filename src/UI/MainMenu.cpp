// src/UI/MainMenu.cpp
#include "MainMenu.hpp"
#include "UploadUI.hpp"
#include <spdlog/spdlog.h>
#include "tinyfiledialogs.h" // make sure tinyfiledialogs is available
#include "UILabel.hpp"

MainMenu::MainMenu(sf::RenderWindow* window) : window(window) {
    // Get current window dimensions
    auto winSize = window->getSize();
    float screenWidth = static_cast<float>(winSize.x);
    float screenHeight = static_cast<float>(winSize.y);

    // --- Left Panel (Buttons & Title) ---
    // Left panel occupies the left half of the screen.
    float leftPanelWidth = screenWidth / 2.0f;
    // Define button size relative to left panel and screen height.
    float buttonWidth = leftPanelWidth * 0.8f;
    float buttonHeight = screenHeight * 0.1f; // 10% of screen height
    // Center the button horizontally within the left half.
    float leftMargin = (leftPanelWidth - buttonWidth) / 2.0f;
    // Compute vertical gaps for 4 buttons + tittle (7 gaps: before, between, and after buttons + tittle)
    float verticalGap = (screenHeight - 4 * buttonHeight) / 7.0f;

    // Create a title label at the top of the left panel.
    // Estimate a title font size (e.g. 8% of screen height)
    int titleFontSize = static_cast<int>(screenHeight * 0.08f);
    // Place title with a small top gap (centered approximately in left panel)
    title = new UILabel("Rhythm Genesis", leftMargin, verticalGap / 2.0f, titleFontSize);

    // Compute y positions for each button.
    float uploadY = verticalGap + title->isVisible() * (titleFontSize + verticalGap);  // optionally leave space for title
    // For simplicity, assume title height is roughly equal to titleFontSize.
    uploadY = verticalGap + titleFontSize + verticalGap; 
    float beatMapEditorY = uploadY + buttonHeight + verticalGap;
    float settingsY = beatMapEditorY + buttonHeight + verticalGap;
    float quitY = settingsY + buttonHeight + verticalGap;

    // Create buttons on the left side.
    uploadButton = new UIButton("Upload Music", leftMargin, uploadY, buttonWidth, buttonHeight, static_cast<int>(buttonHeight * 0.4f),[this, window]() {
        spdlog::info("Upload Music button clicked");
        // File filters for MP3, WAV, and OGG.
        const char* filters[] = {"*.mp3", "*.wav", "*.ogg"};
        const char* selected = tinyfd_openFileDialog("Select Music File", "", 3, filters, "Music Files", 0);
        if (selected) {
            spdlog::info("File selected: {}", selected);
            // Launch the Upload UI screen with the selected file.
            UploadUI uploadUI(window, selected);
            uploadUI.run();
            // Retrieve the new song name entered by the user.
            std::string newSong = uploadUI.getNewSong();
            if (!newSong.empty()) {
                // Add the new song to the song list.
                songList->addItem(newSong);
                spdlog::info("New song added: {}", newSong);
            }
        } else {
            spdlog::info("No file selected.");
        }
    });
    beatMapEditorButton = new UIButton("Beat Map Editor", leftMargin, beatMapEditorY, buttonWidth, buttonHeight, static_cast<int>(buttonHeight * 0.4f), []() {
        spdlog::info("Beat Map Editor button clicked");
        // TODO: Transition to beat map editor.
    });
    settingsButton = new UIButton("Settings", leftMargin, settingsY, buttonWidth, buttonHeight, static_cast<int>(buttonHeight * 0.4f), []() {
        spdlog::info("Settings button clicked");
        // TODO: Open settings menu.
    });
    quitButton = new UIButton("Quit", leftMargin, quitY, buttonWidth, buttonHeight, static_cast<int>(buttonHeight * 0.4f), [this]() {
        spdlog::info("Quit button clicked");
        this->window->close();
    });

    // --- Right Panel (Song List) ---
    float rightPanelX = screenWidth / 2.0f;
    float songListX = rightPanelX + leftMargin;
    float songListY = verticalGap;
    float songListWidth = (screenWidth / 2.0f) - 2 * leftMargin;
    float songListHeight = screenHeight - 2 * verticalGap;
    int songListFontSize = static_cast<int>(screenHeight * 0.05f);
    float marginX = 20.0f;
    float marginY = songListFontSize * 1.5;

    // Demo list of songs.
    std::vector<std::string> songs = {
        "Song 1", "Song 2", "Song 3", "Song 4", "Song 5",
        "Song 6", "Song 7", "Song 8", "Song 9", "Song 10",
        "Song 1", "Song 2", "Song 3", "Song 4", "Song 5",
        "Song 6", "Song 7", "Song 8", "Song 9", "Song 10",
    };
    songList = new UIScrollableList(songListX, songListY, songListWidth, songListHeight, songs, songListFontSize, marginX, marginY);

    // --- Add elements to UIManager ---
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
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                //detect song being selected from main menu
                for (auto& label : songList->getLabels()) {
                    if (label->getGlobalBounds().contains(mousePos)) {
                        std::string clickedSong = label->getText();
                        spdlog::info("Song: {}", clickedSong);
                        //hard code selections for now, update this later
                        if(clickedSong == "Song 1"){
                            jsonPath = "../src/Game/json/gameTest.json";
                            songPath = "../assets/music/retro-game-arcade-short.ogg";
                        }else if(clickedSong == "Song 2"){
                            jsonPath = "../src/Game/json/clair-de-lune.json";
                            songPath = "../assets/music/Clair-de-Lune.ogg";
                        }else if(clickedSong == "Song 3"){
                            jsonPath = "../src/Game/json/clair-de-lune-chords.json";
                            songPath = "../assets/music/Clair-de-Lune.ogg";
                        }else if(clickedSong == "Song 4"){
                            jsonPath = "../src/Game/json/clair-de-lune-single.json";
                            songPath = "../assets/music/Clair-de-Lune.ogg";
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
