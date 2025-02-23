#include <spdlog/spdlog.h>

#include <SFML/Graphics.hpp>

int main() {
    // Initialize logging
    spdlog::info("Starting the game...");

    // Get desktop info
    sf::VideoMode desktop{sf::VideoMode::getDesktopMode()};
    unsigned int screenWidth{desktop.width};
    unsigned int screenHeight{desktop.height};

    // Set initial window size (adjustable)
    unsigned int windowWidth{800};
    unsigned int windowHeight{600};

    // Create a resizable window
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Iris Engine",
                            sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close);
    // vsync not supported by my driver
    window.setFramerateLimit(60);
    // Center the window on the screen
    window.setPosition(
        sf::Vector2i((screenWidth - windowWidth) / 2, (screenHeight - windowHeight) / 2));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                spdlog::info("Window closed");
                window.close();
            }
        }
        window.clear(sf::Color(0, 0, 0, 255));  // black
        window.display();
    }

    return 0;
}
