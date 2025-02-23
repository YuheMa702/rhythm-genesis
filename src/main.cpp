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

    // Load texture
    sf::Texture virusTexture;
    if (!virusTexture.loadFromFile("../assets/virus.png")) { 
        spdlog::error("Failed to load texture");
        return -1;
    }
    sf::Texture mapTexture;
    if (!mapTexture.loadFromFile("../assets/map.jpg")) { 
        spdlog::error("Failed to load texture");
        return -1;
    }

    // Create a sprite
    sf::Sprite playerSprite;
    playerSprite.setTexture(virusTexture);
    playerSprite.setPosition(200.f, 200.f);  // Starting position

    // Create a map
    sf::Sprite map;
    map.setTexture(mapTexture);
    map.setPosition(0, 0);

    // Set up camera (sf::View)
    sf::View camera(sf::FloatRect(0, 0, windowWidth/2, windowHeight/2));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                spdlog::info("Window closed");
                window.close();
            }
        }

        // Simulate player movement (for testing)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            playerSprite.move(2.0f, 0.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            playerSprite.move(-2.0f, 0.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            playerSprite.move(0.0f, -2.0f);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            playerSprite.move(0.0f, 2.0f);
        }

        // Update camera position to follow player
        camera.setCenter(playerSprite.getPosition());

        // Apply camera
        window.setView(camera);

        // Clear screen
        window.clear(sf::Color(0, 0, 0, 255));  // Black background

        // Draw everything
        window.draw(map);
        window.draw(playerSprite);

        // Display
        window.display();
    }

    return 0;
}
