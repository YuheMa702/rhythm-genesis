// src/UI/UploadUI.cpp
#include "UploadUI.hpp"
#include <spdlog/spdlog.h>
#include <thread>
#include <chrono>

UploadUI::UploadUI(sf::RenderWindow* window, const std::string& selectedFile)
    : window(window), selectedFile(selectedFile), exitRequested(false), newSong("") {

    // Get current screen dimensions
    auto winSize = window->getSize();
    float screenWidth = static_cast<float>(winSize.x);
    float screenHeight = static_cast<float>(winSize.y);

    // Title Label
    int titleFontSize = static_cast<int>(screenHeight * 0.06f);
    titleLabel = new UILabel("Upload Music", screenWidth * 0.1f, screenHeight * 0.1f, titleFontSize);

    // File Label: shows selected file path
    int fileFontSize = static_cast<int>(screenHeight * 0.04f);
    fileLabel = new UILabel("Selected File: " + selectedFile, screenWidth * 0.1f, screenHeight * 0.2f, fileFontSize);

    // Progress Bar
    float progressBarWidth = screenWidth * 0.8f;
    float progressBarHeight = screenHeight * 0.03f;
    progressBar = new UIProgressBar(screenWidth * 0.1f, screenHeight * 0.3f, progressBarWidth, progressBarHeight);

    // Song Name Input Box and Label
    songNameBox = new UITextBox(screenWidth * 0.1f, screenHeight * 0.5f, screenWidth * 0.3f);
    UILabel* songNameLabel = new UILabel("Song Name:", screenWidth * 0.1f, screenHeight * 0.4f, fileFontSize);
    uiManager.addElement(songNameLabel);

    // Save Button – triggers simulated beat map generation progress.
    int buttonFontSize = static_cast<int>(screenHeight * 0.04f);
    saveButton = new UIButton("Save", screenWidth * 0.1f, screenHeight * 0.6f, screenWidth * 0.15f, screenHeight * 0.07f,
                              buttonFontSize, [this]() {
        spdlog::info("Save button clicked");
        // Simulate beat map generation progress (blocking simulation).
        for (int i = 0; i <= 100; i++) {
            progressBar->setProgress(i / 100.0f);
            this->window->clear(sf::Color::Black);
            uiManager.draw(*this->window);
            this->window->display();
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }
        // Show confirmation message after processing.
        confirmationLabel->setVisible(true);
    });

    // Back Button – exits the Upload UI and saves the entered song name.
    backButton = new UIButton("Back", screenWidth * 0.3f, screenHeight * 0.6f, screenWidth * 0.15f, screenHeight * 0.07f,
                              buttonFontSize, [this]() {
        spdlog::info("Back button clicked");
        newSong = songNameBox->getText();  // Retrieve song name input.
        exitRequested = true;
    });

    // Confirmation Label (initially hidden)
    confirmationLabel = new UILabel("Upload Successful!", screenWidth * 0.1f, screenHeight * 0.7f, buttonFontSize);
    confirmationLabel->setVisible(false);

    // Add elements to UIManager.
    uiManager.addElement(titleLabel);
    uiManager.addElement(fileLabel);
    uiManager.addElement(progressBar);
    uiManager.addElement(songNameBox);
    uiManager.addElement(saveButton);
    uiManager.addElement(backButton);
    uiManager.addElement(confirmationLabel);
}

UploadUI::~UploadUI() {
    delete titleLabel;
    delete fileLabel;
    delete progressBar;
    delete songNameBox;
    delete saveButton;
    delete backButton;
    delete confirmationLabel;
}

void UploadUI::run() {
    // Modal loop until exit is requested.
    while (window->isOpen() && !exitRequested) {
        sf::Event event;
        while (window->pollEvent(event)) {
            // Allow closing the window.
            if (event.type == sf::Event::Closed) {
                window->close();
            }
            uiManager.handleEvent(event);
        }
        window->clear(sf::Color::Black);
        uiManager.draw(*window);
        window->display();
    }
}
