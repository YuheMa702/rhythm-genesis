// src/UI/MainMenu.hpp
#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <SFML/Graphics.hpp>
#include "UIManager.hpp"
#include "UIButton.hpp"
#include "UILabel.hpp"
#include "UIScrollableList.hpp"
#include <vector>
#include <string>

class MainMenu {
private:
    UIManager uiManager;
    sf::RenderWindow* window;

    // UI elements (allocated dynamically to persist for the menu’s lifetime)
    UILabel* title;
    std::vector<std::string> songs;
    UIScrollableList* songList;
    UIButton* uploadButton;
    UIButton* beatMapEditorButton;
    UIButton* settingsButton;
    UIButton* quitButton;

public:
    MainMenu(sf::RenderWindow* window);
    ~MainMenu();
    void run();
    bool menuRunning = true;
    std::string jsonPath;
    std::string songPath;
};

#endif
