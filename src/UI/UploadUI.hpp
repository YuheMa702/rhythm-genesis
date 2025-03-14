// src/UI/UploadUI.hpp
#ifndef UPLOAD_UI_H
#define UPLOAD_UI_H

#include "UIElement.hpp"
#include "UIButton.hpp"
#include "UILabel.hpp"
#include "UITextBox.hpp"
#include "UIProgressBar.hpp"
#include "UIManager.hpp"
#include <SFML/Graphics.hpp>
#include <string>

class UploadUI {
private:
    UIManager uiManager;
    sf::RenderWindow* window;

    // UI Elements
    UILabel* titleLabel;
    UILabel* fileLabel;        // displays selected file path
    UIProgressBar* progressBar;
    UITextBox* songNameBox;
    UIButton* saveButton;
    UIButton* backButton;
    UILabel* confirmationLabel;

    std::string selectedFile;
    std::string newSong;       // Stores the song name entered by the user.
    bool exitRequested;        // Flag to exit the Upload UI loop.

public:
    UploadUI(sf::RenderWindow* window, const std::string& selectedFile);
    ~UploadUI();
    void run();
    // Provide access to the new song name after exiting.
    std::string getNewSong() const { return newSong; }
};

#endif
