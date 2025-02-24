#include <spdlog/spdlog.h>

#include <SFML/Graphics.hpp>

#include "UI/UIManager.hpp"

// int main() {
//     // Initialize logging
//     spdlog::info("Starting the game...");

//     // Get desktop info
//     sf::VideoMode desktop{sf::VideoMode::getDesktopMode()};
//     unsigned int screenWidth{desktop.width};
//     unsigned int screenHeight{desktop.height};

//     // Set initial window size (adjustable)
//     unsigned int windowWidth{800};
//     unsigned int windowHeight{600};

//     // Create a resizable window
//     sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Iris Engine",
//                             sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close);
//     // vsync not supported by my driver
//     window.setFramerateLimit(60);
//     // Center the window on the screen
//     window.setPosition(
//         sf::Vector2i((screenWidth - windowWidth) / 2, (screenHeight - windowHeight) / 2));

//     // Load texture
//     sf::Texture virusTexture;
//     if (!virusTexture.loadFromFile("../assets/virus.png")) {
//         spdlog::error("Failed to load texture");
//         return -1;
//     }
//     sf::Texture mapTexture;
//     if (!mapTexture.loadFromFile("../assets/map.jpg")) {
//         spdlog::error("Failed to load texture");
//         return -1;
//     }

//     // Create a sprite
//     sf::Sprite playerSprite;
//     playerSprite.setTexture(virusTexture);
//     playerSprite.setPosition(200.f, 200.f);  // Starting position

//     // Create a map
//     sf::Sprite map;
//     map.setTexture(mapTexture);
//     map.setPosition(0, 0);

//     // Set up camera (sf::View)
//     sf::View camera(sf::FloatRect(0, 0, windowWidth/2, windowHeight/2));

//     while (window.isOpen()) {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             if (event.type == sf::Event::Closed) {
//                 spdlog::info("Window closed");
//                 window.close();
//             }
//         }

//         // Simulate player movement (for testing)
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
//             playerSprite.move(2.0f, 0.0f);
//         }
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
//             playerSprite.move(-2.0f, 0.0f);
//         }
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
//             playerSprite.move(0.0f, -2.0f);
//         }
//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
//             playerSprite.move(0.0f, 2.0f);
//         }

//         // Update camera position to follow player
//         camera.setCenter(playerSprite.getPosition());

//         // Apply camera
//         window.setView(camera);

//         // Clear screen
//         window.clear(sf::Color(0, 0, 0, 255));  // Black background

//         // Draw everything
//         window.draw(map);
//         window.draw(playerSprite);

//         // Display
//         window.display();
//     }

//     return 0;
// }

int main() {
    // Initialize logging
    spdlog::info("Starting the game...");

    // Get desktop info
    sf::VideoMode desktop{sf::VideoMode::getDesktopMode()};
    unsigned int screenWidth{desktop.width};
    unsigned int screenHeight{desktop.height};

    sf::RenderWindow window(sf::VideoMode(800, 600), "Rhythm Genesis - UI Test",
                            sf::Style::Resize | sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    window.setPosition(sf::Vector2i((screenWidth - 800) / 2, (screenHeight - 600) / 2));
    spdlog::info("Detected screen size: {}x{}", screenWidth, screenHeight);
    UIManager uiManager;

    // // Create UI elements
    // UILabel titleLabel("Rhythm Genesis", 300, 50, 30);
    // UIButton startButton("Start Game", 300, 200, 200, 50, 30,
    //                      []() { spdlog::info("Start Button Clicked!"); });
    // UIButton settingsButton("Settings", 300, 300, 200, 50, 30,
    //                         []() { spdlog::info("Settings Button Clicked!"); });
    // UIButton quitButton("Quit", 300, 400, 200, 50, 30, [&window]() {
    //     spdlog::info("Quit Button Clicked! Closing...");
    //     window.close();
    // });

    // // Add to UI manager
    // uiManager.addElement(&titleLabel);
    // uiManager.addElement(&startButton);
    // uiManager.addElement(&settingsButton);
    // uiManager.addElement(&quitButton);

    // // Game loop
    // while (window.isOpen()) {
    //     sf::Event event;
    //     while (window.pollEvent(event)) {
    //         if (event.type == sf::Event::Closed) {
    //             window.close();
    //         }
    //         uiManager.handleEvent(event);
    //     }

    //     window.clear(sf::Color::Black);
    //     uiManager.draw(window);
    //     window.display();
    // }

    // return 0;

    // Create UI elements
    UILabel settingsLabel("Settings", 300, 50, 20);
    UISlider volumeSlider(300, 150, 200, 0, 100, 50);  // Volume control (0-100)
    UILabel volumeLabel("Volume", 300, 120, 20);
    UITextBox keybindBox(300, 250, 200);  // Keybinding input box
    UILabel keybindLabel("Enter Keybinding", 300, 220, 20);
    UIButton backButton("Back", 300, 400, 200, 50, 20, [&window]() {
        spdlog::info("Returning to main menu");
        window.close();
    });

    // Add to UI manager
    uiManager.addElement(&settingsLabel);
    uiManager.addElement(&volumeLabel);
    uiManager.addElement(&volumeSlider);
    uiManager.addElement(&keybindLabel);
    uiManager.addElement(&keybindBox);
    uiManager.addElement(&backButton);

    // Game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            uiManager.handleEvent(event);
        }

        window.clear(sf::Color::Black);
        uiManager.draw(window);
        window.display();
    }

    return 0;
}