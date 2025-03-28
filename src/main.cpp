#include <iostream>

#include <spdlog/spdlog.h>

#include <SFML/Graphics.hpp>

#include "../src/UI/MainMenu.hpp"
#include "../src/Game/MainGame.hpp"

#include "Bridge/PythonBridge.hpp"


#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <cmath>


int main() {

    // // Initialize the Python interpreter and add the "src/python" directory to sys.path
    // if (!PythonBridge::initialize("../src/Python/")) {
    //     std::cerr << "Python initialization failed!" << std::endl;
    //     return 1;
    // }

    // // Call the Python function to parse a beatmap file and get the JSON string result.
    // std::string beatmapJSON = PythonBridge::parseBeatmapFile("../assets/music/retro-game-arcade-short.mp3");
    // std::cout << "Beatmap JSON: " << beatmapJSON << std::endl;

    // // Finalize the Python interpreter before exiting
    // PythonBridge::finalize();
    // return 0;

    // Initialize logging

    spdlog::info("Starting Rhythm Genesis Main Menu...");

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    unsigned int screenWidth = desktop.width;
    unsigned int screenHeight = desktop.height;
                            
    sf::RenderWindow window(sf::VideoMode(800, 600), "Rhythm Genesis - Main Menu", sf::Style::Close);
    window.setFramerateLimit(30);
    window.setPosition(sf::Vector2i(0, 0));
    spdlog::info("Detected screen size: {}x{}", screenWidth, screenHeight);

    MainMenu mainMenu(&window);
    MainGame mainGame(&window);
    mainGame.run("../src/Game/gameTest.json");
    // mainMenu.run();

    return 0;
}